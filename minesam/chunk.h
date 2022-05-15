#ifndef CHUNK_H__
#define CHUNK_H__
#include "array2d.h"
#include "array3d.h"
#include "vertexbuffer.h"
#include "blockinfo.h"
#include "perlin.h"

class Chunk
{
    public:
	Chunk(int x, int z, Array2d<Chunk*> *m_chunks);
    ~Chunk();

    void RemoveBlock(int x, int y, int z);
    void SetBlock(int x, int y, int z, BlockType type);
    BlockType GetBlock(int x, int y, int z);
	void Update(BlockInfo* blockinfo[], int x, int z);
	void Render() const;
	bool IsDirty () const ;
	void AddBlockToMesh(VertexBuffer::VertexData * vd, int & count, BlockType bt, int x, int y, int z, float u, float v, float w);
	void AjouterTerrain();
	void SetIsDirtyTrue();
	void SetSauvegarde();
	bool GetSauvegarde();

private:
	Array3d<BlockType> m_blocks;
	VertexBuffer m_vertexBuffer;
	Array2d<Chunk*> *m_chunks;
	bool m_isDirty = false;
	Perlin perlin;
	int m_posX;
	int m_posZ;
	bool m_sauvegarder = false;
};

#endif // CHUNK_H__
