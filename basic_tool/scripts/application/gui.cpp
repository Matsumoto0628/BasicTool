#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "render_context.h"

Gui::Gui(HWND hWnd, RenderContext* pContext)
	: m_hWnd{ hWnd }, m_pContext{ pContext }
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
}

void Gui::Start()
{
	ImGui::StyleColorsDark();
	SceneObject a("a");
	SceneObject b("b");
	a.children.push_back(b);
	m_sceneObjects.push_back(a);
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

void Gui::Terminate()
{
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
	ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowSize(ImVec2(250, 540), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, fixedFlags);

	{
		int selected = m_selectedIndex;
		int index = 0;
		for (auto& obj : m_sceneObjects)
		{
			drawHierarchyNode(obj, selected, index);
		}
		m_selectedIndex = selected;
	}

	ImGui::End();
}

void Gui::drawInspector()
{
	ImGuiWindowFlags fixedFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);
	ImGui::SetNextWindowSize(ImVec2(250, 540), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(710, 20), ImGuiCond_Always);
	ImGui::Begin("Inspector", nullptr, fixedFlags);

	// ここにコンポーネントのパラメータを表示する
	{
		ImGui::SliderFloat("Intensity", &m_params.intensity, 0.0f, 5.0f);
		ImGui::SliderFloat("Radius", &m_params.radius, 0.0f, 1.0f);
		ImGui::Checkbox("Glow", &m_params.enableGlow);
	}

	ImGui::End();
}

void Gui::drawHierarchyNode(SceneObject& obj, int& selectedIndex, int& index)
{
	int currentIndex = index++;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (selectedIndex == currentIndex)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	else if (obj.children.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (ImGui::IsItemClicked())
	{
		selectedIndex = currentIndex;
	}

	bool nodeOpen = ImGui::TreeNodeEx(obj.name.c_str(), flags);
	if (nodeOpen)
	{
		for (auto& child : obj.children)
		{
			drawHierarchyNode(child, selectedIndex, index);
		}
		ImGui::TreePop();
	}
}