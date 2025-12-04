#include "RTDXRender.h"
#include <assert.h>
#include <iostream>

void RTDXRender::render()
{
	prepareForRendering();
}

void RTDXRender::prepareForRendering()
{
	createDevice();
	createCommandManager();
}

void RTDXRender::createDevice()
{
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr) && "Failed to create DXGI Factory!");

	for (UINT adapterIndex = 0; dxgiFactory->EnumAdapters1(adapterIndex, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; adapterIndex++)
	{
		DXGI_ADAPTER_DESC1 desc;
		dxgiAdapter->GetDesc1(&desc);

		hr = D3D12CreateDevice(
			dxgiAdapter,
			D3D_FEATURE_LEVEL_12_0,
			IID_PPV_ARGS(&d3d12Device));

		if (SUCCEEDED(hr))
		{
			std::wcout << L"Using GPU: " << desc.Description << L"\n";
			break;
		}
	}

	assert(dxgiAdapter != nullptr && "The adapter must not be nullptr!");
}

void RTDXRender::createCommandManager()
{
	const D3D12_COMMAND_LIST_TYPE commandListType = D3D12_COMMAND_LIST_TYPE_DIRECT; // All types of commands

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = commandListType;
	HRESULT hr = d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(hr) && "Failed to create Command Queue!");

	hr = d3d12Device->CreateCommandAllocator(
		commandListType,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(hr) && "Failed to create Command Allocator!");

	hr = d3d12Device->CreateCommandList(
		0,
		commandListType,
		commandAllocator,
		nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(hr) && "Failed to create Command List!");

}
