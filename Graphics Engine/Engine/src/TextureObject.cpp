//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "TextureObject.h"
#include "Engine.h"
#include "StringThis.h"

#include <Windows.h>
#include <string>
#include <d3d11.h>
#include "DirectXTex.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device *pHackDevice;
extern ID3D11DeviceContext *pHackDeviceContext;

TextureObject::TextureObject()
	:
	name(TextureObject::Name::Uninitialized),
	TexResourceSlot(0),
	SamplerSlot(0),
	poTextureRV(nullptr),
	poSampler(nullptr)
{

}

TextureObject::~TextureObject()
{
	SafeRelease(poTextureRV);
	SafeRelease(poSampler);
}

TextureObject::TextureObject(TextureObject::Name _name,
							 LPCWSTR filepath,
							 D3D11_FILTER filter)
	:
	name(_name),
	TexResourceSlot(0),
	SamplerSlot(0),
	poTextureRV(nullptr),
	poSampler(nullptr)
{
	DirectX::ScratchImage testTexture;
	HRESULT hr = LoadFromTGAFile(filepath, nullptr, testTexture);
	assert(SUCCEEDED(hr));

	CreateShaderResourceView(pHackDevice, testTexture.GetImage(0, 0, 0), testTexture.GetImageCount(), testTexture.GetMetadata(), &poTextureRV);

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = filter;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pHackDevice->CreateSamplerState(&sampDesc, &poSampler);
	assert(SUCCEEDED(hr));
}

void TextureObject::ActivateTexture()
{
	pHackDeviceContext->PSSetShaderResources(TexResourceSlot, 1, &poTextureRV);
	pHackDeviceContext->PSSetSamplers(SamplerSlot, 1, &poSampler);
}

void TextureObject::SetName(TextureObject::Name _name)
{
	this->name = _name;
}

char *TextureObject::GetName()
{
	// todo - Hack understand why is this needed for print and fix...
	static char pTmp[128];
	strcpy_s(pTmp, 128, StringMe(this->name));
	return pTmp;
}




// --- End of File ---