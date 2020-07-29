#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR4 XMVECTOR;
typedef D3DXMATRIX XMMATRIX;

const float PI = 3.141592f;

std::ostream& operator<<(std::ostream& os, const XMMATRIX mat)
{
	os << mat._11 << ", " << mat._12 << ", " << mat._13 << ", " << mat._14 << "\n";
	os << mat._21 << ", " << mat._22 << ", " << mat._23 << ", " << mat._24 << "\n";
	os << mat._31 << ", " << mat._32 << ", " << mat._33 << ", " << mat._34 << "\n";
	os << mat._41 << ", " << mat._42 << ", " << mat._43 << ", " << mat._44;
	return os;
}

int main()
{
	XMMATRIX scalingMat
	(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX rotatedMat
	(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX translationMat
	(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	std::cout << "translation of identity matrix" << std::endl;
	D3DXMatrixTranslation(&translationMat, 1, 2, -3);
	std::cout << translationMat << std::endl;
	std::cout << std::endl << std::endl;

	std::cout << "scaling of identity matrix" << std::endl;
	D3DXMatrixScaling(&scalingMat, 0.2f, 0.2f, 0.2f);
	std::cout << scalingMat << std::endl;
	std::cout << std::endl << std::endl;

	std::cout << "rotation of identity matrix" << std::endl;
	D3DXMatrixRotationY(&rotatedMat, PI / 4);
	std::cout << rotatedMat << std::endl;
	std::cout << std::endl << std::endl;

	return 0;
}