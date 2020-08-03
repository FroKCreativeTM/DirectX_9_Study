#pragma once

#include <iostream>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

// Ÿ�� ������
typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ======================== ���� ���� ======================== */
const LPCWSTR g_AppName = L"Lighting of Direct3D";
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

namespace d3d
{
	/* ======================== ���ؽ� ����ü ======================== */
	struct NormalVertex
	{
		NormalVertex() {}
		NormalVertex(float x, float y, float z, 
			float nx, float ny, float nz)
		{
			_x = x;
			_y = y;
			_z = z;

			_nx = nx;
			_ny = ny;
			_nz = nz;
		}

		float _x, _y, _z;
		float _nx, _ny, _nz;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;	// ���� ��ȯ ���� ��ǥ
	};


	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMsgLoop(bool (*ptr_display)(float deltaTime));
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ���� ������ ���� ��ƿ��Ƽ �Լ�
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	// ������ ���⼺ ���� �ʱ�ȭ
	D3DLIGHT9 InitDirectionalLight(XMVECTOR3* direction, D3DXCOLOR* color);
	// �� ���� �ʱ�ȭ
	D3DLIGHT9 InitPointLight(XMVECTOR3* position, D3DXCOLOR* color);
	// ����Ʈ ���� �ʱ�ȭ
	D3DLIGHT9 InitSpotLight(XMVECTOR3* position, XMVECTOR3* direction, D3DXCOLOR* color);

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

	/* ======================== ���� ���� ======================== */
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 8.0f);
}
