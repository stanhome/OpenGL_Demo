/* 
 * @module:
 * @brief:
 * @author: stan
 * @date:
 */

#pragma once

#include "ogl/math_3d.h"
#include "ogl/camera.h"

//struct Orientation
//{
//	Vector3f m_scale;
//	Vector3f m_rotation;
//	Vector3f m_pos;
//
//	Orientation()
//		: m_scale(Vector3f(1.0f))
//		, m_rotation(Vector3f(0.0f))
//		, m_pos(Vector3f(0.0f))
//	{
//	}
//};


class Pipeline
{
public:
	Pipeline()
		: m_scale(Vector3f(1.0))
		, m_worldPos(Vector3f(0.0f))
		, m_rotateInfo(Vector3f(0.0f))
		, m_persProjInfo()
	{
	}

	void scale(float scaleX, float scaleY, float scaleZ)
	{
		m_scale.x = scaleX;
		m_scale.y = scaleY;
		m_scale.z = scaleZ;
	}

	void worldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void rotate(float rotateX, float rotateY, float rotateZ)
	{
		m_rotateInfo.x = rotateX;
		m_rotateInfo.y = rotateY;
		m_rotateInfo.z = rotateZ;
	}

	void setPerspectiveProj(const PersProjInfo &p)
	{
		m_persProjInfo = p;
	}

	void setCamera(const Vector3f &pos, const Vector3f &target, const Vector3f &up)
	{
		m_camera.pos = pos;
		m_camera.target = target;
		m_camera.up = up;
	}

	void setCamera(const Camera &camera)
	{
		setCamera(camera.getPos(), camera.getTarget(), camera.getUp());
	}

	const Matrix4f &getWPTrans();
	const Matrix4f &getVPTrans();
	const Matrix4f &getWVPTrans();
	const Matrix4f &getWorldTrans();
	const Matrix4f &getViewTrans();
	const Matrix4f &getProjTrans();

private:
	Vector3f m_scale;
	Vector3f m_worldPos;
	Vector3f m_rotateInfo;

	PersProjInfo m_persProjInfo;

	struct {
		Vector3f pos;
		Vector3f target;
		Vector3f up;
	} m_camera;

	Matrix4f m_WVPtransformation;
	Matrix4f m_VPtransformation;
	Matrix4f m_worldProjTransformation;
	Matrix4f m_worldTransformation;
	Matrix4f m_Vtransformation;
	Matrix4f m_projTransformation;
};