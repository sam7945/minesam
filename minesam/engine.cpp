#include "engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "transformation.h"
#include "player.h"
#include "vector3.h"
#include <GL/gl.h>
#include <stdlib.h>
#include "shader.h"
#include <fstream>


Engine::Engine() :
	m_player(Vector3f(128, (1.7f + 3.001) + 17, 128.f), 0, 0)  //position initiale
	, m_textureAtlas(BLOCK_TYPE(BTYPE_LAST)) //Nombre de block dans texture
	, m_chunks((VIEW_DISTANCE * 2) / CHUNK_SIZE_X, (VIEW_DISTANCE * 2) / CHUNK_SIZE_Z)
	, m_ET()
	, m_blockinfo()
	,m_shader01()
{

}

Engine::~Engine()
{

}

void Engine::Init()
{
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
	{
		std::cerr << " ERREUR GLEW : " << glewGetErrorString(glewErr) << std::endl;
		abort();
	}

	glClearColor(0.196078f, 0.6f, 0.8f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)Width() / (float)Height(), 0.0001f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);

	// Light
	GLfloat light0Pos[4] = { 0.0f, CHUNK_SIZE_Y, 0.0f, 1.0f };
	GLfloat light0Amb[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat light0Diff[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0Spec[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

	CenterMouse();
	HideCursor();
}

void Engine::DeInit()
{

}

void Engine::LoadResource()
{
	TextureAtlas::TextureIndex texDirtIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "dirt.jpg");
	TextureAtlas::TextureIndex texGrassIndex = m_textureAtlas.AddTexture(TEXTURE_PATH "grass.png ");
	TextureAtlas::TextureIndex texGrassCoter = m_textureAtlas.AddTexture(TEXTURE_PATH "sidegrass.png");
	TextureAtlas::TextureIndex texCobble = m_textureAtlas.AddTexture(TEXTURE_PATH "cobble.png");
	TextureAtlas::TextureIndex texTableDessus = m_textureAtlas.AddTexture(TEXTURE_PATH "TableDessus.png");
	TextureAtlas::TextureIndex texTableAvDe = m_textureAtlas.AddTexture(TEXTURE_PATH "TableDevant.png");
	TextureAtlas::TextureIndex texTableGD = m_textureAtlas.AddTexture(TEXTURE_PATH "TableDroit.png");
	TextureAtlas::TextureIndex texTableDessous = m_textureAtlas.AddTexture(TEXTURE_PATH "TableDessous.png");
	TextureAtlas::TextureIndex texObsidian = m_textureAtlas.AddTexture(TEXTURE_PATH "Obsidian.png");
	TextureAtlas::TextureIndex texWood = m_textureAtlas.AddTexture(TEXTURE_PATH "wood.jpg");
	TextureAtlas::TextureIndex texLeaf = m_textureAtlas.AddTexture(TEXTURE_PATH "leaf.jpg");
	TextureAtlas::TextureIndex texSnow = m_textureAtlas.AddTexture(TEXTURE_PATH "snow.jpg");
	TextureAtlas::TextureIndex texSand = m_textureAtlas.AddTexture(TEXTURE_PATH "sand.jpg");


	float u;
	float v;
	float w;
	float h;


	m_textureAtlas.TextureIndexToCoord(texDirtIndex, u, v, w, h);
	m_blockinfo[BTYPE_DIRT] = new BlockInfo(BTYPE_DIRT, "Dirt");
	m_blockinfo[BTYPE_DIRT]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texGrassIndex, u, v, w, h);
	m_blockinfo[BTYPE_GRASS] = new BlockInfo(BTYPE_GRASS, "Grass");
	m_blockinfo[BTYPE_GRASS]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texTableDessus, u, v, w, h);
	m_blockinfo[BTYPE_CRAFT] = new BlockInfo(BTYPE_CRAFT, "Craft");
	m_blockinfo[BTYPE_CRAFT]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texCobble, u, v, w, h);
	m_blockinfo[BTYPE_COBBLESTONE] = new BlockInfo(BTYPE_COBBLESTONE, "Cobblestone");
	m_blockinfo[BTYPE_COBBLESTONE]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texObsidian, u, v, w, h);
	m_blockinfo[BTYPE_OBSIDIAN] = new BlockInfo(BTYPE_OBSIDIAN, "Obsidian");
	m_blockinfo[BTYPE_OBSIDIAN]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texWood, u, v, w, h);
	m_blockinfo[BTYPE_WOOD] = new BlockInfo(BTYPE_WOOD, "Wood");
	m_blockinfo[BTYPE_WOOD]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texLeaf, u, v, w, h);
	m_blockinfo[BTYPE_LEAF] = new BlockInfo(BTYPE_LEAF, "Leaf");
	m_blockinfo[BTYPE_LEAF]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texTableDessous, u, v, w, h);
	m_blockinfo[BTYPE_PLANK] = new BlockInfo(BTYPE_PLANK, "Plank");
	m_blockinfo[BTYPE_PLANK]->SetPositionTexture(u, v, w);


	m_textureAtlas.TextureIndexToCoord(texSnow, u, v, w, h);
	m_blockinfo[BTYPE_SNOW] = new BlockInfo(BTYPE_SNOW, "Snow");
	m_blockinfo[BTYPE_SNOW]->SetPositionTexture(u, v, w);

	m_textureAtlas.TextureIndexToCoord(texSand, u, v, w, h);
	m_blockinfo[BTYPE_SAND] = new BlockInfo(BTYPE_SAND, "Sand");
	m_blockinfo[BTYPE_SAND]->SetPositionTexture(u, v, w);

	m_blockinfo[BTYPE_DIRT]->SetDurability(5);
	m_blockinfo[BTYPE_GRASS]->SetDurability(5);
	m_blockinfo[BTYPE_CRAFT]->SetDurability(10);
	m_blockinfo[BTYPE_COBBLESTONE]->SetDurability(20);
	m_blockinfo[BTYPE_OBSIDIAN]->SetDurability(60);
	m_blockinfo[BTYPE_WOOD]->SetDurability(10);
	m_blockinfo[BTYPE_LEAF]->SetDurability(3);
	m_blockinfo[BTYPE_PLANK]->SetDurability(5);
	m_blockinfo[BTYPE_SNOW]->SetDurability(3);
	m_blockinfo[BTYPE_SAND]->SetDurability(4);



	LoadTexture(m_textureFont, TEXTURE_PATH "font.bmp");
	LoadTexture(m_textureCrosshair, TEXTURE_PATH "cross.bmp");

	LoadTexture(m_textureDirt, TEXTURE_PATH "dirt.jpg");
	LoadTexture(m_textureCobble, TEXTURE_PATH "cobble.png");
	LoadTexture(m_textureGrass, TEXTURE_PATH "grass.png");
	LoadTexture(m_texturetable, TEXTURE_PATH "TableDessus.png");
	LoadTexture(m_textureHUD, TEXTURE_PATH "HUD.png");

	LoadTexture(m_textureObsidian, TEXTURE_PATH "Obsidian.png");
	LoadTexture(m_textureWood, TEXTURE_PATH "wood.jpg");
	LoadTexture(m_textureLeaf, TEXTURE_PATH "leaf.jpg");
	LoadTexture(m_texturePlank, TEXTURE_PATH "TableDessous.png");

	if (!m_textureAtlas.Generate(128, false))
	{
		std::cout << " Unable to generate texture atlas ..." << std::endl;
		abort();
	}

	std::cout << " Loading and compiling shaders ..." << std::endl;
	if (!m_shader01.Load(SHADER_PATH "shader01.vert ", SHADER_PATH "shader01.frag ", true))
	{
		std::cout << " Failed to load shader " << std::endl;
		exit(1);
	}





	//comparer
	for (int x = 0; x < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_X); x++)
	{
		for (int y = 0; y < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_Z); y++)
		{
			//if (donner)
			//{
			//
			//}
			m_chunks.Set(x, y, new Chunk(x, y,&m_chunks));
		}
	}



	//m_chunks.Get(8, 8)->AjouterTerrain();
}

