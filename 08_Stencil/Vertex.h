#pragma once

/* ======================== 버텍스 구조체 ======================== */
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x = x;
		_y = y;
		_z = z;

		_nx = nx;
		_ny = ny;
		_nz = nz;

		_u = u;
		_v = v;
	}

	float _x, _y, _z;		// 위치 벡터
	float _nx, _ny, _nz;	// 법선 벡터
	float _u, _v;			// 텍스처 벡터 
};
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) // 정점 변환 이전 좌표