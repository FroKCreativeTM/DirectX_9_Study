#include "D3DUtil.h"
#include "Vertex.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9* g_Device = nullptr;				// 다이렉트3D 디바이스

// 주전자와 재질
ID3DXMesh*						g_Teapot = nullptr;
D3DMATERIAL9					g_TeapotMtrl;

// 나무 상자와 텍스처
IDirect3DVertexBuffer9*			g_BGQuad = nullptr;
IDirect3DTexture9*				g_ToddFace = nullptr;
D3DMATERIAL9					g_BGMtrl;

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
	// 주전자의 재질
	g_TeapotMtrl = d3d::RED_MTRL;
	g_TeapotMtrl.Diffuse.a = 0.5f;
	g_BGMtrl = d3d::WHITE_MTRL;

	// 주전자 생성
	D3DXCreateTeapot(g_Device, &g_Teapot, nullptr);

	// 정점 버퍼를 생성한다.
	g_Device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&g_BGQuad,
		0);

	// 사각형 데이터로 정점 버퍼를 채운다.
	Vertex* v;
	g_BGQuad->Lock(0, 0, (void**)&v, 0);
	{
		v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	}
	g_BGQuad->Unlock();

	// 조명 설정
	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.2f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	g_Device->SetLight(0, &dir);
	g_Device->LightEnable(0, true);

	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 텍스처 데이터를 로드한다.
	D3DXCreateTextureFromFile(g_Device, L"todd.png", &g_ToddFace);

	// 텍스처를 활성화한다.
	g_Device->SetTexture(0, g_ToddFace);

	// 텍스처 필터링을 지정한다.
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// 선형 필터 확대 필터
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// 선형 필터 축소 필터
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);			// 밉맵

	// 재질의 알파값을 사용한다.
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// 블렌딩 인수를 지정하여 알파 성분이 투명도를 결정하게 한다.
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 뷰 행렬
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	g_Device->SetTransform(D3DTS_VIEW, &V);

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
	d3d::Release<ID3DXMesh*>(g_Teapot);
	d3d::Release<IDirect3DVertexBuffer9*>(g_BGQuad);
	d3d::Release<IDirect3DTexture9*>(g_ToddFace);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		/* 입력 및 갱신 */
		// 키보드 입력을 통해서 알파를 증가 및 감소시킨다.
		if (::GetAsyncKeyState('A') & 0x8000f)
			g_TeapotMtrl.Diffuse.a += 0.01f;

		if (::GetAsyncKeyState('S') & 0x8000f)
			g_TeapotMtrl.Diffuse.a -= 0.01f;

		// 알파가 [0, 1] 범위 내에 있도록 한다.
		if (g_TeapotMtrl.Diffuse.a > 1.0f)
			g_TeapotMtrl.Diffuse.a = 1.0f;
		if (g_TeapotMtrl.Diffuse.a < 0.0f)
			g_TeapotMtrl.Diffuse.a = 0.0f;

		/* 렌더 */
		D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			// 배경을 그린다.
			XMMATRIX world;
			D3DXMatrixIdentity(&world);
			g_Device->SetTransform(D3DTS_WORLD, &world);
			g_Device->SetFVF(FVF_VERTEX);
			g_Device->SetStreamSource(0, g_BGQuad, 0, sizeof(Vertex));
			g_Device->SetMaterial(&g_BGMtrl);
			g_Device->SetTexture(0, g_ToddFace);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			// 주전자를 그린다.
			g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			{
				D3DXMatrixScaling(&world, 1.5f, 1.5f, 1.5f);
				g_Device->SetTransform(D3DTS_WORLD, &world);
				g_Device->SetMaterial(&g_TeapotMtrl);
				g_Device->SetTexture(0, nullptr);
				g_Teapot->DrawSubset(0);
			}
			g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}