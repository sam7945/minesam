#ifndef PLAYER_H__
#define PLAYER_H__

#include "vector3.h"
#include "define.h"
#include "transformation.h"

class Player
{
public:
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	Vector3f SimulateMove(bool front, bool back, bool left, bool right, bool space, bool shift, float elapsedTime,bool collisionY);
	void ApplyTransformation(Transformation & transformation) const;
	Vector3f Position();
	void SetPosition(Vector3f pos);

private:
	float lastx;
	float lasty;

	float xrot = 0;
	float yrot = 0;

	float diffy;
	float diffx;

	float m_last = 0;
	float m_now = 0;

	bool m_saut = false;
	bool m_collisionY = false;
	bool m_sol = false;

	float xpos = 0, ypos = 0, zpos = 0, angle = 0;

	Vector3f m_position;
};
#endif	
