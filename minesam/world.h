#include "chunk.h"
#include "array2d.h"
#include "vector3.h"
#include "Player.h"

#ifndef WORLD_H__
#define WORLD_H__

class World
{
public:
	World(Vector3f* m_currentBlock,Vector3f* m_currentFaceNormal,Vector3f* m_posPrecedent, Player* player);
	~World();
	
	
	
	Array2d<Chunk*> getChunks() const;
	void setChunks(int x, int y);

	void GetBlocAtCursor(int Width, int Height,bool m_click);
	void Craft();
	template<class T>
	Chunk* ChunkAt(T x, T y, T z) const;
	template<class T>
	Chunk* ChunkAt(const Vector3<T>& pos) const;
	template<class T>
	BlockType BlockAt(T x, T y, T z, BlockType defaultBlockType) const;
	template<class T>
	bool EqualWithEpsilon(const T& v1, const T& v2, T epsilon);
	template<class T>
	bool InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon);

	

private:
	Array2d<Chunk*> m_chunks;
	Vector3f* m_currentBlock;
	Vector3f* m_currentFaceNormal;
	Vector3f* m_posPrecedent;
	Player* m_player;
	
	
};
template <class T>
Chunk* World::ChunkAt(T x, T y, T z) const
{
	int cx = (int)x / CHUNK_SIZE_X;
	int cz = (int)z / CHUNK_SIZE_Z;
	// validation...

	return getChunks().Get(cx, cz);
}

template <class T>
Chunk* World::ChunkAt(const Vector3<T>& pos) const
{
	return ChunkAt(pos.x, pos.y, pos.z);
}

template <class T>
BlockType World::BlockAt(T x, T y, T z, BlockType defaultBlockType) const
{
	Chunk* c = ChunkAt(x, y, z);

	if (!c)
		return defaultBlockType;

	int bx = (int)x % CHUNK_SIZE_X;
	int by = (int)y % CHUNK_SIZE_Y;
	int bz = (int)z % CHUNK_SIZE_Z;

	return c->GetBlock(bx, by, bz);
}
template <class T>
bool World::EqualWithEpsilon(const T& v1, const T& v2, T epsilon)
{
	return (fabs(v2 - v1) < epsilon);
}

template <class T>
bool World::InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon)
{
	return (v >= vinf - epsilon && v <= vsup + epsilon);
}


#endif // !WORLD_H__

