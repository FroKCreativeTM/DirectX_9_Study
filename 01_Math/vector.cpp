#include <iostream>

#include <d3d9.h>
#include <d3dx9.h>

typedef D3DXVECTOR4 XMVECTOR;
typedef D3DXMATRIX XMMATRIX;

std::ostream& operator<<(std::ostream& os, const XMVECTOR vec)
{
	os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	return os;
}

int main()
{
	XMVECTOR vec1(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR vec2(3.0f, 3.0f, 3.0f, 1.0f);

	XMVECTOR vec3 = vec1 + vec2;

	std::cout << vec3 << std::endl;

	return 0;
}