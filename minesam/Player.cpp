#include "Player.h"
#include "openglcontext.h"

Player::Player(const Vector3f& position, float rotX, float rotY) :m_position(position), xrot(rotX), yrot(rotY)
{

}
void Player::TurnLeftRight(float value)
{
	yrot += value * .1f;// set the xrot to yrot with the addition of the difference in the x position
					  //change yrot avec l'addition de yrot avec la différence de la position y

};
void Player::TurnTopBottom(float value)
{
	xrot += value * .1f; //set the xrot to xrot with the addition of the difference in the y position
						//change xrot avec l'addition de xrot avec la différence de la position y

	if (xrot >= 90)
	{
		xrot -= value * .1f;
	}
	if (xrot <= -90)
	{
		xrot -= value * .1f;
	}

};
Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, bool space, bool shift, float elapsedTime, bool collisionY)
{
	Vector3f delta(0, 0, 0);
	m_collisionY = collisionY;

	if (front)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		delta.x += float(sin(yrotrad));
		delta.z -= float(cos(yrotrad));
		//ypos -= float(sin(xrotrad));//si je veux bouger de haut en bas en me déplacant avec la sourie
	}
	if (back)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		delta.x -= float(sin(yrotrad));
		delta.z += float(cos(yrotrad));
		//ypos += float(sin(xrotrad)); //si je veux bouger de haut en bas en me déplacant avec la sourie
	}
	if (left)
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		delta.x -= float(cos(yrotrad)) * 0.8;
		delta.z -= float(sin(yrotrad)) * 0.8;
	}
	if (right)
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		delta.x += float(cos(yrotrad)) * 0.8;
		delta.z += float(sin(yrotrad)) * 0.8;
	}
	if (space && m_saut == false && m_collisionY == false && m_sol == false)
	{
		if ((m_now - m_last) < 1.1f)
		{
			delta.y += elapsedTime * 40;
			m_now = m_position.y;

		}
		else if ((m_now - m_last) >= 1.1f)
		{
			m_saut = true;
		}
	}
	if (m_saut == true || m_collisionY == true)
	{
		delta.y -= elapsedTime * 70;
		m_now = m_position.y;
		m_sol = true;
		if ((m_now - m_last) == 0)
		{
			m_saut = false;
		}
	}

	if (!space)
	{
		delta.y -= elapsedTime * 70;
		if ((m_now - m_last) == 0)
		{
			m_sol = false;
		}
		m_last = m_position.y;
		m_now = m_position.y;
	}

	delta.x *= elapsedTime * 8;// vitesse, boost vitesse
	delta.y *= elapsedTime * 8;
	delta.z *= elapsedTime * 8;




	//m_position += Vector3f(xpos, ypos, zpos);
	return delta;
};
void Player::ApplyTransformation(Transformation & transformation) const
{
	transformation.ApplyRotation(-xrot, 1.f, 0, 0);
	transformation.ApplyRotation(-yrot, 0, 1.f, 0);
	transformation.ApplyTranslation(-m_position);
}
Vector3f Player::Position()
{
	return m_position;
}

void Player::SetPosition(Vector3f pos)
{
	m_position = pos;
}