void Engine::UnloadResource()
{
	for (int x = 0; x < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_X); x++)
	{
		for (int y = 0; y < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_Z); y++)
		{
			delete m_chunks.Get(x, y);
		}
	}
}

void Engine::Render(float elapsedTime)
{
	if (elapsedTime > 0.1)
	{
		return;
	}
	m_ET = elapsedTime;

	static float gameTime = elapsedTime;

	gameTime += elapsedTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Transformation transfo;
	m_textureAtlas.Bind();

	for (int x = 0; x < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_X); x++)
	{
		for (int z = 0; z < ((VIEW_DISTANCE * 2) / CHUNK_SIZE_Z); z++)
		{
			if (m_chunks.Get(x, z)->IsDirty())
			{
				m_chunks.Get(x, z)->Update(m_blockinfo, x, z);
			}

			m_shader01.Use();
			m_chunks.Get(x, z)->Render();
		}
	}


	Shader::Disable();

	// Transformations initiales
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3f pos = m_player.Position();

	Vector3f delta = m_player.SimulateMove(m_keyW, m_keyS, m_keyA, m_keyD, m_KeySpace, m_Keyshift, elapsedTime, m_collisionY);

	// Changer ICI delta pour tenir compte de la gravit�, saut, etc

	BlockType bt1, bt2, bt3;

	// Collision par rapport au d�placement en x:
	bt1 = BlockAt(pos.x + delta.x, pos.y, pos.z, BTYPE_AIR);
	bt2 = BlockAt(pos.x + delta.x, pos.y - 0.9f, pos.z, BTYPE_AIR);
	bt3 = BlockAt(pos.x + delta.x, pos.y - 1.7f, pos.z, BTYPE_AIR);
	if (bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR)
		delta.x = 0;

	//collision y
	bt1 = BlockAt(pos.x, pos.y + delta.y, pos.z, BTYPE_AIR);
	bt2 = BlockAt(pos.x, pos.y + delta.y - 0.9f, pos.z, BTYPE_AIR);
	bt3 = BlockAt(pos.x, pos.y + delta.y - 1.7f, pos.z, BTYPE_AIR);
	if (bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR)
	{
		delta.y = 0;
		if (bt1 != BTYPE_AIR)
		{
			m_collisionY = true;
		}
		else if (bt1 == BTYPE_AIR && bt3 != BTYPE_AIR)
		{
			m_collisionY = false;
		}
	}

	//collision z
	bt1 = BlockAt(pos.x, pos.y, pos.z + delta.z, BTYPE_AIR);
	bt2 = BlockAt(pos.x, pos.y - 0.9f, pos.z + delta.z, BTYPE_AIR);
	bt3 = BlockAt(pos.x, pos.y - 1.7f, pos.z + delta.z, BTYPE_AIR);
	if (bt1 != BTYPE_AIR || bt2 != BTYPE_AIR || bt3 != BTYPE_AIR)
		delta.z = 0;

	pos += delta;

	m_player.SetPosition(pos);

	m_player.ApplyTransformation(transfo); //mouvement sourie

	transfo.ApplyTranslation(0.5f, 0.5f, 0.5f);
	transfo.Use();

	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DrawHud();
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GetBlocAtCursor(m_gauche, m_droite);
}

