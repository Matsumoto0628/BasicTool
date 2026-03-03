#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "render_context.h"
#include "game_object.h"
#include "scene_manager.h"
#include "particle_controller.h"
#include "camera.h"
#include <windows.h>
#include <commdlg.h>
#include <filesystem>

Gui::Gui(HWND hWnd, const RenderContext* const pContext, const std::vector<std::unique_ptr<GameObject>>* ppGameObjects)
	: m_hWnd{ hWnd }, m_pContext{ pContext }, m_ppGameObjects{ ppGameObjects }
{
}

Gui::~Gui() 
{
	m_hWnd = nullptr;
}

void Gui::Initialize() 
{
	// imguiの初期化
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		static_cast<void>(io);
	}

	// バックエンドの初期化
	{
		ImGui_ImplWin32_Init(m_hWnd);
		ImGui_ImplDX11_Init(m_pContext->GetDevice(), m_pContext->GetDeviceContext());
	}

	ImGui::StyleColorsDark();
}

void Gui::Update()
{
	// フレーム更新
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	// UI作成
	{
		drawMainMenu();
		drawHierarchy();
		drawInspector();
		if (m_openPopupRequestd)
		{
			m_openPopupRequestd = false;
			ImGui::OpenPopup("Save");
		}
		drawSerializePopup();
		if (m_openExportRequested)
		{
			m_openExportRequested = false;
			ImGui::OpenPopup("Export");
		}

		drawExportPopup();
	}

	// 描画
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	checkOpenFile();
	checkExit();
}

void Gui::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gui::drawMainMenu()
{
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) 
			{ 
				m_openFileRequested = true;
			}
			if (ImGui::MenuItem("Save")) 
			{ 
				m_openPopupRequestd = true;
			}
			if (ImGui::MenuItem("Export"))
			{
				m_openExportRequested = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) 
			{ 
				m_exitRequested = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Create Controller")) 
			{
				auto pCameraGameObject = SceneManager::GetCurrentScene()->FindGameObject("Camera");
				auto pCamera = pCameraGameObject->FindComponent<Camera>(Component::Type::Camera);
				auto& particleControllerGameObject = SceneManager::GetCurrentScene()->Instantiate("ParticleController", true);
				particleControllerGameObject.AddComponent<ParticleController>(
					&particleControllerGameObject.GetTransform(), 
					m_pContext, 
					pCamera, 
					&pCameraGameObject->GetTransform()
				);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleColor();
}

void Gui::drawHierarchy()
{
	const ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::SetNextWindowSize(ImVec2{ 250, 520 }, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2{ 0, 20 }, ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, fixedFlags);

	{
		for (auto& pGameObject : *m_ppGameObjects)
		{
			if (!pGameObject->GetTransform().GetParent()) 
			{
				drawHierarchyNode(pGameObject.get());
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleColor(3);
}

void Gui::drawInspector()
{
	const ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4{ 0.01f, 0.01f, 0.01f, 1.0f });
	ImGui::SetNextWindowSize(ImVec2{ 250, 520 }, ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2{ 710, 20 }, ImGuiCond_Always);
	ImGui::Begin("Inspector", nullptr, fixedFlags);

	// ここにコンポーネントのパラメータを表示する
	for (auto& pGameObject : *m_ppGameObjects)
	{
		if (pGameObject.get() == m_pSelectGameObject) // ->でアクセスはできるが、関数に渡すときは.get()が必要
		{
			pGameObject->GetTransform().Show();
			for (auto& pComponent : *pGameObject->GetComponents()) 
			{
				pComponent->Show();
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleColor(3);
}

void Gui::drawHierarchyNode(GameObject* pGameObject)
{
	ImGuiTreeNodeFlags flags =
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	if (pGameObject == m_pSelectGameObject)
	{
		flags |= ImGuiTreeNodeFlags_Selected; // フラグを追加する
	}
	if (pGameObject->GetTransform().GetChildren().empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	ImGui::PushID(pGameObject);
	bool nodeOpen = ImGui::TreeNodeEx(pGameObject->GetName().c_str(), flags);

	if (ImGui::IsItemClicked()) // TreeNodeEx()を先にしないとクリックできなくなる
	{
		m_pSelectGameObject = pGameObject;
	}

	if (ImGui::BeginPopupContextItem())
	{
		m_pSelectGameObject = pGameObject;

		if (ImGui::MenuItem("Delete"))
		{
			if (pGameObject->GetName() != "Camera")
			{
				pGameObject->Destroy();
			}
		}

		ImGui::EndPopup();
	}
	
	if (nodeOpen)
	{
		for (auto& child : pGameObject->GetTransform().GetChildren())
		{
			drawHierarchyNode(&child->GetGameObject());
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void Gui::drawSerializePopup()
{
	if (ImGui::BeginPopupModal("Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter file name:");
		ImGui::InputText("##SaveFileName", m_serializeFileName, sizeof(m_serializeFileName));

		ImGui::Separator();

		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			// 保存処理
			SceneManager::GetCurrentScene()->Serialize(m_serializeFileName);

			m_openPopupRequestd = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_openPopupRequestd = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

std::string Gui::openFileDialog()
{
	char fileName[MAX_PATH] = "";

	OPENFILENAMEA ofn{};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = "json";

	if (GetOpenFileNameA(&ofn))
	{
		return std::string(fileName);
	}

	return "";
}

void Gui::checkOpenFile()
{
	if (m_openFileRequested)
	{
		m_openFileRequested = false;
		std::string fullPath = openFileDialog();
		if (!fullPath.empty())
		{
			std::filesystem::path path(fullPath);
			std::string sceneName = path.stem().string();
			SceneManager::GetCurrentScene()->Deserialize(sceneName);
		}
	}
}

void Gui::checkExit()
{
	if (m_exitRequested)
	{
		m_exitRequested = false;
		PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
}

void Gui::drawExportPopup()
{
	if (ImGui::BeginPopupModal("Export", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("File name:");
		ImGui::InputText("##ExportFileName", m_exportFileName, sizeof(m_exportFileName));

		ImGui::Separator();

		ImGui::InputInt("Horizontal Count", &m_exportNumW);
		ImGui::InputInt("Vertical Count", &m_exportNumH);

		// 最低値保証
		if (m_exportNumW < 1) m_exportNumW = 1;
		if (m_exportNumH < 1) m_exportNumH = 1;

		ImGui::Separator();

		if (ImGui::Button("Export", ImVec2(120, 0)))
		{
			std::wstring wFileName =
				std::filesystem::path(m_exportFileName).wstring();

			SceneManager::GetCurrentScene()
				->GetContext()
				->Export(wFileName,
					static_cast<UINT>(m_exportNumW),
					static_cast<UINT>(m_exportNumH));

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}