#include "ogl/camera.h"

const static float STEP_SCALE = 1.0f;
const static float EDGE_STEP = 0.5f;
//边缘的默认范围
const static int MARGIN = 10;

Camera::Camera(int width, int height)
	: Camera(width, height, Vector3f(0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 1.0f, 0.0f))
{
}

Camera::Camera(int width, int height, const Vector3f &pos, const Vector3f &target, const Vector3f &up)
	: m_pos(pos)
	, m_target(target)
	, m_up(up)
	, m_windowWidth(width)
	, m_windowHeight(height)
	, m_angleH(0.0f)
	, m_angleV(0.0f)
	, m_onUpperEdge(false)
	, m_onLowerEdge(false)
	, m_onLeftEdge(false)
	, m_onRightEdge(false)
	, m_mousePos()
{
	m_target.normalize();
	m_up.normalize();

	init();
}

bool Camera::onKeyBoard(OGLDEV_KEY key)
{
	bool ret = false;
	switch (key)
	{

	case OGLDEV_KEY_UP:
	{
		m_pos += (m_target * STEP_SCALE);
		ret = true;
	}
		break;

	case OGLDEV_KEY_DOWN:
	{
		m_pos -= (m_target * STEP_SCALE);
		ret = true;
	}
		break;

	case OGLDEV_KEY_LEFT:
	{
		//left-handed coordinate system.
		Vector3f left = m_target.cross(m_up);
		left.normalize();
		left *= STEP_SCALE;
		m_pos += left;

		ret = true;
	}
		break;

	case OGLDEV_KEY_RIGHT:
	{
		Vector3f right = m_up.cross(m_target);
		right.normalize();
		right *= STEP_SCALE;
		m_pos += right;
		ret = true;
	}
		break;

	case OGLDEV_KEY_PAGE_UP:
	{
		m_pos.y += STEP_SCALE;
		break;
	}

	case OGLDEV_KEY_PAGE_DOWN:
	{
		m_pos.y -= STEP_SCALE;
		break;
	}

	default:
		break;
	}

	return ret;
}

void Camera::onMouse(int x, int y)
{
	const int deltaX = x - m_mousePos.x;
	const int deltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	m_angleH += (float)deltaX / 20.f;
	m_angleV += (float)deltaY / 20.f;

	if (deltaX == 0)
	{
		if (x <= MARGIN)
		{
			m_onLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN))
		{
			m_onRightEdge = true;
		}
	}
	else
	{
		m_onLeftEdge = m_onRightEdge = false;
	}

	if (deltaY == 0)
	{
		if (y <= MARGIN)
		{
			m_onUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN))
		{
			m_onLowerEdge = true;
		}
	}
	else
	{
		m_onUpperEdge = m_onLowerEdge = false;
	}

	update();
}

void Camera::onRender()
{
	bool shouldUpdate = false;
	if (m_onLeftEdge)
	{
		m_angleH -= EDGE_STEP;
		shouldUpdate = true;
	}
	else if (m_onRightEdge)
	{
		m_angleH += EDGE_STEP;
		shouldUpdate = true;
	}
	else if (m_onUpperEdge)
	{
		if (m_angleV > -90.0f)
		{
			m_angleV -= EDGE_STEP;
			shouldUpdate = true;
		}
	}
	else if (m_onLowerEdge)
	{
		if (m_angleV < 90.0f)
		{
			m_angleV += EDGE_STEP;
			shouldUpdate = true;
		}
	}

	if (shouldUpdate)
	{
		update();
	}
}


void Camera::init()
{
	Vector3f htarget(m_target.x, 0.0, m_target.z);
	htarget.normalize();
	if (htarget.z >= 0.0f)
	{
		if (htarget.x >= 0.0f)
		{
			//第一象限
			m_angleH = 360.0f - toDegree(asin(htarget.z));
		}
		else
		{
			//第二象限
			m_angleH = 180.0f + toDegree(asin(htarget.z));
		}
	}
	else
	{
		if (htarget.x >= 0.0f)
		{
			m_angleH = toDegree(asin(-htarget.z));
		}
		else
		{
			m_angleH = 90.0f + toDegree(asin(-htarget.z));
		}
	}

	m_angleV = -toDegree(asin(m_target.y));

	m_onUpperEdge = false;
	m_onLowerEdge = false;
	m_onLeftEdge = false;
	m_onRightEdge = false;
	m_mousePos.x = m_windowWidth / 2.0;
	m_mousePos.y = m_windowHeight / 2.0;

	//用于鼠标移动
	//glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

void Camera::update()
{
	const Vector3f vAxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Vector3f view(1.0f, 0.0f, 0.0f);
	view.rotate(m_angleH, vAxis);

	// rotate the view vector by the vertical angle around the horizontal axis
	Vector3f hAxis = vAxis.cross(view);
	hAxis.normalize();
	view.rotate(m_angleV, hAxis);

	m_target = view;
	m_target.normalize();

	m_up = m_target.cross(hAxis);
	m_up.normalize();
}