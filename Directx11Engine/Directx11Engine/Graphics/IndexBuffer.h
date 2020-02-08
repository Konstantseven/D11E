#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer() {};
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddresOf() const;
	UINT BufferSize() const;
	HRESULT Initialize(ID3D11Device*, const DWORD*, const UINT);
private:
	IndexBuffer(const IndexBuffer&);
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize = 0;
};