void Engine::KeyPressEvent(unsigned char key) //wasd space //22,0,18,3,57
{
	switch (key)
	{
	case 36: // ESC
		Stop();
		break;
	case 94: // F10
		SetFullscreen(!IsFullscreen());
		break;
	case 22:
		m_keyW = true;//w
		break;
	case 0:
		m_keyA = true;//a
		break;
	case 18:
		m_keyS = true;//s
		break;
	case 3:
		m_keyD = true;//d
		break;
	case 57:
		m_KeySpace = true;
		break;
	case 38:
		m_Keyshift = true;
		break;
	default:
		std::cout << "Unhandled key: " << (int)key << std::endl;
	}
}

void Engine::KeyReleaseEvent(unsigned char key)
{
	switch (key)
	{
	case 24: // Y
		m_wireframe = !m_wireframe;
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 22:
		m_keyW = false;//w
		break;
	case 0:
		m_keyA = false;//a
		break;
	case 18:
		m_keyS = false;//s
		break;
	case 3:
		m_keyD = false;//d
		break;
	case 57:
		m_KeySpace = false;//space
		break;
	case 38:
		m_Keyshift = false;
		break;
	}
}

void Engine::MouseMoveEvent(int x, int y)
{
	// Centrer la souris seulement si elle n'est pas d�j� centr�e
	// Il est n�cessaire de faire la v�rification pour �viter de tomber
	// dans une boucle infinie o� l'appel � CenterMouse g�n�re un
	// MouseMoveEvent, qui rapelle CenterMouse qui rapelle un autre
	// MouseMoveEvent, etc
	if (x == (Width() / 2) && y == (Height() / 2))
		return;

	MakeRelativeToCenter(x, y); //ajuste x et y pour moi, diff�rence entre centre et ou je pointe

	m_player.TurnLeftRight((float)x); //bouge gauche droite x
	m_player.TurnTopBottom((float)y); //bouge haut et bas y

	CenterMouse();
}

