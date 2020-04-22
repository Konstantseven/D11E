#include "VideoAdapterData.h"

VideoAdapterData::VideoAdapterData(IDXGIAdapter* pAdapter) {
	this->pAdapter = pAdapter;

	HRESULT hResult = pAdapter->GetDesc(&this->dxgiDescription);
	if (FAILED(hResult)) {
		helpers::error_logger::Log(hResult, "Failed to get description for the IDXGIAdapter!");
	}
}