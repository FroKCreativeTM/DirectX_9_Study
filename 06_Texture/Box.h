#pragma once

#include <d3dx9.h>
#include <string>

class Box
{
private : 
	IDirect3DVertexBuffer9*			m_pVertexBuffer;
	IDirect3DIndexBuffer9*			m_pIndexBuffer;
	IDirect3DDevice9*				m_pDevice;

public : 
	Box(IDirect3DDevice9* _device);
	~Box();

	bool draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex);
};

