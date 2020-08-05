#include "D3DUtil.h"
#include "Box.h"
#include "TextureVertex.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;				// 다이렉트3D 디바이스

// 박스를 그리기 위한 객체와 텍스처
Box*				g_Box = nullptr;
IDirect3DTexture9*	g_ToddFace = nullptr;

INT WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR strCmdLine,
	int nCmdShow)
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
	g_Box = new Box(g_Device);

	// 방향성 광원 설정
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

	g_Device->SetLight(0, &light);
	g_Device->LightEnable(0, true);

	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 텍스처 생성
	D3DXCreateTextureFromFile(
		g_Device,
		L"todd.png",
		&g_ToddFace);

	// 텍스처 필터 생성(선형 필터링)
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 투영 행렬 설정
	XMMATRIX proj;
	D3DXMatrixPerspectiveLH(
		&proj,
		D3DX_PI * 0.5f,
		(FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	d3d::Delete<Box*>(g_Box);
	d3d::Release<IDirect3DTexture9*>(g_ToddFace);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 2.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * fDeltaTime;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * fDeltaTime;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * fDeltaTime;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * fDeltaTime;

		// 뷰 행렬
		XMVECTOR3 position(cosf(angle) * 3.0f, height, sinf(angle) * 3.0f);
		XMVECTOR3 target(0.0f, 0.0f, 0.0f);
		XMVECTOR3 up(0.0f, 1.0f, 0.0f);
		XMMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		g_Device->SetTransform(D3DTS_VIEW, &V);

		D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Device->SetMaterial(&d3d::WHITE_MTRL);
			g_Device->SetTexture(0, g_ToddFace);

			g_Box->draw(nullptr, nullptr, nullptr);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}