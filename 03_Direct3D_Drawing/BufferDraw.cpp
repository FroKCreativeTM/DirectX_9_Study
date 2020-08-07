#include "D3DUtil.h"

using namespace std;

typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9*				g_Device = nullptr;				// ���̷�Ʈ3D ����̽�
IDirect3DVertexBuffer9*			g_pVertexBuffer = nullptr;		// ���� ����
IDirect3DIndexBuffer9*			g_pIndexBuffer = nullptr;		// �ε��� ����
// ID3DXMesh* g_Teapot = nullptr;								// �׽�Ʈ�� �޽�

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
	// ������ �ε��� ���۸� �����Ѵ�.
	g_Device->CreateVertexBuffer(
		8 * sizeof(d3d::Vertex),							// ���ۿ� �Ҵ��� ����Ʈ ��
		D3DUSAGE_WRITEONLY,									// ���۰� �̿�� ����� �����ϴ� �ΰ��� Ư��
		d3d::Vertex::FVF,									// ���� ���ۿ� ������ ������ ������ ���� ����
		D3DPOOL_MANAGED,									// ���۰� ��ġ�� �޸� Ǯ
		&g_pVertexBuffer,									// ������� ���� ���۸� ���� ������
		0);													// �̿���� �ʴ� ���̴�.

	g_Device->CreateIndexBuffer(
		36 * sizeof(DWORD),									// ���ۿ� �Ҵ��� ����Ʈ ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 								// ���۰� �̿�� ����� �����ϴ� �ΰ��� Ư��
		D3DFMT_INDEX16, 									// ���� ���ۿ� ������ ������ ������ ���� ����
		D3DPOOL_MANAGED,									// ���۰� ��ġ�� �޸� Ǯ
		&g_pIndexBuffer, 									// ������� ���� ���۸� ���� ������
		0);													// �̿���� �ʴ� ���̴�.

	// ť�� ���� �����͸� �����ϰ� ���ۿ� �̸� ä���.
	d3d::Vertex* vertices;
	g_pVertexBuffer->Lock(
		0,						// ����� ������ ���� ��ġ�� ������
		0, 						// ��� ����Ʈ�� ��
		(void**)&vertices, 		// ��� �޸��� ������ ����Ű�� ������
		0);						// ����� �̷������ ����� �����Ѵ�.

	// ���� ť���� ������
	vertices[0] = d3d::Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = d3d::Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = d3d::Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = d3d::Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = d3d::Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = d3d::Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = d3d::Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = d3d::Vertex(1.0f, -1.0f, 1.0f);

	g_pVertexBuffer->Unlock();

	// �Թ�ü�� �ﰢ������ �����Ѵ�.
	WORD* indices = nullptr;
	g_pIndexBuffer->Lock(
		0,						// ����� ������ ���� ��ġ�� ������
		0, 						// ��� ����Ʈ�� ��
		(void**)&indices, 		// ��� �޸��� ������ ����Ű�� ������
		0);						// ����� �̷������ ����� �����Ѵ�.

	// ����
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// �ĸ�
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// ���� ����
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// ������ ����
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// ���
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// �ϴ�
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	g_pIndexBuffer->Unlock();

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
	d3d::Release<IDirect3DVertexBuffer9*>(g_pVertexBuffer);
	d3d::Release<IDirect3DIndexBuffer9*>(g_pIndexBuffer);
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
			// ���⿡ �׸� ��鿡 ���� �ۼ��մϴ�.
			g_Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(d3d::Vertex));
			g_Device->SetIndices(g_pIndexBuffer);
			g_Device->SetFVF(d3d::Vertex::FVF);
			
			g_Device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,			// �׸����� �ϴ� �⺻�� Ÿ��
				0, 							// �̹� ȣ�⿡ �̿�� �ε����� ������ ��� ��ȣ
				0, 							// ������ �ּ� �ε��� ��
				8, 							// ȣ�⿡ ������ ���� ��
				0, 							// �ε��� ���� ������ �б⸦ ������ ��ҷ��� �ε���
				12);						// �׸����� �ϴ� �⺻���� ��

			// g_Teapot->DrawSubset(0);
		}
		g_Device->EndScene();

		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

