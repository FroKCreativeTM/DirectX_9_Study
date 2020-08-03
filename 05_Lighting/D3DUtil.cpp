#include "D3DUtil.h"

#include "D3DUtil.h"

bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = g_AppName;

	if (!RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = ::CreateWindow(g_AppName, g_AppName,
		WS_EX_TOPMOST,
		0, 0, width, height,
		0, /*parent hwnd*/
		0, /* menu */
		hInstance,
		0 /*extra*/
	);

	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	// 1. IDirect3D9 �������̽����� �����͸� ��´�.
	// �� �������̽��� �ý����� ������ �ϵ���� ��ġ�� ���� ������ ��� 3D �׷����� ���÷����ϴµ�, �̿��ϴ�
	// ������ �ϵ���� ��ġ�� ��Ÿ���� C++ ��ü�� IDirect3DDevice9 �������̽��� ����µ� �̿��Ѵ�.
	IDirect3D9* _d3d9;
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);	// �����ϸ� NULL ��ȯ

	// 2. �⺻ ���÷��� ����Ͱ� �ϵ���� ���ؽ� ���μ����� �����ϴ��� �˾ƺ��� ���� ��ġ Ư���� Ȯ���Ѵ�.
	// IDirect3DDevice9 �������̽��� ����� ���� �̰ͺ��� Ȯ���ؾ� �Ѵ�.
	D3DCAPS9 caps;
	_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// �ϵ���� ���ؽ� ���μ����� �����ϴ���
	int nVP = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		nVP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 3. D3DPRESENT_PARAMETERS ����ü �ν��Ͻ��� �ʱ�ȭ �Ѵ�. �� ����ü�� �츮�� ������� �ϴ�
	// IDirect3DDevice9 Ư���� �����ϱ� ���� �� ���� ������ ����� �����Ѵ�.
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = WIN_WIDTH;
	d3dpp.BackBufferHeight = WIN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;	// ��üȭ��
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// ���� ����
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 4. �ʱ�ȭ�� D3DPRESENT_PARAMETERS�� ���� IDirect3DDevice9 ��ü�� �����Ѵ�.
	// IDirect3DDevice9* device = nullptr;

	HRESULT hResult = _d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// �⺻ ���
		deviceType,				// ��ġ Ÿ��
		hwnd,				// ��ġ�� ����� ������
		nVP,					// ���ؽ� ���μ��� Ÿ��
		&d3dpp,					// �ÿ� ����
		device					// ������ ��ġ
	);

	if (FAILED(hResult))
	{
		MessageBox(nullptr, L"CreateDevice() - FAILED", 0, MB_OK);
		return false;
	}

	return true;
}

int d3d::EnterMsgLoop(bool(*ptr_display)(float deltaTime))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float fLastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	/* �޽����� ������ ������ ������Ʈ�ϰ� ����� �׸��� */
			float fCurTime = (float)timeGetTime();
			float fDeltaTime = (fCurTime - fLastTime) * 0.001f;

			ptr_display(fDeltaTime); // ���÷��� �Լ� ȣ��

			fLastTime = fCurTime;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK d3d::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

D3DLIGHT9 d3d::InitDirectionalLight(XMVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color;
	light.Diffuse = *color * 0.3f;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 d3d::InitPointLight(XMVECTOR3* position, D3DXCOLOR* color)
{
	return D3DLIGHT9();
}

D3DLIGHT9 d3d::InitSpotLight(XMVECTOR3* position, XMVECTOR3* direction, D3DXCOLOR* color)
{
	return D3DLIGHT9();
}
