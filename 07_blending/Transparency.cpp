#include "D3DUtil.h"
#include "Vertex.h"

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9* g_Device = nullptr;				// ���̷�Ʈ3D ����̽�

// �����ڿ� ����
ID3DXMesh*						g_Teapot = nullptr;
D3DMATERIAL9					g_TeapotMtrl;

// ���� ���ڿ� �ؽ�ó
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
	// �������� ����
	g_TeapotMtrl = d3d::RED_MTRL;
	g_TeapotMtrl.Diffuse.a = 0.5f;
	g_BGMtrl = d3d::WHITE_MTRL;

	// ������ ����
	D3DXCreateTeapot(g_Device, &g_Teapot, nullptr);

	// ���� ���۸� �����Ѵ�.
	g_Device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&g_BGQuad,
		0);

	// �簢�� �����ͷ� ���� ���۸� ä���.
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

	// ���� ����
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

	// �ؽ�ó �����͸� �ε��Ѵ�.
	D3DXCreateTextureFromFile(g_Device, L"todd.png", &g_ToddFace);

	// �ؽ�ó�� Ȱ��ȭ�Ѵ�.
	g_Device->SetTexture(0, g_ToddFace);

	// �ؽ�ó ���͸��� �����Ѵ�.
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// ���� ���� Ȯ�� ����
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// ���� ���� ��� ����
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);			// �Ӹ�

	// ������ ���İ��� ����Ѵ�.
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// ���� �μ��� �����Ͽ� ���� ������ ������ �����ϰ� �Ѵ�.
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �� ���
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	g_Device->SetTransform(D3DTS_VIEW, &V);

	// ���� ��Ʈ������ �����Ѵ�.
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
		/* �Է� �� ���� */
		// Ű���� �Է��� ���ؼ� ���ĸ� ���� �� ���ҽ�Ų��.
		if (::GetAsyncKeyState('A') & 0x8000f)
			g_TeapotMtrl.Diffuse.a += 0.01f;

		if (::GetAsyncKeyState('S') & 0x8000f)
			g_TeapotMtrl.Diffuse.a -= 0.01f;

		// ���İ� [0, 1] ���� ���� �ֵ��� �Ѵ�.
		if (g_TeapotMtrl.Diffuse.a > 1.0f)
			g_TeapotMtrl.Diffuse.a = 1.0f;
		if (g_TeapotMtrl.Diffuse.a < 0.0f)
			g_TeapotMtrl.Diffuse.a = 0.0f;

		/* ���� */
		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			// ����� �׸���.
			XMMATRIX world;
			D3DXMatrixIdentity(&world);
			g_Device->SetTransform(D3DTS_WORLD, &world);
			g_Device->SetFVF(FVF_VERTEX);
			g_Device->SetStreamSource(0, g_BGQuad, 0, sizeof(Vertex));
			g_Device->SetMaterial(&g_BGMtrl);
			g_Device->SetTexture(0, g_ToddFace);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			// �����ڸ� �׸���.
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