#pragma once

#include <iostream>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

/* ======================== 전역 변수 ======================== */
const LPCWSTR g_AppName = L"Color of Direct3D";
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

namespace d3d
{
	/* ======================== 전역 변수 ======================== */
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	/* ======================== 버텍스 구조체 ======================== */
	struct ColorVertex
	{
		ColorVertex() {}
		ColorVertex(float x, float y, float z, D3DCOLOR color)
		{
			_x = x;
			_y = y;
			_z = z;

			_color = color;
		}

		float _x, _y, _z;
		D3DCOLOR _color;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;	// 정점 변환 이전 좌표
	};


	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMsgLoop(bool (*ptr_display)(float deltaTime));
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	template<typename T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}

	template<typename T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
}




