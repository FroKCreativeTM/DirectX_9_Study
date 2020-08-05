#include "D3DUtil.h"
#include "TextureVertex.h"

/* ========= ���� �Լ� ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= ���� ���� ========= */
IDirect3DDevice9* g_Device = nullptr;				// ���̷�Ʈ3D ����̽�
IDirect3DVertexBuffer9* g_pVertexBuffer = nullptr;		// ���� ����

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
	// ���� ���۸� �����Ѵ�.
	g_Device->CreateVertexBuffer(
		8 * sizeof(TextureVertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&g_Quad,
		0);

	// �Ƕ�̵� �����ͷ� ���� ���۸� ä���.
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

	// ���� ��� �� ��
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
		// wrap ��巹�� ���� �����Ѵ�.
		if (GetAsyncKeyState('W') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}
		// border color ��巹�� ���� �����Ѵ�.
		if (GetAsyncKeyState('B') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			g_Device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
		}
		// clamp ��巹�� ���� �����Ѵ�.
		if (GetAsyncKeyState('C') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}
		// mirror ��巹�� ���� �����Ѵ�.
		if (GetAsyncKeyState('M') & 0x8000f)
		{
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			g_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}

		D3DCOLOR bgColour = 0xFFFDAB9F;	// ������ - ��� ��ũ
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