#include "D3DUtil.h"
#include "Vertex.h"

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);
// 거울 생성
void RenderMirror();
// 씬 그리기
void RenderScene();

/* ========= 전역 변수 ========= */
IDirect3DDevice9*				g_Device = nullptr;				// 다이렉트3D 디바이스
IDirect3DVertexBuffer9*			g_VertexBuffer = nullptr;		// 배경을 그리기 위한 정점 버퍼

// 주전자와 재질
ID3DXMesh*						g_Teapot = 0;
D3DXVECTOR3						g_TeapotPosition(0.0f, 3.0f, -7.5f);
D3DMATERIAL9					g_TeapotMtrl = d3d::YELLOW_MTRL;

// 거울과 텍스처
IDirect3DVertexBuffer9*			g_Mirror = nullptr;
IDirect3DTexture9*				g_MirrorTexture = nullptr;

// 보조 텍스처와 머터리얼
IDirect3DTexture9*				g_FloorTex = 0;
IDirect3DTexture9*				g_WallTex = 0;
IDirect3DTexture9*				g_MirrorTex = 0;
D3DMATERIAL9					g_FloorMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9					g_WallMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9					g_MirrorMtrl = d3d::WHITE_MTRL;

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
	// 직반사광을 20퍼 정도로 줄인다.
	g_WallMtrl.Specular = d3d::WHITE * 0.2f;

	// 주전자를 생성한다.
	D3DXCreateTeapot(g_Device, &g_Teapot, 0);

	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/
	//
	g_Device->CreateVertexBuffer(
		24 * sizeof(Vertex),
		0, // usage
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&g_VertexBuffer,
		0);

	Vertex* v = 0;
	g_VertexBuffer->Lock(0, 0, (void**)&v, 0);

	// 계단부분
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// 벽 부분
	v[6] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// 거울 부분
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	g_VertexBuffer->Unlock();

	// 텍스처 설정
	D3DXCreateTextureFromFile(g_Device, L"checker.jpg", &g_FloorTex);
	D3DXCreateTextureFromFile(g_Device, L"brick0.jpg", &g_WallTex);
	D3DXCreateTextureFromFile(g_Device, L"ice.bmp", &g_MirrorTex);

	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	
	// 조명 설정
	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	g_Device->SetLight(0, &light);
	g_Device->LightEnable(0, true);

	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 카메라 설정
	D3DXVECTOR3    pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	g_Device->SetTransform(D3DTS_VIEW, &V);

	// 투영 행렬 설정
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4.0f, // 45 - degree
		(float)WIN_WIDTH / (float)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	{
		d3d::Release<IDirect3DVertexBuffer9*>(g_VertexBuffer);
		d3d::Release<IDirect3DTexture9*>(g_FloorTex);
		d3d::Release<IDirect3DTexture9*>(g_WallTex);
		d3d::Release<IDirect3DTexture9*>(g_MirrorTex);
		d3d::Release<ID3DXMesh*>(g_Teapot);
	}
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		static float radius = 20.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			g_TeapotPosition.x -= 3.0f * fDeltaTime;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			g_TeapotPosition.x += 3.0f * fDeltaTime;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			radius -= 2.0f * fDeltaTime;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			radius += 2.0f * fDeltaTime;

		static float angle = (3.0f * D3DX_PI) / 2.0f;

		if (::GetAsyncKeyState('A') & 0x8000f)
			angle -= 0.5f * fDeltaTime;

		if (::GetAsyncKeyState('S') & 0x8000f)
			angle += 0.5f * fDeltaTime;

		D3DXVECTOR3 position(cosf(angle) * radius, 3.0f, sinf(angle) * radius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		g_Device->SetTransform(D3DTS_VIEW, &V);

		/* 렌더 */
		D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크

		// 스텐실 버퍼를 사용하기 위해서 D3DCLEAR_STENCIL 선언한다.
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, bgColour, 1.0f, 0);

		g_Device->BeginScene(); 
		{
			RenderScene();
			RenderMirror();
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

void RenderMirror()
{
	g_Device->SetRenderState(D3DRS_STENCILENABLE, true);
	g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			// 스텐실 테스트 항상 성공(D3DCMP_ALWAYS)
	g_Device->SetRenderState(D3DRS_STENCILREF, 0x1);					// 스텐실 참조 값은 0x1
	g_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);			// 
	g_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);		// 
	g_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// 깊이 테스트가 실패한다면 픽셀이 가려졌다는 의미(D3DRS_STENCILZFAIL)
	g_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// 스텐실 테스트가 실패한 경우 D3DSTENCILOP_KEEP로 지정하여 테스트가 실패하는 일은 없으므로 불필요하니 이렇게 설정
	g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// 깊이 테스트와 스텐실 테스트가 모두 성공한 경우(D3DRS_STENCILPASS) D3DSTENCILOP_REPLACE를 지정하여 스텐실 버퍼 항목을
																		// 스텐실 참조 값인 0x1로 대체한다.

	// 깊이 버퍼와 후면 버퍼로의 쓰기를 막는다.
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 스텐실 버퍼에 거울을 그린다.
	g_Device->SetStreamSource(0, g_VertexBuffer, 0, sizeof(Vertex));
	g_Device->SetFVF(FVF_VERTEX);
	g_Device->SetMaterial(&g_MirrorMtrl);
	g_Device->SetTexture(0, g_MirrorTexture);
	XMMATRIX I;
	D3DXMatrixIdentity(&I);
	g_Device->SetTransform(D3DTS_WORLD, &I);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	// 깊이 버퍼 쓰기를 활성화한다.
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// 단지 주전자가 거울을 통과하는 경우만 그리도록
	g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// 반사의 위치
	XMMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f);
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		g_TeapotPosition.x,
		g_TeapotPosition.y,
		g_TeapotPosition.z);

	W = T * R;

	// 깊이 버퍼를 지우고, 거울에 비친 주전자를 블렌딩한다.
	g_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// 마지막으로 거울에 비친 주전자를 그린다.
	g_Device->SetTransform(D3DTS_WORLD, &W);
	g_Device->SetMaterial(&g_TeapotMtrl);
	g_Device->SetTexture(0, 0);

	g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	g_Teapot->DrawSubset(0);

	// 렌더 상태를 복구한다.
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_Device->SetRenderState(D3DRS_STENCILENABLE, false);
	g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void RenderScene()
{
	// 주전자 그리기
	g_Device->SetMaterial(&g_TeapotMtrl);
	g_Device->SetTexture(0, 0);
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		g_TeapotPosition.x,
		g_TeapotPosition.y,
		g_TeapotPosition.z);

	g_Device->SetTransform(D3DTS_WORLD, &W);
	g_Teapot->DrawSubset(0);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	g_Device->SetTransform(D3DTS_WORLD, &I);

	g_Device->SetStreamSource(0, g_VertexBuffer, 0, sizeof(Vertex));
	g_Device->SetFVF(FVF_VERTEX);

	// 계단 그리기
	g_Device->SetMaterial(&g_FloorMtrl);
	g_Device->SetTexture(0, g_FloorTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// 벽 그리기
	g_Device->SetMaterial(&g_WallMtrl);
	g_Device->SetTexture(0, g_WallTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// 거울 그리기
	g_Device->SetMaterial(&g_MirrorMtrl);
	g_Device->SetTexture(0, g_MirrorTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}