void Engine::MousePressEvent(const MOUSE_BUTTON& button, int x, int y)
{
	CenterMouse();
	GetBlocAtCursor(Width(), Height());
	switch (button)
	{
	case OpenglContext::MOUSE_BUTTON::MOUSE_BUTTON_LEFT:
		ChunkAt(m_currentBlock)->RemoveBlock((int)m_currentBlock.x % CHUNK_SIZE_X, (int)m_currentBlock.y % CHUNK_SIZE_Y, (int)m_currentBlock.z % CHUNK_SIZE_Z);
		ChunkAt(m_currentBlock + m_currentFaceNormal)->SetSauvegarde();
		break;
	case OpenglContext::MOUSE_BUTTON::MOUSE_BUTTON_RIGHT:
		m_click = true;

		ChunkAt(m_currentBlock + m_currentFaceNormal)->SetBlock(((int)(m_currentBlock.x + m_currentFaceNormal.x) % 16), ((int)(m_currentBlock.y + m_currentFaceNormal.y) % 128), ((int)(m_currentBlock.z + m_currentFaceNormal.z) % 16), m_choix);
		ChunkAt(m_currentBlock + m_currentFaceNormal)->SetSauvegarde();

		break;
	case OpenglContext::MOUSE_BUTTON::MOUSE_BUTTON_WHEEL_UP:
		--m_choix;
		if (m_choix <= 0)
		{
			m_choix = 8;
		}
		break;
	case OpenglContext::MOUSE_BUTTON::MOUSE_BUTTON_WHEEL_DOWN:
		++m_choix;
		if (m_choix > 8)
		{
			m_choix = 1;
		}
		break;
	}

}

void Engine::MouseReleaseEvent(const MOUSE_BUTTON& button, int x, int y)
{
	switch (button)
	{
	case MOUSE_BUTTON_LEFT:

		break;
	case MOUSE_BUTTON_RIGHT:

		break;
	}
	while (m_click == true)
	{
		m_click = false;
	}

}

bool Engine::LoadTexture(Texture& texture, const std::string& filename, bool stopOnError)
{
	texture.Load(filename);
	if (!texture.IsValid())
	{
		std::cerr << "Unable to load texture (" << filename << ")" << std::endl;
		if (stopOnError)
			Stop();

		return false;
	}

	return true;
}

