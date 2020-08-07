#include "D3DUtil.h"

using namespace std;

typedef D3DXVECTOR3 XMVECTOR3;
typedef D3DXMATRIX XMMATRIX;

/* ========= 공통 함수 ========= */
bool Setup();
void Cleanup();
bool Display(float fDeltaTime);

/* ========= 전역 변수 ========= */
IDirect3DDevice9*				g_Device = nullptr;				// 다이렉트3D 디바이스
IDirect3DVertexBuffer9*			g_pVertexBuffer = nullptr;		// 정점 버퍼
IDirect3DIndexBuffer9*			g_pIndexBuffer = nullptr;		// 인덱스 버퍼
// ID3DXMesh* g_Teapot = nullptr;								// 테스트용 메시

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
	// 정점과 인덱스 버퍼를 생성한다.
	g_Device->CreateVertexBuffer(
		8 * sizeof(d3d::Vertex),							// 버퍼에 할당할 바이트 수
		D3DUSAGE_WRITEONLY,									// 버퍼가 이용될 방법을 결정하는 부가적 특성
		d3d::Vertex::FVF,									// 정점 버퍼에 보관될 정점의 유연한 정점 포맷
		D3DPOOL_MANAGED,									// 버퍼가 위치할 메모리 풀
		&g_pVertexBuffer,									// 만들어질 정점 버퍼를 받을 포인터
		0);													// 이용되지 않는 곳이다.

	g_Device->CreateIndexBuffer(
		36 * sizeof(DWORD),									// 버퍼에 할당할 바이트 수
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 								// 버퍼가 이용될 방법을 결정하는 부가적 특성
		D3DFMT_INDEX16, 									// 정점 버퍼에 보관될 정점의 유연한 정점 포맷
		D3DPOOL_MANAGED,									// 버퍼가 위치할 메모리 풀
		&g_pIndexBuffer, 									// 만들어질 정점 버퍼를 받을 포인터
		0);													// 이용되지 않는 곳이다.

	// 큐브 정점 데이터를 생성하고 버퍼에 이를 채운다.
	d3d::Vertex* vertices;
	g_pVertexBuffer->Lock(
		0,						// 잠금을 시작할 버퍼 위치의 오프셋
		0, 						// 잠글 바이트의 수
		(void**)&vertices, 		// 잠근 메모리의 시작을 가리키는 포인터
		0);						// 잠금이 이루어지는 방법을 지정한다.

	// 단위 큐브의 정점들
	vertices[0] = d3d::Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = d3d::Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = d3d::Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = d3d::Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = d3d::Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = d3d::Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = d3d::Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = d3d::Vertex(1.0f, -1.0f, 1.0f);

	g_pVertexBuffer->Unlock();

	// 입방체의 삼각형들을 정의한다.
	WORD* indices = nullptr;
	g_pIndexBuffer->Lock(
		0,						// 잠금을 시작할 버퍼 위치의 오프셋
		0, 						// 잠글 바이트의 수
		(void**)&indices, 		// 잠근 메모리의 시작을 가리키는 포인터
		0);						// 잠금이 이루어지는 방법을 지정한다.

	// 전면
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// 후면
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// 왼쪽 측면
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// 오른쪽 측면
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// 상단
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// 하단
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	g_pIndexBuffer->Unlock();

	// 카메라의 위치와 방향을 조정한다.
	XMVECTOR3 position(0.0f, 0.0f, -5.0f);
	XMVECTOR3 target(0.0f, 0.0f, 0.0f);
	XMVECTOR3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &position, &target, &up);
	g_Device->SetTransform(D3DTS_VIEW, &matView);

	// 투영 행렬을 지정한다.
	XMMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)WIN_WIDTH / (float)WIN_HEIGHT,
		1.0f,
		1000.0f);
	g_Device->SetTransform(D3DTS_PROJECTION, &matProj);

	// 렌더 상태를 지정한다. (현재는 와이어 프레임)
	g_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(g_pVertexBuffer);
	d3d::Release<IDirect3DIndexBuffer9*>(g_pIndexBuffer);
}

// 갱신과 렌더링의 두 가지 역할을 담당
bool Display(float fDeltaTime)
{
	D3DCOLOR bgColour = 0xFFFDAB9F;	// 배경색상 - 살몬 핑크

	// D3DXCreateTeapot(g_Device, &g_Teapot, 0);

	if (g_Device)
	{
		// 입방체를 회전시킨다.
		XMMATRIX Rx, Ry;

		// x축으로 45도 회전시킨다.
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		// 각 프레임당 y 회전을 증가시킨다.
		static float fy = 0.0f;
		D3DXMatrixRotationY(&Ry, fy);
		fy += fDeltaTime;

		// 각도가 2 * PI에 이르면 초기화
		if (fy >= 6.28f)
			fy = 0.0f;

		// 회전을 결합한다.
		XMMATRIX p = Rx * Ry;

		g_Device->SetTransform(D3DTS_WORLD, &p);

		// 장면을 그린다.
		g_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, bgColour, 1.0f, 0);

		g_Device->BeginScene();
		{
			// 여기에 그릴 장면에 대해 작성합니다.
			g_Device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(d3d::Vertex));
			g_Device->SetIndices(g_pIndexBuffer);
			g_Device->SetFVF(d3d::Vertex::FVF);
			
			g_Device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,			// 그리고자 하는 기본형 타입
				0, 							// 이번 호출에 이용될 인덱스에 더해질 기반 번호
				0, 							// 참조할 최소 인덱스 값
				8, 							// 호출에 참조될 정점 수
				0, 							// 인덱스 버퍼 내에서 읽기를 시작할 요소로의 인덱스
				12);						// 그리고자 하는 기본형의 수

			// g_Teapot->DrawSubset(0);
		}
		g_Device->EndScene();

		g_Device->Present(0, 0, 0, 0);
	}

	return true;
}

