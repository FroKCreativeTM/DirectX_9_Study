#pragma once

#include <iostream>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

/* ======================== 전역 변수 ======================== */
const LPCWSTR g_AppName = L"Init Direct3D";
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

namespace d3d
{
	/* ======================== 버텍스 구조체 ======================== */
	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}

		float _x, _y, _z;
		static const DWORD FVF = D3DFVF_XYZ;	// 정점 변환 이전 좌표
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


