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
	// �ϴ� ������ Ų��.
	g_Device->SetRenderState(D3DRS_LIGHTING, true);

	// ���� ���۸� �����Ѵ�.
	g_Device->CreateVertexBuffer(
		8 * sizeof(d3d::TextureVertex),
		D3DUSAGE_WRITEONLY,
		d3d::TextureVertex::FVF,
		D3DPOOL_MANAGED,
		&g_Quad,
		0);

	// �Ƕ�̵� �����ͷ� ���� ���۸� ä���.
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

	// �ؽ�ó �����͸� �ε��Ѵ�.
	D3DXCreateTextureFromFile(g_Device, L"todd.png", &g_ToddFace);

	// �ؽ�ó�� Ȱ��ȭ�Ѵ�.
	g_Device->SetTexture(0, g_ToddFace);

	// �ؽ�ó ���͸��� �����Ѵ�.
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// ���� ���� Ȯ�� ����
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// ���� ���� ��� ����
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);			// �Ӹ�

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
	d3d::Release<IDirect3DVertexBuffer9*>(g_Quad);
}

bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ
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