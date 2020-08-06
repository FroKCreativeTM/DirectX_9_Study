#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

// 타입 재정의
typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ======================== 전역 변수 ======================== */
const LPCWSTR g_AppName = L"Stencil of Direct3D";
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

namespace d3d
{
	// 그림자를 위한 구조체들
	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();

		D3DXVECTOR3 _center;
		float       _radius;
	};

	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMsgLoop(bool (*ptr_display)(float deltaTime));
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 재질 설정을 위한 유틸리티 함수
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	// 간단한 방향성 광원 초기화
	D3DLIGHT9 InitDirectionalLight(XMVECTOR3* direction, D3DXCOLOR* color);
	// 점 광원 초기화
	D3DLIGHT9 InitPointLight(XMVECTOR3* position, D3DXCOLOR* color);
	// 스포트 광원 초기화
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

	const float CONST_INFINITY = std::numeric_limits<float>::infinity();
	const float EPSILON = 0.001f;

	//
	// Randomness
	//

	// Desc: Return random float in [lowBound, highBound] interval.
	float GetRandomFloat(float lowBound, float highBound);


	// Desc: Returns a random vector in the bounds specified by min and max.
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

	//
	// Conversion
	//
	DWORD FtoDw(float f);

	/* ======================== 전역 변수 ======================== */
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
}
