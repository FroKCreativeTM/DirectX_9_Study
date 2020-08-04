#include "D3DUtil.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;				// 다이렉트3D 디바이스
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;		// 정점 버퍼
IDirect3DIndexBuffer9* g_pIndexBuffer = nullptr;		// 인덱스 버퍼

// 피라미드를 그리기 위한 정점 버퍼
IDirect3DVertexBuffer9* g_Quad = nullptr;
IDirect3DTexture9* g_ToddFace = nullptr;
XMMATRIX g_matWorld;

INT WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR strCmdLine,
	INT nCmdLine)
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
	// 일단 조명을 킨다.
	g_Device->SetRenderState(D3DRS_LIGHTING, true);

	// 정점 버퍼를 생성한다.
	g_Device->CreateVertexBuffer(
		8 * sizeof(d3d::TextureVertex),
		D3DUSAGE_WRITEONLY,
		d3d::TextureVertex::FVF,
		D3DPOOL_MANAGED,
		&g_Quad,
		0);

	// 피라미드 데이터로 정점 버퍼를 채운다.
	d3d::TextureVertex* v;
	g_Quad->Lock(0, 0, (void**)&v, 0);
	{
		v[0] = d3d::TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = d3d::TextureVertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = d3d::TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		v[3] = d3d::TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[4] = d3d::TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[5] = d3d::TextureVertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	}
	g_Quad->Unlock();

	// 텍스처 데이터를 로드한다.
	D3DXCreateTextureFromFile(g_Device, L"todd.png", &g_ToddFace);

	// 텍스처를 활성화한다.
	g_Device->SetTexture(0, g_ToddFace);

	// 텍스처 필터링을 지정한다.
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// 선형 필터 확대 필터
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// 선형 필터 축소 필터
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);			// 밉맵

	// 투영 매트릭스를 지정한다.
	XMMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI * 0.5f,
		(FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(g_Quad);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Device->SetStreamSource(0, g_Quad, 0, sizeof(d3d::TextureVertex));
			g_Device->SetFVF(d3d::TextureVertex::FVF);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}