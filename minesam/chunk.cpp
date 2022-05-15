#include "chunk.h"
#include <iostream>
#include "vertexbuffer.h"
#include <cstdlib>
#include <fstream>

Chunk::Chunk(int x1, int z1, Array2d<Chunk*> *m_chunks) : m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), perlin(16, 6, 1, 95),m_chunks(m_chunks)
{
	m_posX = x1;
	m_posZ = z1;

	std::string load = "sauvegarde" + std::to_string(m_posX) + "_" + std::to_string(m_posZ);
	std::ifstream entrer(load, std::fstream::binary);

	if (entrer.good())
	{
		//taille fichier
		entrer.seekg(0, std::ios_base::end);
		int size = entrer.tellg();
		entrer.seekg(0, std::ios_base::beg);

		BlockType* donner = new BlockType[size];
		entrer.read((char*)donner, size);
		entrer.close();

		m_blocks.SetTableauInterne((BlockType*)donner);
		m_isDirty = true;
	}
	else
	{

		m_blocks.Reset(BTYPE_AIR);
		int y1 = 0;
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				// La m´ethode Get accepte deux param^etre ( coordonn´ee en X et Z) et retourne une valeur qui respecte
				// les valeurs utilis´ees lors de la cr´eation de l ’ objet Perlin
				// La valeur retourn´ee est entre 0 et 1
				float val = perlin.Get((float)(m_posX * CHUNK_SIZE_X + x) / 2000.f, (float)(m_posZ * CHUNK_SIZE_Z + z) / 2000.f);
				// Utiliser val pour d´eterminer la hauteur du terrain `a la position x , z
				// Vous devez vous assurer que la hauteur ne d´epasse pas CHUNK_SIZE_Y
				// Remplir les blocs du bas du terrain jusqu ’ `a la hauteur calcul´ee .
				// N ’ h´esitez pas `a jouer avec la valeur retourn´ee pour obtenir un r´esultat qui vous semble satisfaisant
				y1 = 32 + (val * 64);

				if (y1 >= 30)
				{
					SetBlock(x, y1, z, BTYPE_SNOW);
				}
				else if (y1 <= 16)
				{
					SetBlock(x, y1, z, BTYPE_SAND);

				}
				else
				{
					SetBlock(x, y1, z, BTYPE_GRASS);

				}

			}
		}
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				for (int y = (CHUNK_SIZE_Y - 4); y > 0; --y)
				{
					//r = (std::rand() % 7) + 1;
					if (GetBlock(x, y + 1, z) == BTYPE_GRASS || GetBlock(x, y + 2, z) == BTYPE_GRASS || GetBlock(x, y + 3, z) == BTYPE_GRASS)
					{
						SetBlock(x, y, z, BTYPE_DIRT);
					}
					if (GetBlock(x, y + 1, z) == BTYPE_SNOW || GetBlock(x, y + 2, z) == BTYPE_SNOW || GetBlock(x, y + 3, z) == BTYPE_SNOW)
					{
						SetBlock(x, y, z, BTYPE_DIRT);
					}
					if (GetBlock(x, y + 1, z) == BTYPE_SAND || GetBlock(x, y + 2, z) == BTYPE_SAND || GetBlock(x, y + 3, z) == BTYPE_SAND)
					{
						SetBlock(x, y, z, BTYPE_DIRT);
					}
				}
			}
		}
		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int y = 0;
				while (GetBlock(x, y, z) != BTYPE_DIRT)
				{
					SetBlock(x, y, z, BTYPE_COBBLESTONE);
					++y;
				}
			}
		}
	}
}


Chunk::~Chunk()
{
	if (m_sauvegarder == true)
	{
		std::string s = "sauvegarde" + std::to_string(m_posX) + "_" + std::to_string(m_posZ);
		std::ofstream sauvegarde(s, std::fstream::binary);
		sauvegarde.write((char*)m_blocks.GetTableauInterne(), (CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z));
		sauvegarde.close();
	}
}

void Chunk::RemoveBlock(int x, int y, int z)
{
	m_isDirty = true;
	m_blocks.Set(x, y, z, BTYPE_AIR);

}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{

	m_isDirty = true;
	m_blocks.Set(x, y, z, type);

}

