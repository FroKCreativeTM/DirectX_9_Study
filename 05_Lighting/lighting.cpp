#include "D3DUtil.h"


/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9* g_Device = nullptr;				// ���̷�Ʈ3D ����̽�
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;		// ���� ����
IDirect3DIndexBuffer9* g_pIndexBuffer = nullptr;		// �ε��� ����

// �Ƕ�̵带 �׸��� ���� ���� ����
IDirect3DVertexBuffer9* g_Pyramid;
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
	// �ϴ� ������ Ų��.
	g_Device->SetRenderState(D3DRS_LIGHTING, true);

	// ���� ���۸� �����Ѵ�.
	g_Device->CreateVertexBuffer(
		12 * sizeof(d3d::NormalVertex),
		D3DUSAGE_WRITEONLY,
		d3d::NormalVertex::FVF,
		D3DPOOL_MANAGED,
		&g_Pyramid,
		0);
	
	// �Ƕ�̵� �����ͷ� ���� ���۸� ä���.
	d3d::NormalVertex* v;
	g_Pyramid->Lock(0, 0, (void**)&v, 0);
	{
		// ����
		v[0] = d3d::NormalVertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
		v[1] = d3d::NormalVertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
		v[2] = d3d::NormalVertex(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

		// ���� ����
		v[3] = d3d::NormalVertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
		v[4] = d3d::NormalVertex(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
		v[5] = d3d::NormalVertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

		// ������ ����
		v[6] = d3d::NormalVertex(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
		v[7] = d3d::NormalVertex(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
		v[8] = d3d::NormalVertex(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

		// �ĸ�
		v[9] = d3d::NormalVertex(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		v[10] = d3d::NormalVertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
		v[11] = d3d::NormalVertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	}
	g_Pyramid->Unlock();

	// �Ƕ�̵��� ������ �����Ѵ�.
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = d3d::WHITE;
	mtrl.Diffuse = d3d::WHITE;
	mtrl.Specular = d3d::WHITE;
	mtrl.Emissive = d3d::BLACK;
	mtrl.Power = 5.0f;
	g_Device->SetMaterial(&mtrl);

	// ���⼺ ���� ����
	D3DLIGHT9 dir;
	ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.3f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// ���� �ѱ�
	g_Device->SetLight(0, &dir);
	g_Device->LightEnable(0, true);

	// ���� ����ȭ �� ���ݻ籤 Ȱ��ȭ
	g_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	// �� ����� �����ϴ� �ڵ�
	D3DXVECTOR3 position(0.0f, 1.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
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
	d3d::Release<IDirect3DVertexBuffer9*>(g_Pyramid);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		D3DXMATRIX yRot;

		static float y = 0.0f;

		D3DXMatrixRotationY(&yRot, y);
		y += fDeltaTime;

		if (y >= 6.28f)
			y = 0.0f;

		g_Device->SetTransform(D3DTS_WORLD, &yRot);

		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Device->SetStreamSource(0, g_Pyramid, 0, sizeof(d3d::NormalVertex));
			g_Device->SetFVF(d3d::NormalVertex::FVF);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}