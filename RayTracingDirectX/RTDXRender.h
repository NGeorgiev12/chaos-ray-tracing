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

	// Render a frame: record and submit commands, wait for GPU to finish
	void renderFrame();

	// Release all allocated resources
	void cleanUp();

	// Create D3D12 device, an interface to the GPU which allows access to its resources and capabilities
	void createDevice();

	// Create ID3D12CommandQueue, Allocator and CommandList to submit rendering commands to the GPU
	void createCommandManager();

	// Create a render target to which the GPU will render
	void createRenderTarget();

	// Create a Render Target View (RTV) descriptor to the render target
	void createRenderTargetView();

	// Add commands to the command list to generate a texture filled with a constant color
	void generateConstColorTexture();

	// Create a read-back buffer to copy the rendered texture from GPU to CPU and fill it
	void createReadbackBuffer();

	// Copy the rendered texture from GPU to the read-back buffer
	void copyTextureToReadbackBuffer();

	// Create a fence for synchronizing CPU and GPU after frame rendering
	void createFence();

	// Stall the CPU until the GPU finishes with the frame rendering
	void waitForGPURenderFrame();

	// Close the command list and execute it on the command queue
	void closeListAndExecuteQueue();

	// Prepare command list for recording commands for a new frame
	void frameBegin();

	// Execute the recorded commands and present the rendered frame
	void frameEnd();

	// Write the rendered image from the read-back buffer to a file
	void writeImageToFile();

private: // Members	

	// DXGI Factory to create other DXGI objects
	IDXGIFactory4* dxgiFactory = nullptr;

	// The graphics adapter (GPU)
	IDXGIAdapter1* dxgiAdapter = nullptr;

	// The Direct3D 12 device
	ID3D12Device* d3d12Device = nullptr;

	// Holds rendering commands before they are executed by the GPU
	ID3D12CommandQueue* commandQueue = nullptr;

	// The actual command list to record rendering commands
	ID3D12GraphicsCommandList* commandList = nullptr;

	// Allocates memory for command lists
	ID3D12CommandAllocator* commandAllocator = nullptr;

	// The render target resource (a texture we render to in GPU)
	ID3D12Resource* renderTarget = nullptr;

	// Description of the render target resource
	D3D12_RESOURCE_DESC renderTargetDesc;

	// The heap which holds the render target of the texture
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	// Handle for the descriptor of the texture with which it could be used in the pipeline
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	// A read-back buffer to copy the rendered texture from GPU to CPU
	ID3D12Resource* readbackBuffer = nullptr;

	// Memory layout of the render target for copying to the read-back buffer
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT renderTargetFootprint;

	// Fence for synchronizing CPU and GPU after frame rendering
	ID3D12Fence* renderFrameFence = nullptr;

	// The event which is signaled when the GPU has finished rendering the frame
	HANDLE renderFrameEvent = nullptr;

	// The value to be set to the fence after the GPU has finished rendering the frame
	UINT64 renderFrameFenceValue = 1;
};

