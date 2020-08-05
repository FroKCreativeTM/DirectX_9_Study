#include "Box.h"
#include "TextureVertex.h"

Box::Box(IDirect3DDevice9* _device)
    : m_pDevice(_device)
{
    m_pDevice->CreateVertexBuffer(
        24 * sizeof(TextureVertex),
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX,
        D3DPOOL_MANAGED,
        &m_pVertexBuffer,
        0);

    TextureVertex* v;
    m_pVertexBuffer->Lock(0, 0, (void**)&v, 0);
    {
		// ¾ÕÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[0] = TextureVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[1] = TextureVertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[2] = TextureVertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[3] = TextureVertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		// µÞÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[4] = TextureVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[5] = TextureVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[6] = TextureVertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		v[7] = TextureVertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

		// À­ÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[8] = TextureVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[9] = TextureVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[10] = TextureVertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		v[11] = TextureVertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		// ¾Æ·§ÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[12] = TextureVertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		v[13] = TextureVertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		v[14] = TextureVertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
		v[15] = TextureVertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

		// ¿ÞÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[16] = TextureVertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[17] = TextureVertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[18] = TextureVertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[19] = TextureVertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// ¿À¸¥ÂÊ ¸é Á¤Á¡ µ¥ÀÌÅÍ
		v[20] = TextureVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[21] = TextureVertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[22] = TextureVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[23] = TextureVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
    m_pVertexBuffer->Unlock();

	m_pDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndexBuffer,
		0);

	WORD* indices = nullptr;
	m_pIndexBuffer->Lock(0, 0, (void**)&indices, 0);
	{
		// ¾Õ¸éÀÇ ÀÎµ¦½ºµé
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;

		// µÞ¸éÀÇ ÀÎµ¦½ºµé
		indices[6] = 4; indices[7] = 5; indices[8] = 6;
		indices[9] = 4; indices[10] = 6; indices[11] = 7;

		// À­¸éÀÇ ÀÎµ¦½ºµé
		indices[12] = 8; indices[13] = 9; indices[14] = 10;
		indices[15] = 8; indices[16] = 10; indices[17] = 11;

		// ¾Æ·§ ¸éÀÇ ÀÎµ¦½ºµé
		indices[18] = 12; indices[19] = 13; indices[20] = 14;
		indices[21] = 12; indices[22] = 14; indices[23] = 15;

		// ¿ÞÂÊ ¸éÀÇ ÀÎµ¦½ºµé
		indices[24] = 16; indices[25] = 17; indices[26] = 18;
		indices[27] = 16; indices[28] = 18; indices[29] = 19;

		// ¿À¸¥ÂÊ ¸éÀÇ ÀÎµ¦½ºµé
		indices[30] = 20; indices[31] = 21; indices[32] = 22;
		indices[33] = 20; indices[34] = 22; indices[35] = 23;
	}
	m_pIndexBuffer->Unlock();
}

Box::~Box()
{
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
}

bool Box::draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
{
	if (world)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, world);
	}
	if (mtrl)
	{
		m_pDevice->SetMaterial(mtrl);
	}
	if (tex)
	{
		m_pDevice->SetTexture(0, tex);
	}

	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TextureVertex));
	m_pDevice->SetIndices(m_pIndexBuffer);
	m_pDevice->SetFVF(FVF_VERTEX);
	m_pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 
		0,
		24,
		0,
		12);

    return true;
}
