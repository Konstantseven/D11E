#include "VideoAdapterData.h"

VideoAdapterData::VideoAdapterData(IDXGIAdapter* pAdapter) {
	this->pAdapter = pAdapter;

	HRESULT hResult = pAdapter->GetDesc(&this->dxgiDescription);
	if (FAILED(hResult)) {
		ErrorLogger::Log(hResult, "Failed to get description for the IDXGIAdapter!");
	}
}