#include "D3DUtil.h"
#include "TextureVertex.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;				// 다이렉트3D 디바이스
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;		// 정점 버퍼

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
	// 정점 버퍼를 생성한다.
	g_Device->CreateVertexBuffer(
		8 * sizeof(TextureVertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&g_Quad,
		0);

	// 피라미드 데이터로 정점 버퍼를 채운다.
	TextureVertex* v;
	g_Quad->Lock(0, 0, (void**)&v, 0);
	{
		v[0] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = TextureVertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		v[3] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[4] = TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[5] = TextureVertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
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

	// 조명 사용 안 함
	g_Device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(g_Quad);
	d3d::Release<IDirect3DTexture9*>(g_ToddFace);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		// wrap 어드레스 모드로 지정한다.
		if (GetAsyncKeyState('W') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}
		// border color 어드레스 모드로 지정한다.
		if (GetAsyncKeyState('B') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			g_Device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
		}
		// clamp 어드레스 모드로 지정한다.
		if (GetAsyncKeyState('C') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
		// mirror 어드레스 모드로 지정한다.
		if (GetAsyncKeyState('M') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}

		D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Device->SetStreamSource(0, g_Quad, 0, sizeof(TextureVertex));
			g_Device->SetFVF(FVF_VERTEX);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}