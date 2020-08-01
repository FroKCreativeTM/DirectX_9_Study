#include "D3DUtil.h"

typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9* g_Device = nullptr;				// ���̷�Ʈ3D ����̽�
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;		// ���� ����
IDirect3DIndexBuffer9* g_pIndexBuffer = nullptr;		// �ε��� ����

XMMATRIX g_matWorld;
IDirect3DVertexBuffer9* g_Triangle = nullptr;

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
	// ���� ���� ����
	g_Device->CreateVertexBuffer(
		3 * sizeof(d3d::ColorVertex), 
		D3DUSAGE_WRITEONLY, 
		d3d::ColorVertex::FVF, 
		D3DPOOL_MANAGED, 
		&g_Triangle, 
		nullptr);

	// �ﰢ�� �����ͷ� ���۸� ä���.
	d3d::ColorVertex* tColorVertex;
	g_Triangle->Lock(0, 0, (void**)&tColorVertex, 0);

	tColorVertex[0] = d3d::ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	tColorVertex[1] = d3d::ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	tColorVertex[2] = d3d::ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

	g_Triangle->Unlock();

	// ���� ��Ʈ������ �����Ѵ�.
	XMMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj, 
		D3DX_PI * 0.5f, 
		(FLOAT)WIN_WIDTH / (FLOAT)WIN_HEIGHT, 
		1.0f, 
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// ���� ���¸� �����Ѵ�. (�������� ���� �����̴�.)
	g_Device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(g_Triangle);
}

// ���Ű� �������� �� ���� ������ ���
bool Display(float fDeltaTime)
{
	if (g_Device)
	{
		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ

		g_Device->BeginScene();
		{
			g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

			g_Device->SetFVF(d3d::ColorVertex::FVF);
			g_Device->SetStreamSource(0, g_Triangle, 0, sizeof(d3d::ColorVertex));

			// �÷� ���̵����� ȭ�� ���ʿ� �ﰢ���� �׸���.
			D3DXMatrixTranslation(&g_matWorld, -1.25, 0.0f, 0.0f);
			g_Device->SetTransform(D3DTS_WORLD, &g_matWorld);

			g_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

			// ���� ���̵����� ȭ�� �����ʿ� �ﰢ���� �׸���.
			D3DXMatrixTranslation(&g_matWorld, 1.25f, 0.0f, 0.0f);
			g_Device->SetTransform(D3DTS_WORLD, &g_matWorld);

			g_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			g_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		}
		g_Device->EndScene();
		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

