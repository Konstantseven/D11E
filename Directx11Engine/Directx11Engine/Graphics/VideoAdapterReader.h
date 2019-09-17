#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#include <wrl/client.h>
#include <vector>
#include "VideoAdapterData.h"

class VideoAdapterReader
{
public:
	static std::vector<VideoAdapterData> GetVideoAdapters();
private:
	static std::vector<VideoAdapterData> videoAdapters;
};
