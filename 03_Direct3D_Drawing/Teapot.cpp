#include "D3DUtil.h"

using namespace std;

typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;				// 다이렉트3D 디바이스

/* ========= 물체 변수 ========= */
ID3DXMesh* g_Teapot = nullptr;								// 테스트용 메시

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR strCmdLine, INT nCmdLine)
{
	if (!d3d::InitD3D(hInstance, WIN_WIDTH, WIN_HEIGHT, true, D3DDEVTYPE_HAL, &g_Device))
	{
		MessageBox(nullptr, L"InitD3D() - FAILED", 0, MB_OK);
		return 0;
	}

	if (!Setup())
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
	D3DXCreateTeapot(g_Device, &g_Teapot, nullptr);

	// 카메라의 위치와 방향을 조정한다.
	XMVECTOR3 position(0.0f, 0.0f, -5.0f);
	XMVECTOR3 target(0.0f, 0.0f, 0.0f);
	XMVECTOR3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &position, &target, &up);
	g_Device->SetTransform(D3DTS_VIEW, &matView);

	// 투영 행렬을 지정한다.
	XMMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)WIN_WIDTH / (float)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// 렌더 상태를 지정한다. (현재는 와이어 프레임)
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(g_Teapot);
}

// 갱신과 렌더링의 두 가지 역할을 담당
bool Display(float fDeltaTime)
{
	D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크

	// D3DXCreateTeapot(g_Device, &g_Teapot, 0);

	if (g_Device)
	{
		// 입방체를 회전시킨다.
		XMMATRIX Rx, Ry;

		// x축으로 45도 회전시킨다.
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		// 각 프레임당 y 회전을 증가시킨다.
		static float fy = 0.0f;
		D3DXMatrixRotationY(&Ry, fy);
		fy += fDeltaTime;

		// 각도가 2 * PI에 이르면 초기화
		if (fy >= 6.28f)
			fy = 0.0f;

		// 회전을 결합한다.
		XMMATRIX p = Rx * Ry;

		g_Device->SetTransform(D3DTS_WORLD, &p);

		// 장면을 그린다.
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Teapot->DrawSubset(0);
		}
		g_Device->EndScene();

		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

