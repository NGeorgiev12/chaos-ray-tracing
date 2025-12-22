#include "RTDXRender.h"
#include <assert.h>
#include <iostream>
#include <fstream>

void RTDXRender::render()
{
	prepareForRendering();
	renderFrame();
	cleanUp();
}

void RTDXRender::prepareForRendering()
{
	createDevice();
	createCommandManager();
	createRenderTarget();
	createRenderTargetView();
	createFence();
}

void RTDXRender::renderFrame()
{
	generateConstColorTexture();
	createReadbackBuffer();
	closeListAndExecuteQueue();
	waitForGPURenderFrame();
	frameEnd();
}

void RTDXRender::cleanUp()
{
	if (renderFrameEvent)
	{
		CloseHandle(renderFrameEvent);
		renderFrameEvent = nullptr;
	}
	if (renderFrameFence)
	{
		renderFrameFence->Release();
		renderFrameFence = nullptr;
	}
	if (readbackBuffer)
	{
		readbackBuffer->Release();
		readbackBuffer = nullptr;
	}
	if (renderTarget)
	{
		renderTarget->Release();
		renderTarget = nullptr;
	}
	if (rtvHeap)
	{
		rtvHeap->Release();
		rtvHeap = nullptr;
	}
	if (commandList)
	{
		commandList->Release();
		commandList = nullptr;
	}
	if (commandAllocator)
	{
		commandAllocator->Release();
		commandAllocator = nullptr;
	}
	if (commandQueue)
	{
		commandQueue->Release();
		commandQueue = nullptr;
	}
	if (d3d12Device)
	{
		d3d12Device->Release();
		d3d12Device = nullptr;
	}
	if (dxgiAdapter)
	{
		dxgiAdapter->Release();
		dxgiAdapter = nullptr;
	}
	if (dxgiFactory)
	{
		dxgiFactory->Release();
		dxgiFactory = nullptr;
	}
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
			IID_PPV_ARGS(&d3d12Device)
		);

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
		IID_PPV_ARGS(&commandAllocator)
	);
	assert(SUCCEEDED(hr) && "Failed to create Command Allocator!");

	hr = d3d12Device->CreateCommandList(
		0,
		commandListType,
		commandAllocator,
		nullptr,
		IID_PPV_ARGS(&commandList)
	);
	assert(SUCCEEDED(hr) && "Failed to create Command List!");

}

void RTDXRender::createRenderTarget()
{
	renderTargetDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	renderTargetDesc.Width = 800;
	renderTargetDesc.Height = 600;
	renderTargetDesc.DepthOrArraySize = 1;
	renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetDesc.SampleDesc.Count = 1;
	renderTargetDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	HRESULT hr = d3d12Device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&renderTargetDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		nullptr,
		IID_PPV_ARGS(&renderTarget)
	);

	assert(SUCCEEDED(hr) && "Failed to create Render Target!");
}

void RTDXRender::createRenderTargetView()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(hr) && "Failed to create RTV Descriptor Heap!");

	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	d3d12Device->CreateRenderTargetView(renderTarget, nullptr, rtvHandle);
}

void RTDXRender::generateConstColorTexture()
{
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	FLOAT clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // RGBA
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void RTDXRender::createReadbackBuffer()
{
	UINT64 readbackBufferSize = 0;
	d3d12Device->GetCopyableFootprints(
		&renderTargetDesc,
		0,
		1,
		0,
		&renderTargetFootprint,
		nullptr,
		nullptr,
		&readbackBufferSize
	);

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_READBACK;
	D3D12_RESOURCE_DESC readbackDesc = {};
	readbackDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	readbackDesc.Width = readbackBufferSize;
	readbackDesc.Height = 1;
	readbackDesc.DepthOrArraySize = 1;
	readbackDesc.MipLevels = 1;
	readbackDesc.SampleDesc.Count = 1;
	readbackDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = d3d12Device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&readbackDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&readbackBuffer)
	);

	assert(SUCCEEDED(hr) && "Failed to create Readback Buffer!");

	copyTextureToReadbackBuffer();
}

void RTDXRender::copyTextureToReadbackBuffer()
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = renderTarget;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;

	commandList->ResourceBarrier(1, &barrier);

	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = renderTarget;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	srcLocation.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.pResource = readbackBuffer;
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	dstLocation.PlacedFootprint = renderTargetFootprint;

	commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrier);
}

void RTDXRender::createFence()
{
	HRESULT hr = d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&renderFrameFence));
	assert(SUCCEEDED(hr) && "Failed to create Fence!");

	renderFrameEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	assert(renderFrameEvent);
}

void RTDXRender::waitForGPURenderFrame()
{
	if (renderFrameFence->GetCompletedValue() < renderFrameFenceValue)
	{
		HRESULT hr = renderFrameFence->SetEventOnCompletion(renderFrameFenceValue, renderFrameEvent);
		assert(SUCCEEDED(hr) && "Failed to set the fence value!");

		WaitForSingleObject(renderFrameEvent, INFINITE);
	}
}

void RTDXRender::closeListAndExecuteQueue()
{
	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr) && "Failed to close Command List!");

	ID3D12CommandList* ppCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	renderFrameFenceValue++;
	hr = commandQueue->Signal(renderFrameFence, renderFrameFenceValue);
	assert(SUCCEEDED(hr) && "Failed to signal Command Queue!");
}

void RTDXRender::frameBegin()
{

}

void RTDXRender::frameEnd()
{
	writeImageToFile();

	HRESULT hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr) && "Failed to reset Command Allocator!");

	hr = commandList->Reset(commandAllocator, nullptr);
	assert(SUCCEEDED(hr) && "Failed to reset Command List!");

	// frameIdx++;
}

void RTDXRender::writeImageToFile()
{
	void* renderTargetData = nullptr;
	HRESULT hr = readbackBuffer->Map(0, nullptr, &renderTargetData);
	assert(SUCCEEDED(hr) && "Failed to map Readback Buffer!");

	std::string filename = "output.ppm";
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	assert(ofs && "Failed to open output file!");

	ofs << "P3\n" << renderTargetDesc.Width << " " << renderTargetDesc.Height << "\n255\n";

	const int totalPixels = renderTargetDesc.Width * renderTargetDesc.Height;
	int processedPixels = 0;
	const int updateInterval = totalPixels / 200;

	for (UINT row = 0; row < renderTargetDesc.Height; row++)
	{
		UINT rowPitch = renderTargetFootprint.Footprint.RowPitch;
		uint8_t* rowData = reinterpret_cast<uint8_t*>(renderTargetData) + row * rowPitch;

		for (UINT col = 0; col < renderTargetDesc.Width; col++)
		{
			processedPixels++;
			if (processedPixels % updateInterval == 0)
			{
				int progress = (processedPixels * 200) / totalPixels;
				std::cout << "Progress: " << progress << "%" << std::endl;
			}

			uint8_t* pixelData = rowData + col * 4; // 4 bytes per pixel (RGBA)
			uint8_t r = pixelData[0];
			uint8_t g = pixelData[1];
			uint8_t b = pixelData[2];
			ofs << static_cast<int>(r) << " "
				<< static_cast<int>(g) << " "
				<< static_cast<int>(b) << " ";
		}

		ofs << "\n";
	}

	ofs.close();
	readbackBuffer->Unmap(0, nullptr);
}
