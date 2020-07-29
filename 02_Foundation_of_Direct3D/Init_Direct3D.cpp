#include <iostream>

#include <Windows.h>

#include "D3DUtility.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR strCmdLine,
	int nCmdLines)
{
	if (!d3d::InitD3D(hInstance, WIN_WIDTH, WIN_HEIGHT, true, D3DDEVTYPE_HAL, &g_Device))
	{
		MessageBox(nullptr, L"InitD3D() - FAILED", 0, MB_OK);
		return 0;
	}

	if(!Setup())
	{
		MessageBox(nullptr, L"Setup() - FAILED", 0, MB_OK);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	g_Device->Release();

	return 0;
}

bool Setup()
{
	return true;
}

void Cleanup()
{
}

bool Display(float fDeltaTime)
{
	D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크

	if (g_Device)
	{
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);
		g_Device->Present(0, 0, 0, 0);
	}
	return true;
}
