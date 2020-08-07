#include "D3DUtil.h"

using namespace std;

typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9* g_Device = nullptr;				// ���̷�Ʈ3D ����̽�

/* ========= ��ü ���� ========= */
ID3DXMesh* g_Teapot = nullptr;								// �׽�Ʈ�� �޽�

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR strCmdLine, INT nCmdLine)
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
	D3DXCreateTeapot(g_Device, &g_Teapot, nullptr);

	// ī�޶��� ��ġ�� ������ �����Ѵ�.
	XMVECTOR3 position(0.0f, 0.0f, -5.0f);
	XMVECTOR3 target(0.0f, 0.0f, 0.0f);
	XMVECTOR3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &position, &target, &up);
	g_Device->SetTransform(D3DTS_VIEW, &matView);

	// ���� ����� �����Ѵ�.
	XMMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)WIN_WIDTH / (float)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// ���� ���¸� �����Ѵ�. (����� ���̾� ������)
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(g_Teapot);
}

// ���Ű� �������� �� ���� ������ ���
bool Display(float fDeltaTime)
{
	D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ

	// D3DXCreateTeapot(g_Device, &g_Teapot, 0);

	if (g_Device)
	{
		// �Թ�ü�� ȸ����Ų��.
		XMMATRIX Rx, Ry;

		// x������ 45�� ȸ����Ų��.
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		// �� �����Ӵ� y ȸ���� ������Ų��.
		static float fy = 0.0f;
		D3DXMatrixRotationY(&Ry, fy);
		fy += fDeltaTime;

		// ������ 2 * PI�� �̸��� �ʱ�ȭ
		if (fy >= 6.28f)
			fy = 0.0f;

		// ȸ���� �����Ѵ�.
		XMMATRIX p = Rx * Ry;

		g_Device->SetTransform(D3DTS_WORLD, &p);

		// ����� �׸���.
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			g_Teapot->DrawSubset(0);
		}
		g_Device->EndScene();

		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

