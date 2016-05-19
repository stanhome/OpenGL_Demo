/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include "math_3d.h"
#include "keys.h"

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	Camera(int windowWidth, int windowHeight, const Vector3f &pos, const Vector3f &target, const Vector3f &up);

	bool onKeyBoard(OGLDEV_KEY key);
	void onMouse(int x, int y);

	void onRender();

	const Vector3f &getPos() const
	{
		return m_pos;
	}

	const Vector3f &getTarget() const
	{
		return m_target;
	}

	const Vector3f &getUp() const
	{
		return m_up;
	}

private:
	void init();
	void update();

private:
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	int m_windowWidth;
	int m_windowHeight;

	float m_angleH;
	float m_angleV;

	bool m_onUpperEdge;
	bool m_onLowerEdge;
	bool m_onLeftEdge;
	bool m_onRightEdge;

	Vector2i m_mousePos;
};
