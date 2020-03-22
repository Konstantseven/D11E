#pragma once

#include <d3d11.h>
#include "../Helpers/ErrorLogger.h"

class VideoAdapterData 
{
public:
	explicit VideoAdapterData(IDXGIAdapter* pAdapter);

	IDXGIAdapter* pAdapter;
	DXGI_ADAPTER_DESC dxgiDescription;
};