BlockType Chunk::GetBlock(int x, int y, int z)
{
	return m_blocks.Get(x, y, z);
}
void Chunk::Update(BlockInfo* blockinfo[], int x1, int z1)
{
	// Update mesh
	if (m_isDirty)
	{
		int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
		VertexBuffer::VertexData* vd = new VertexBuffer::VertexData[maxVertexCount];

		int count = 0;

		for (int x = 0; x < CHUNK_SIZE_X; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
			{
				for (int y = 0; y < CHUNK_SIZE_Y; ++y)
				{
					if (count > USHRT_MAX)
						break;

					BlockType bt = GetBlock(x, y, z);

					if (bt != BTYPE_AIR)
					{
						AddBlockToMesh(vd, count, bt, (x + x1 * CHUNK_SIZE_X), y, (z + z1 * CHUNK_SIZE_Z), blockinfo[bt]->GetU(), blockinfo[bt]->GetV(), blockinfo[bt]->GetW());
					}
				}
			}
		}
		if (count > USHRT_MAX)
		{
			count = USHRT_MAX;
			std::cout << "[ Chunk :: Update ] Chunk data truncaned , too much vertices to have a 16 bit index " << std::endl;
		}
		m_vertexBuffer.SetMeshData(vd, count);
		delete[] vd;
	}
	m_isDirty = false;
}
void Chunk::AddBlockToMesh(VertexBuffer::VertexData* vd, int & count, BlockType bt, int x, int y, int z, float u, float v, float w)
{
	//if ((x == VIEW_DISTANCE * 2 - 1) || (x != VIEW_DISTANCE * 2 - 1 && GetBlock((x + 1) % 16, y % 128, z % 16) == BTYPE_AIR) || (x % CHUNK_SIZE_X == 15 && ((m_chunks->Get(m_posX + 1, m_posZ) == NULL) || ((m_chunks->Get(m_posX + 1, m_posZ) != NULL && m_chunks->Get(m_posX + 1, m_posZ)->GetBlock(0, y, z) == BTYPE_AIR))))) {}
	
	if(((x % 16) < 15 && GetBlock(((x%16)+1),y% 128,z % 16) == BTYPE_AIR) ||
		((x % 16) == 15 && m_posX < 15 && m_chunks->Get(m_posX+1,m_posZ)->GetBlock(0,y%128,z%16) == BTYPE_AIR)|| 
		(x % 16) == 15 && m_posX == 15)
	{
		//droite
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .8f, .8f, .8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .8f, .8f, .8f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, u, v + w);
	}
	if (y == CHUNK_SIZE_Y - 1 || (y != CHUNK_SIZE_Y - 1 && GetBlock(x % 16, (y + 1) % 128, z % 16) == BTYPE_AIR))
	{
		//dessus
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u, v + w);
	}
	//if ((z == VIEW_DISTANCE * 2 - 1) || (z != VIEW_DISTANCE * 2 - 1 && GetBlock(x % 16, y % 128, (z + 1) % 16) == BTYPE_AIR) || (z % CHUNK_SIZE_X == 15 && ((m_chunks->Get(m_posX, m_posZ+1) == NULL) || ((m_chunks->Get(m_posX, m_posZ+1) != NULL && m_chunks->Get(m_posX, m_posZ+1)->GetBlock(x, y, 0) == BTYPE_AIR)))))
	if (((z % 16) < 15 && GetBlock(x % 16, y % 128, (z % 16)+1) == BTYPE_AIR) ||
		((z % 16) == 15 && m_posZ < 15 && m_chunks->Get(m_posX , m_posZ + 1)->GetBlock(x % 16, y % 128, 0) == BTYPE_AIR) ||
		(z % 16) == 15 && m_posZ == 15)
	{
		// front
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .9f, .9f, .9f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .9f, .9f, .9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .9f, .9f, .9f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .9f, .9f, .9f, u, v + w);
	}

	//if ((x % 16 == 0 && m_chunks->Get(m_posX - 1, m_posZ) != NULL && m_chunks->Get(m_posX - 1, m_posZ)->GetBlock(15, y % 128, z % 16) == BTYPE_AIR) || (x != 0 && GetBlock((x - 1) % 16, y % 128, z % 16) == BTYPE_AIR) || (x % CHUNK_SIZE_X == 0 && ((m_chunks->Get(m_posX - 1, m_posZ) == NULL) || ((m_chunks->Get(m_posX - 1, m_posZ) != NULL && m_chunks->Get(m_posX - 1, m_posZ)->GetBlock(15, y, z) == BTYPE_AIR))))) {}
	
	if (((x % 16) > 0 && GetBlock(((x % 16) - 1), y % 128, z % 16) == BTYPE_AIR) ||
		((x % 16) == 0 && m_posX > 0 && m_chunks->Get(m_posX - 1, m_posZ)->GetBlock(15, y % 128, z % 16) == BTYPE_AIR) ||
		(x % 16) == 0 && m_posX == 0)
	{
		//gauche
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .8f, .8f, .8f, u, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .8f, .8f, .8f, u, v + w);
	}
	if ((y == 0) || (y != 0 && GetBlock(x % 16, (y - 1) % 128, z % 16) == BTYPE_AIR))
	{
		//dessous
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, u, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, 1.f, 1.f, 1.f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u, v + w);
	}
	//if ((z % 16 == 0 && m_chunks->Get(m_posX,m_posZ-1) != NULL && m_chunks->Get(m_posX,m_posZ-1)->GetBlock(x % 16, y % 128, 15)) || (z != 0 && GetBlock(x % 16, y % 128, (z - 1) % 16) == BTYPE_AIR) || (z % CHUNK_SIZE_X == 0 && ((m_chunks->Get(m_posX, m_posZ-1) == NULL) || ((m_chunks->Get(m_posX, m_posZ-1) != NULL && m_chunks->Get(m_posX, m_posZ-1)->GetBlock(x, y, 15) == BTYPE_AIR)))))
	if (((z % 16) > 0 && GetBlock(x % 16, y % 128, (z % 16)-1) == BTYPE_AIR) ||
		((z % 16) == 0 && m_posZ > 0 && m_chunks->Get(m_posX, m_posZ-1)->GetBlock(x%16, y % 128, 15) == BTYPE_AIR) ||
		(z % 16) == 0 && m_posZ == 0)
	{
		//arrière
		vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .9f, .9f, .9f, u, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .9f, .9f, .9f, u + w, v);
		vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .9f, .9f, .9f, u + w, v + w);
		vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .9f, .9f, .9f, u, v + w);
	}
}
void Chunk::SetIsDirtyTrue()
{
	m_isDirty = true;
}

void Chunk::SetSauvegarde()
{
	m_sauvegarder = true;
}

bool Chunk::GetSauvegarde()
{
	return m_sauvegarder;
}

void Chunk::Render() const
{
	m_vertexBuffer.Render();
};
bool Chunk::IsDirty() const
{
	return m_isDirty;
};

