#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "render_context.h"
#include "game_object.h"

Gui::Gui(HWND hWnd, const RenderContext* const pContext, std::vector<std::unique_ptr<GameObject>>* ppGameObjects)
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
	}

	// 描画
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void Gui::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gui::drawMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) { /* 新規処理 */ }
			if (ImGui::MenuItem("Open")) { /* 開く処理 */ }
			if (ImGui::MenuItem("Save")) { /* 保存処理 */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) { /* 終了処理 */ }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo")) {}
			if (ImGui::MenuItem("Redo")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Gui::drawHierarchy()
{
	const ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

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
}

void Gui::drawInspector()
{
	const ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBg] = ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f };
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.02f, 0.02f, 0.02f, 1.0f};
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
	ImGui::PopID();

	if (ImGui::IsItemClicked()) // TreeNodeEx()を先にしないとクリックできなくなる
	{
		m_pSelectGameObject = pGameObject;
	}
	
	if (nodeOpen)
	{
		for (auto& child : pGameObject->GetTransform().GetChildren())
		{
			drawHierarchyNode(&child->GetGameObject());
		}
		ImGui::TreePop();
	}
}