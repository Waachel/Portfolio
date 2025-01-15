//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Engine.h"
#include "Mesh.h"
#include "StringThis.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

using namespace Azul;

Mesh::Mesh( unsigned int _numVerts, unsigned int _numIndices)
	:
	numVerts{_numVerts},
	numIndices{_numIndices},
	poVertexBuffer_pos{nullptr},
	poVertexBuffer_color{nullptr},
	poVertexBuffer_norm{nullptr},
	poVertexBuffer_texCoord{nullptr},
	poIndexBuffer{nullptr},
	poConstantBuff_Projection{nullptr},
	poConstantBuff_World{nullptr},
	poConstantBuff_View{nullptr},
	poConstantBuff_lightColor{nullptr},
	poConstantBuff_lightPos{nullptr}
{

}

Mesh::~Mesh()
{
	SafeRelease(poVertexBuffer_pos);
	SafeRelease(poVertexBuffer_color);
	SafeRelease(poVertexBuffer_norm);
	SafeRelease(poVertexBuffer_texCoord);
	SafeRelease(poIndexBuffer);
	SafeRelease(poConstantBuff_Projection);
	SafeRelease(poConstantBuff_World);
	SafeRelease(poConstantBuff_View);
	SafeRelease(poConstantBuff_lightColor);
	SafeRelease(poConstantBuff_lightPos);
}


void Mesh::Wash()
{

}

bool Mesh::Compare(DLink *pTarget)
{
	// This is used in ManBase.Find() 
	assert(pTarget != nullptr);

	Mesh *pDataB = (Mesh *) pTarget;

	bool status = false;

	if(this->name == pDataB->name)
	{
		status = true;
	}

	return status;
}

void Mesh::SetName(Mesh::Name _name)
{
	this->name = _name;
}

char *Mesh::GetName()
{
	// todo - Hack understand why is this needed for print and fix...
	static char pTmp[128];
	strcpy_s(pTmp, 128, StringMe(this->name));
	return pTmp;
}


void Mesh::RenderIndexBuffer()
{
	// ---------------------------------------------
	//    Set (point to ) Index buffer 
	//    Render configuration: Triangles
	// ---------------------------------------------
	pHackDeviceContext->IASetIndexBuffer(poIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pHackDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------------------------
	// RENDER - using index data
	//--------------------------------------------------------
	pHackDeviceContext->DrawIndexed(numIndices, 0, 0);
}

void Mesh::TransferConstantBuffer(Camera *pCam, Mat4 *pWorld)
{
	assert(pCam);
	assert(pWorld);

	pHackDeviceContext->UpdateSubresource(poConstantBuff_View, 0, nullptr, &pCam->getViewMatrix(), 0, 0);
	pHackDeviceContext->UpdateSubresource(poConstantBuff_Projection, 0, nullptr, &pCam->getProjMatrix(), 0, 0);
	pHackDeviceContext->UpdateSubresource(poConstantBuff_World, 0, nullptr, pWorld, 0, 0);

}


void Mesh::ActivateMesh()
{

	// ---------------------------------------------
	//    Set (point to ) vertex: position 
	//    Set (point to ) vertex: color
	// ---------------------------------------------
	const UINT offset = 0;

	if(poVertexBuffer_pos)
	{
		const UINT vertexStride_pos = sizeof(VertexPos);
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::Position, 1, &poVertexBuffer_pos, &vertexStride_pos, &offset);
	}
	if(poVertexBuffer_color)
	{
		const UINT vertexStride_color = sizeof(VertexColor);
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::Color, 1, &poVertexBuffer_color, &vertexStride_color, &offset);
	}

	if(poVertexBuffer_norm)
	{
		const UINT vertexStride_norm = sizeof(VertexNorm);
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::Norm, 1, &poVertexBuffer_norm, &vertexStride_norm, &offset);
	}

	if(poVertexBuffer_texCoord)
	{
		const UINT vertexStride_texCoord = sizeof(VertexTexCoord);
		pHackDeviceContext->IASetVertexBuffers((uint32_t)VertexSlot::TexCoord, 1, &poVertexBuffer_texCoord, &vertexStride_texCoord, &offset);
	}

	//--------------------------------------------------
	// Enable shader
	//    Set (point to the shader to use)
	//    Set (point to the constant buffers to use)
	//--------------------------------------------------
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Projection, 1, &poConstantBuff_Projection);
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::View, 1, &poConstantBuff_View);
	pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1, &poConstantBuff_World);

	if(poConstantBuff_lightColor)
	{
		pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Color, 1, 
												 &poConstantBuff_lightColor);
	}

	if(poConstantBuff_lightPos)
	{
		pHackDeviceContext->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::LightPos, 1,
												 &poConstantBuff_lightPos);
	}


}

ID3D11Buffer *Mesh::CreateVertexBuffer(unsigned int NumBytes, void *pData)
{
	assert(pData);

	// Create an initialize the vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc{0};
	D3D11_SUBRESOURCE_DATA resourceData{0};

	// type of buffer
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = NumBytes;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	resourceData.pSysMem = pData;
	ID3D11Buffer *pVertexBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&vertexBufferDesc, &resourceData, &pVertexBuffer);
	assert(SUCCEEDED(hr));

	return pVertexBuffer;
}

ID3D11Buffer *Mesh::CreateIndexBuffer(unsigned int NumBytes, void *pData)
{
	assert(pData);

	// Create and initialize the index buffer.
	D3D11_BUFFER_DESC indexBufferDesc{0};
	D3D11_SUBRESOURCE_DATA resourceData{0};

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = NumBytes;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	resourceData.pSysMem = pData;

	ID3D11Buffer *pIndexBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&indexBufferDesc, &resourceData, &pIndexBuffer);
	assert(SUCCEEDED(hr));

	return pIndexBuffer;
}

ID3D11Buffer *Mesh::CreateConstantBuffer(unsigned int NumBytes)
{

	D3D11_BUFFER_DESC constantBufferDesc{0};

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = NumBytes;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Buffer *pConstantBuffer;
	HRESULT hr;
	hr = pHackDevice->CreateBuffer(&constantBufferDesc, nullptr, &pConstantBuffer);
	assert(SUCCEEDED(hr));

	return pConstantBuffer;
}

// --- End of File ---
