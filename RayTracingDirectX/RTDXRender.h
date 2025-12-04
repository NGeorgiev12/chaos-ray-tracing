#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

class RTDXRender
{
public:

	// Initiates DirectX 12 renderer
	void render();

private: // Functions

	// Create neccessary DirectX 12 infrastructure
	void prepareForRendering();

	// Create D3D12 device, an interface to the GPU which allows access to its resources and capabilities
	void createDevice();

	// Create ID3D12CommandQueue, Allocator and CommandList to submit rendering commands to the GPU
	void createCommandManager();


private: // Members	

	IDXGIFactory4* dxgiFactory = nullptr;
	IDXGIAdapter1* dxgiAdapter = nullptr;
	ID3D12Device* d3d12Device = nullptr;

	// Holds rendering commands before they are executed by the GPU
	ID3D12CommandQueue* commandQueue = nullptr;

	// The actual command list to record rendering commands
	ID3D12GraphicsCommandList* commandList = nullptr;

	// Allocates memory for command lists
	ID3D12CommandAllocator* commandAllocator = nullptr;
};