void Engine::DrawHud()
{
	switch (m_choix)
	{
	case 1:
		m_choixHUDX = 20;
		break;
	case 2:
		m_choixHUDX = 100.0;
		break;
	case 3:
		m_choixHUDX = 180.0;
		break;
	case 4:
		m_choixHUDX = 260.0;
		break;
	case 5:
		m_choixHUDX = 340.0;
		break;
	case 6:
		m_choixHUDX = 420.0;
		break;
	case 7:
		m_choixHUDX = 500.0;
		break;
	case 8:
		m_choixHUDX = 580.0;
		break;
	default:
		break;
	}

	//2D
	// Setter le blend function , tout ce qui sera noir sera transparent
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width(), 0, Height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// Bind de la texture pour le font
	m_textureFont.Bind();
	std::ostringstream ss;
	ss << " Fps : " << GetFps();
	PrintText(10, Height() - 25, ss.str());
	ss.str("");
	ss << " Position : " << m_player.Position(); // IMPORTANT : on utilise l � operateur << pour afficher la position
	PrintText(10, 10, ss.str());




	// Affichage du crosshair
	m_textureCrosshair.Bind();
	static const int crossSize = 32;

	glLoadIdentity();
	glTranslated(Width() / 2 - crossSize / 2, Height() / 2 - crossSize / 2, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(crossSize, 0);
	glTexCoord2f(1, 1);
	glVertex2i(crossSize, crossSize);
	glTexCoord2f(0, 1);
	glVertex2i(0, crossSize);
	glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);

	//dessous 1
	static const int dirt = 64;
	glPushMatrix();
	m_textureHUD.Bind();

	glLoadIdentity();
	glTranslated(20.0, 65.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(644, 0);
	glTexCoord2f(1, 1);
	glVertex2i(644, dirt + 20);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt + 20);
	glEnd();
	glPopMatrix();



	//dessous
	glPushMatrix();
	glLoadIdentity();

	glTranslated(m_choixHUDX, m_choixHUDY, 0);
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2i(0, 0);
	glVertex2i(dirt + 20, 0);
	glVertex2i(dirt + 20, dirt + 20);
	glVertex2i(0, dirt + 20);
	glEnd();
	glPopMatrix();

	//dirt
	glPushMatrix();
	m_textureDirt.Bind();

	glLoadIdentity();
	glTranslated(30.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	//cobble
	glPushMatrix();
	m_textureGrass.Bind();

	glLoadIdentity();
	glTranslated(110.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();


	//grass
	glPushMatrix();
	m_texturetable.Bind();

	glLoadIdentity();
	glTranslated(190.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//table
	glPushMatrix();
	m_textureCobble.Bind();

	glLoadIdentity();
	glTranslated(270.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//Obsidian
	glPushMatrix();
	m_textureObsidian.Bind();

	glLoadIdentity();
	glTranslated(350.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();

	//wood
	glPushMatrix();
	m_textureWood.Bind();

	glLoadIdentity();
	glTranslated(430.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//leaf
	glPushMatrix();
	m_textureLeaf.Bind();

	glLoadIdentity();
	glTranslated(510.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//plank
	glPushMatrix();
	m_texturePlank.Bind();

	glLoadIdentity();
	glTranslated(590.0, 75.0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(1, 0);
	glVertex2i(dirt, 0);
	glTexCoord2f(1, 1);
	glVertex2i(dirt, dirt);
	glTexCoord2f(0, 1);
	glVertex2i(0, dirt);
	glEnd();
	glPopMatrix();

	glPushMatrix();


	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();




}

void Engine::PrintText(unsigned int x, unsigned int y, const std::string & t)
{
	glLoadIdentity();
	glTranslated(x, y, 0);
	for (unsigned int i = 0; i < t.length(); ++i)
	{
		float left = (float)((t[i] - 32) % 16) / 16.0f;
		float top = (float)((t[i] - 32) / 16) / 16.0f;
		top += 0.5f;
		glBegin(GL_QUADS);
		glTexCoord2f(left, 1.0f - top - 0.0625f);
		glVertex2f(0, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top - 0.0625f);
		glVertex2f(12, 0);
		glTexCoord2f(left + 0.0625f, 1.0f - top);
		glVertex2f(12, 12);
		glTexCoord2f(left, 1.0f - top);
		glVertex2f(0, 12);
		glEnd();
		glTranslated(8, 0, 0);
	}
}

int Engine::GetFps()
{
	return 1.f / m_ET;
}

template <class T>
Chunk* Engine::ChunkAt(T x, T y, T z) const
{
	int cx = (int)x / CHUNK_SIZE_X;
	int cz = (int)z / CHUNK_SIZE_Z;
	// validation...

	return m_chunks.Get(cx, cz);
}

template <class T>
Chunk* Engine::ChunkAt(const Vector3<T>& pos) const
{
	return ChunkAt(pos.x, pos.y, pos.z);
}

template <class T>
BlockType Engine::BlockAt(T x, T y, T z, BlockType defaultBlockType) const
{
	Chunk* c = ChunkAt(x, y, z);

	if (!c)
		return defaultBlockType;

	int bx = (int)x % CHUNK_SIZE_X;
	int by = (int)y % CHUNK_SIZE_Y;
	int bz = (int)z % CHUNK_SIZE_Z;

	return c->GetBlock(bx, by, bz);
}
void Engine::GetBlocAtCursor(bool gauche, bool droite)
{
	int x = Width() / 2;
	int y = Height() / 2;

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	posX += .5f;
	posY += .5f;
	posZ += .5f;

	// Le cast vers int marche juste pour les valeurs entiere, utiliser une fonction de la libc si besoin
	// de valeurs negatives
	int px = (int)(posX);
	int py = (int)(posY);
	int pz = (int)(posZ);

	bool found = false;

	if ((m_player.Position() - Vector3f(posX, posY, posZ)).Length() < MAX_SELECTION_DISTANCE)
	{
		// Apres avoir determine la position du bloc en utilisant la partie entiere du hit
		// point retourne par opengl, on doit verifier de chaque cote du bloc trouve pour trouver
		// le vrai bloc. Le vrai bloc peut etre different a cause d'erreurs de precision de nos
		// nombres flottants (si z = 14.999 par exemple, et qu'il n'y a pas de blocs a la position
		// 14 (apres arrondi vers l'entier) on doit trouver et retourner le bloc en position 15 s'il existe
		// A cause des erreurs de precisions, ils arrive que le cote d'un bloc qui doit pourtant etre a la
		// position 15 par exemple nous retourne plutot la position 15.0001
		for (int x = px - 1; !found && x <= px + 1; ++x)
		{
			for (int y = py - 1; !found && x >= 0 && y <= py + 1; ++y)
			{
				for (int z = pz - 1; !found && y >= 0 && z <= pz + 1; ++z)
				{
					if (z >= 0)
					{
						BlockType bt = BlockAt(x, y, z, BTYPE_AIR);
						if (bt == BTYPE_AIR)
							continue;

						// Skip water blocs
						//if(bloc->Type == BT_WATER)
						//    continue;

						m_currentBlock.x = x;
						m_currentBlock.y = y;
						m_currentBlock.z = z;


						if (InRangeWithEpsilon<float>(posX, x, x + 1, 0.05) && InRangeWithEpsilon<float>(posY, y, y + 1, 0.05) && InRangeWithEpsilon<float>(posZ, z, z + 1, 0.05))
						{
							found = true;
						}
					}
				}
			}
		}
	}

	if (!found)
	{
		m_currentBlock.x = -1;
	}
	else
	{
		// Find on which face of the bloc we got an hit
		m_currentFaceNormal.Zero();
		const float epsilon = 0.099999999f;
		// Front et back:
		if (EqualWithEpsilon<float>(posZ, m_currentBlock.z, epsilon))
			m_currentFaceNormal.z = -1;
		else if (EqualWithEpsilon<float>(posZ, m_currentBlock.z + 1, epsilon))
			m_currentFaceNormal.z = 1;
		else if (EqualWithEpsilon<float>(posX, m_currentBlock.x, epsilon))
			m_currentFaceNormal.x = -1;
		else if (EqualWithEpsilon<float>(posX, m_currentBlock.x + 1, epsilon))
			m_currentFaceNormal.x = 1;
		else if (EqualWithEpsilon<float>(posY, m_currentBlock.y, epsilon))
			m_currentFaceNormal.y = -1;
		else if (EqualWithEpsilon<float>(posY, m_currentBlock.y + 1, epsilon))
			m_currentFaceNormal.y = 1;

		if (gauche == true && m_click == false)
		{
			m_click = true;
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->RemoveBlock((int)m_currentBlock.x % 16, (int)m_currentBlock.y % 128, (int)m_currentBlock.z % 16);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
		}
		else if (droite == true && m_click == false)
		{
			if (m_currentFaceNormal.z == -1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock((int)m_currentBlock.x % 16, (int)m_currentBlock.y % 128, ((int)m_currentBlock.z - 1) % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
			if (m_currentFaceNormal.y == -1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock((int)m_currentBlock.x % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
			if (m_currentFaceNormal.x == -1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x - 1) % 16, (int)m_currentBlock.y % 128, (int)m_currentBlock.z % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
			if (m_currentFaceNormal.z == 1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock((int)m_currentBlock.x % 16, (int)m_currentBlock.y % 128, ((int)m_currentBlock.z + 1) % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
			if (m_currentFaceNormal.y == 1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock((int)m_currentBlock.x % 16, ((int)m_currentBlock.y + 1) % 128, (int)m_currentBlock.z % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
			if (m_currentFaceNormal.x == 1)
			{
				m_click = true;
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x + 1) % 16, (int)m_currentBlock.y % 128, (int)m_currentBlock.z % 16, m_choix);
				ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetSauvegarde();
			}
		}
	}
	if (m_click == true)
	{
		Craft();
	}
}
void Engine::Craft()
{
	//arbre
	if (ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y) % 128, (int)m_currentBlock.z % 16) == BTYPE_CRAFT
		&& ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16) == BTYPE_GRASS)
	{
		for (int i = 0; i < 7; i++)
		{
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y + i) % 128, ((int)m_currentBlock.z) % 16, BTYPE_WOOD);
		}
		ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, ((int)m_currentBlock.z) % 16, BTYPE_DIRT);

		for (int h = 0; h < 4; h++)
		{
			for (int x1 = (m_currentBlock.x - 2) + h; x1 <= (m_currentBlock.x + 2) - h; x1++)
			{
				for (int z1 = (m_currentBlock.z - 2) + h; z1 <= (m_currentBlock.z + 2) - h; z1++)
				{
					for (int y1 = (m_currentBlock.y + 4); y1 <= (m_currentBlock.y + 4) + h; y1++)
					{
						if (x1 != 0 && z1 != 0)
						{
							ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(x1 % 16, y1 % 128, z1 % 16, BTYPE_LEAF);
						}
					}
				}

			}
		}
	}

	//obsidian
	if (ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y) % 128, (int)m_currentBlock.z % 16) == BTYPE_CRAFT
		&& ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16) == BTYPE_COBBLESTONE)
	{
		ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y) % 128, (int)m_currentBlock.z % 16, BTYPE_AIR);
		ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, BTYPE_OBSIDIAN);
	}

	//planks
	if (ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y) % 128, (int)m_currentBlock.z % 16) == BTYPE_CRAFT
		&& ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->GetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16) == BTYPE_WOOD)
	{
		ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y) % 128, (int)m_currentBlock.z % 16, BTYPE_AIR);
		if (m_currentBlock.x <= 13 && m_currentBlock.z <= 13)
		{
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x + 1) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x + 1) % 16, ((int)m_currentBlock.y - 1) % 128, ((int)m_currentBlock.z + 1) % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, ((int)m_currentBlock.z + 1) % 16, BTYPE_PLANK);
		}
		else
		{
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x - 1) % 16, ((int)m_currentBlock.y - 1) % 128, (int)m_currentBlock.z % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x - 1) % 16, ((int)m_currentBlock.y - 1) % 128, ((int)m_currentBlock.z - 1) % 16, BTYPE_PLANK);
			ChunkAt(m_currentBlock.x, m_currentBlock.y, m_currentBlock.z)->SetBlock(((int)m_currentBlock.x) % 16, ((int)m_currentBlock.y - 1) % 128, ((int)m_currentBlock.z - 1) % 16, BTYPE_PLANK);
		}
	}
}

template <class T>
bool Engine::EqualWithEpsilon(const T& v1, const T& v2, T epsilon)
{
	return (fabs(v2 - v1) < epsilon);
}

template <class T>
bool Engine::InRangeWithEpsilon(const T& v, const T& vinf, const T& vsup, T epsilon)
{
	return (v >= vinf - epsilon && v <= vsup + epsilon);
}