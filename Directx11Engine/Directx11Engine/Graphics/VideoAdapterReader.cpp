#include "VideoAdapterReader.h"

std::vector<VideoAdapterData> VideoAdapterReader::videoAdapters;

std::vector<VideoAdapterData> VideoAdapterReader::GetVideoAdapters() {
	if (videoAdapters.size() > 0) {
		return videoAdapters;
	}

	Microsoft::WRL::ComPtr<IDXGIFactory> idxgiFactory;
	HRESULT hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(idxgiFactory.GetAddressOf()));
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to create DXGIFactory for enumerating video adapters!");
		exit(-1);
	}

	IDXGIAdapter* idxgiAdapter;
	UINT index = 0;
	while (SUCCEEDED(idxgiFactory->EnumAdapters(index, &idxgiAdapter))) {
		videoAdapters.emplace_back(VideoAdapterData(idxgiAdapter));
		++index;
	}

	return videoAdapters;
}