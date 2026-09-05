#include "pch.h"
#include "ImGuiManager.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

void ImGuiManager::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(GAME->GetGameDesc().hWnd);

    ImGui_ImplDX12_InitInfo* info = new ImGui_ImplDX12_InitInfo();
    CreateView();
    info->Device = DEVICE.Get();
    info->CommandQueue = COMMAND_QUEUE.Get();
    info->SrvDescriptorHeap = _descHeap.Get();
    info->NumFramesInFlight = SWAP_CHAIN_BUFFER_COUNT;
    info->RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    info->DSVFormat = DXGI_FORMAT_UNKNOWN;
    info->SrvDescriptorAllocFn = nullptr;
    info->SrvDescriptorFreeFn = nullptr;
    info->LegacySingleSrvCpuDescriptor = _descHeap->GetCPUDescriptorHandleForHeapStart();
    info->LegacySingleSrvGpuDescriptor = _descHeap->GetGPUDescriptorHandleForHeapStart();
    ImGui_ImplDX12_Init(info);
}

void ImGuiManager::Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), COMMAND_LIST.Get());
}

void ImGuiManager::CreateView()
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    // 셰이더에서 직접 참조할 수 있게 세팅
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    heapDesc.NumDescriptors = 1;
    ThrowIfFailed(DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_descHeap.GetAddressOf())));
}
