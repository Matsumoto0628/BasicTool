#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

Gui::Gui(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_hWnd(hWnd), m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
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
		ImGui_ImplDX11_Init(m_pDevice.Get(), m_pDeviceContext.Get());
	}
}

void Gui::Start()
{
	ImGui::StyleColorsDark();
}

void Gui::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Gui::Draw()
{
	// UI作成
	{
		ImGui::Begin("Hello!");
		ImGui::Text("Hello, world!");
		ImGui::End();
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