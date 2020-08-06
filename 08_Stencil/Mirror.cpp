#include "D3DUtil.h"
#include "Vertex.h"

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);
// �ſ� ����
void RenderMirror();
// �� �׸���
void RenderScene();

/* ========= ���� ���� ========= */
IDirect3DDevice9*				g_Device = nullptr;				// ���̷�Ʈ3D ����̽�
IDirect3DVertexBuffer9*			g_VertexBuffer = nullptr;		// ����� �׸��� ���� ���� ����

// �����ڿ� ����
ID3DXMesh*						g_Teapot = 0;
D3DXVECTOR3						g_TeapotPosition(0.0f, 3.0f, -7.5f);
D3DMATERIAL9					g_TeapotMtrl = d3d::YELLOW_MTRL;

// �ſ�� �ؽ�ó
IDirect3DVertexBuffer9*			g_Mirror = nullptr;
IDirect3DTexture9*				g_MirrorTexture = nullptr;

// ���� �ؽ�ó�� ���͸���
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
	// ���ݻ籤�� 20�� ������ ���δ�.
	g_WallMtrl.Specular = d3d::WHITE * 0.2f;

	// �����ڸ� �����Ѵ�.
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

	// ��ܺκ�
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// �� �κ�
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

	// �ſ� �κ�
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	g_VertexBuffer->Unlock();

	// �ؽ�ó ����
	D3DXCreateTextureFromFile(g_Device, L"checker.jpg", &g_FloorTex);
	D3DXCreateTextureFromFile(g_Device, L"brick0.jpg", &g_WallTex);
	D3DXCreateTextureFromFile(g_Device, L"ice.bmp", &g_MirrorTex);

	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	
	// ���� ����
	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	g_Device->SetLight(0, &light);
	g_Device->LightEnable(0, true);

	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// ī�޶� ����
	D3DXVECTOR3    pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	g_Device->SetTransform(D3DTS_VIEW, &V);

	// ���� ��� ����
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

		/* ���� */
		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ

		// ���ٽ� ���۸� ����ϱ� ���ؼ� D3DCLEAR_STENCIL �����Ѵ�.
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
	g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);			// ���ٽ� �׽�Ʈ �׻� ����(D3DCMP_ALWAYS)
	g_Device->SetRenderState(D3DRS_STENCILREF, 0x1);					// ���ٽ� ���� ���� 0x1
	g_Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);			// 
	g_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);		// 
	g_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// ���� �׽�Ʈ�� �����Ѵٸ� �ȼ��� �������ٴ� �ǹ�(D3DRS_STENCILZFAIL)
	g_Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// ���ٽ� �׽�Ʈ�� ������ ��� D3DSTENCILOP_KEEP�� �����Ͽ� �׽�Ʈ�� �����ϴ� ���� �����Ƿ� ���ʿ��ϴ� �̷��� ����
	g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// ���� �׽�Ʈ�� ���ٽ� �׽�Ʈ�� ��� ������ ���(D3DRS_STENCILPASS) D3DSTENCILOP_REPLACE�� �����Ͽ� ���ٽ� ���� �׸���
																		// ���ٽ� ���� ���� 0x1�� ��ü�Ѵ�.

	// ���� ���ۿ� �ĸ� ���۷��� ���⸦ ���´�.
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���ٽ� ���ۿ� �ſ��� �׸���.
	g_Device->SetStreamSource(0, g_VertexBuffer, 0, sizeof(Vertex));
	g_Device->SetFVF(FVF_VERTEX);
	g_Device->SetMaterial(&g_MirrorMtrl);
	g_Device->SetTexture(0, g_MirrorTexture);
	XMMATRIX I;
	D3DXMatrixIdentity(&I);
	g_Device->SetTransform(D3DTS_WORLD, &I);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	// ���� ���� ���⸦ Ȱ��ȭ�Ѵ�.
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// ���� �����ڰ� �ſ��� ����ϴ� ��츸 �׸�����
	g_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	g_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// �ݻ��� ��ġ
	XMMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f);
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		g_TeapotPosition.x,
		g_TeapotPosition.y,
		g_TeapotPosition.z);

	W = T * R;

	// ���� ���۸� �����, �ſ￡ ��ģ �����ڸ� �����Ѵ�.
	g_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// ���������� �ſ￡ ��ģ �����ڸ� �׸���.
	g_Device->SetTransform(D3DTS_WORLD, &W);
	g_Device->SetMaterial(&g_TeapotMtrl);
	g_Device->SetTexture(0, 0);

	g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	g_Teapot->DrawSubset(0);

	// ���� ���¸� �����Ѵ�.
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_Device->SetRenderState(D3DRS_STENCILENABLE, false);
	g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void RenderScene()
{
	// ������ �׸���
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

	// ��� �׸���
	g_Device->SetMaterial(&g_FloorMtrl);
	g_Device->SetTexture(0, g_FloorTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// �� �׸���
	g_Device->SetMaterial(&g_WallMtrl);
	g_Device->SetTexture(0, g_WallTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// �ſ� �׸���
	g_Device->SetMaterial(&g_MirrorMtrl);
	g_Device->SetTexture(0, g_MirrorTex);
	g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}
