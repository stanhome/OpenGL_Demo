#pragma once

#include <stdio.h>

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include <math.h>
#endif // WIN32

#include <vector3.h>
#include <matrix3x3.h>
#include <matrix4x4.h>

#include "ogl/types.h"


#define toRadian(x) (float)((x) * M_PI / 180.0f)
#define toDegree(x) (float)((x) * 180.0f / M_PI)

float randomFloat();

struct Vector2i
{
	int x;
	int y;
};

struct Vector2f
{
	float x;
	float y;

	Vector2f()
		:Vector2f(0, 0)
	{
	}

	Vector2f(float _x, float _y)
		: x(_x), y(_y)
	{
	}
};

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{}

	Vector3f(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{}

	Vector3f(float f)
		: x(f), y(f), z(f)
	{}

	Vector3f(const float *pFloatArray)
		: x(pFloatArray[0]), y(pFloatArray[1]), z(pFloatArray[2])
	{}

	Vector3f &operator+=(const Vector3f &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	Vector3f &operator-=(const Vector3f &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	Vector3f &operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;

		return *this;
	}

	operator const float*() const
	{
		return &(x);
	}

	Vector3f cross(const Vector3f &v) const;
	Vector3f &normalize();

	void rotate(float angle, const Vector3f &axis);

	void print() const
	{
		printf("(%.02f, %.02f, %.02f)", x, y, z);
	}

	static Vector3f ZERO;
};

inline Vector3f operator+(const Vector3f &lhs, const Vector3f &rhs)
{
	Vector3f ret(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);

	return ret;
}

inline Vector3f operator-(const Vector3f &lhs, const Vector3f &rhs)
{
	Vector3f ret(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);

	return ret;
}

inline Vector3f operator*(const Vector3f &lhs, float f)
{
	Vector3f ret(lhs.x * f, lhs.y * f, lhs.z * f);

	return ret;
}


/*
  @TODO: vector4f
*/

//inline Vector4f operator/(const Vector4f &lhs, float f)
//{
//
//}

// frustum (平头截体)
struct PersProjInfo
{
	float FOV;
	float width;
	float height;
	float zNear;
	float zFar;
};

struct Quaternion
{
	float x, y, z, w;

	Quaternion(float _x, float _y, float _z, float _w);

	void normalize();

	//共轭
	Quaternion conjugate();

	Vector3f toDegrees();
};

Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, const Vector3f &v);

/*
  @TODO::Matrix4f
*/
class Matrix4f {
public:
	float m[4][4];

	Matrix4f() {}

	inline Matrix4f operator*(const Matrix4f &rhs) const
	{
		Matrix4f ret;
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned j = 0; j < 4; ++j)
			{
				ret.m[i][j] = 
					m[i][0] * rhs.m[0][j] +
					m[i][1] * rhs.m[1][j] +
					m[i][2] * rhs.m[2][j] +
					m[i][3] * rhs.m[3][j];
			}
		}

		return ret;
	}

	void initScaleTransform(float x, float y, float z);
	// x, y, z is degree.
	void initRotateTransform(float x, float y, float z);
	void initTranslationTransform(float x, float y, float z);
	void initCameraTransform(const Vector3f &target, const Vector3f &up);
	void initPersProjTransform(const PersProjInfo &p);

	void logToConsole(const char *title = "") const;
};

