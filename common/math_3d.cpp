#include <stdlib.h>

#include "ogl/math_3d.h"
#include "ogl/util.h"

float randomFloat()
{
	float max = RAND_MAX;
	return ((float)RANDOM() / max);
}

Vector3f Vector3f::ZERO = Vector3f(0.0f, 0.0f, 0.0f);

Vector3f Vector3f::cross(const Vector3f &v) const
{
	const float _x = y * v.z - z * v.y;
	const float _y = z * v.x - x * v.z;
	const float _z = x * v.y - y * v.x;

	return Vector3f(_x, _y, _z);
}

Vector3f &Vector3f::normalize()
{
	const float length = sqrtf(x * x + y * y + z * z);
	x /= length;
	y /= length;
	z /= length;

	return *this;
}

void Vector3f::rotate(float angle, const Vector3f &axe)
{
	const float sinHalfAngle = sinf(toRadian(angle / 2));
	const float cosHalfAngle = cosf(toRadian(angle / 2));

	const float rx = axe.x * sinHalfAngle;
	const float ry = axe.y * sinHalfAngle;
	const float rz = axe.z * sinHalfAngle;
	const float rw = cosHalfAngle;
	Quaternion rotationQ(rx, ry, rz, rw);

	Quaternion conjugateQ = rotationQ.conjugate();
	Quaternion w = rotationQ * (*this) * conjugateQ;

	x = w.x;
	y = w.y;
	z = w.z;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs)
{
	const float w = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
	const float x = (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
	const float y = (lhs.y * rhs.w) + (lhs.w * rhs.y) + (lhs.z * rhs.x) - (lhs.x * rhs.z);
	const float z = (lhs.z * rhs.w) + (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x);

	return Quaternion(x, y, z, w);
}

Quaternion operator*(const Quaternion &q, const Vector3f &v)
{
	const float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	return Quaternion(x, y, z, w);
}

void Quaternion::normalize()
{
	float length = sqrtf(x * x + y * y + z * z + w * w);

	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

Quaternion Quaternion::conjugate()
{
	return Quaternion(-x, -y, -z, w);
}

Vector3f Quaternion::toDegrees()
{
	float f[3];

	//atan2(y,x)所表达的意思是坐标原点为起点，指向(x,y)的射线在坐标平面上与x轴正方向之间的角的角度
	f[0] = atan2(x * z + y * w, x * w - y * z);
	//反余弦
	f[1] = acos(-x * x - y * y - z * z - w * w);
	f[2] = atan2(x * z - y * w, x * w + y * z);

	f[0] = toDegree(f[0]);
	f[1] = toDegree(f[1]);
	f[2] = toDegree(f[2]);

	return Vector3f(f);
}


/*
 * Vector4f
 */
void Matrix4f::initScaleTransform(float x, float y, float z)
{
	m[0][0] = x;	m[0][1] = 0.0f;	m[0][2] = 0.0f;	m[0][3] = 0.0f;
	m[1][0] = 0.0f;	m[1][1] = y;	m[1][2] = 0.0f;	m[1][3] = 0.0f;
	m[2][0] = 0.0f;	m[2][1] = 0.0f;	m[2][2] = z;	m[2][3] = 0.0f;
	m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = 0.0f;	m[3][3] = 1.0f;
}

void Matrix4f::initRotateTransform(float rotateX, float rotateY, float rotateZ)
{
	Matrix4f rx, ry, rz;

	const float x = toRadian(rotateX);
	const float y = toRadian(rotateY);
	const float z = toRadian(rotateZ);

	rx.m[0][0] = 1.0f;		rx.m[0][1] = 0.0f;		rx.m[0][2] = 0.0f;		rx.m[0][3] = 0.0f;
	rx.m[1][0] = 0.0f;		rx.m[1][1] = cosf(x);	rx.m[1][2] = -sinf(x);	rx.m[1][3] = 0.0f;
	rx.m[2][0] = 0.0f;		rx.m[2][1] = sinf(x);	rx.m[2][2] = cosf(x);	rx.m[2][3] = 0.0f;
	rx.m[3][0] = 0.0f;		rx.m[3][1] = 0.0f;		rx.m[3][2] = 0.0f;		rx.m[3][3] = 1.0f;

	ry.m[0][0] = cosf(y);	ry.m[0][1] = 0.0f;		ry.m[0][2] = -sinf(y);	ry.m[0][3] = 0.0f;
	ry.m[1][0] = 0.0f;		ry.m[1][1] = 1.0f;		ry.m[1][2] = 0.0f;		ry.m[1][3] = 0.0f;
	ry.m[2][0] = sinf(y);	ry.m[2][1] = 0.0f;		ry.m[2][2] = cosf(y);	ry.m[2][3] = 0.0f;
	ry.m[3][0] = 0.0f;		ry.m[3][1] = 0.0f;		ry.m[3][2] = 0.0f;		ry.m[3][3] = 1.0f;

	rz.m[0][0] = cosf(z);	rz.m[0][1] = -sinf(z);		rz.m[0][2] = 0.0f;		rz.m[0][3] = 0.0f;
	rz.m[1][0] = sinf(z);	rz.m[1][1] = cosf(z);		rz.m[1][2] = 0.0f;		rz.m[1][3] = 0.0f;
	rz.m[2][0] = 0.0f;		rz.m[2][1] = 0.0f;		rz.m[2][2] = 1.0f;		rz.m[2][3] = 0.0f;
	rz.m[3][0] = 0.0f;		rz.m[3][1] = 0.0f;		rz.m[3][2] = 0.0f;		rz.m[3][3] = 1.0f;

	*this = rz * ry * rx;
}

void Matrix4f::initTranslationTransform(float x, float y, float z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4f::initCameraTransform(const Vector3f &target, const Vector3f &up)
{
	Vector3f N = target;
	N.normalize();
	Vector3f U = up;
	U.normalize();
	U = U.cross(N);
	Vector3f V = N.cross(U);

	m[0][0] = U.x;	m[0][1] = U.y;	m[0][2] = U.z;	m[0][3] = 0.0f;
	m[1][0] = V.x;	m[1][1] = V.y;	m[1][2] = V.z;	m[1][3] = 0.0f;
	m[2][0] = N.x;	m[2][1] = N.y;	m[2][2] = N.z;	m[2][3] = 0.0f;
	m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = 0.0f;	m[3][3] = 1.0f;
}

void Matrix4f::initPersProjTransform(const PersProjInfo &p)
{
	 //ar is screen width / screen height.
	const float ar = p.width / p.height;
	const float zRange = p.zNear - p.zFar;
	const float tanHalfFOV = tanf(toRadian(p.FOV / 2.0f));

	m[0][0] = 1.0f/(ar * tanHalfFOV);	m[0][1] = 0.0f;				m[0][2] = 0.0f;							m[0][3] = 0.0f;
	m[1][0] = 0.0f;						m[1][1] = 1.0f/tanHalfFOV;	m[1][2] = 0.0f;							m[1][3] = 0.0f;
	m[2][0] = 0.0f;						m[2][1] = 0.0f;				m[2][2] = (-p.zNear - p.zFar)/zRange;	m[2][3] = 2.0f * p.zFar * p.zNear / zRange;
	m[3][0] = 0.0f;						m[3][1] = 0.0f;				m[3][2] = 1.0f;							m[3][3] = 0.0f;
}

void Matrix4f::logToConsole(const char *title) const
{
	printf("---- %s Matrix4f value -----\n", title);
	printf("|%f, %f, %f, %f|\n", m[0][0], m[0][1], m[0][2], m[0][3]);
	printf("|%f, %f, %f, %f|\n", m[1][0], m[1][1], m[1][2], m[1][3]);
	printf("|%f, %f, %f, %f|\n", m[2][0], m[2][1], m[2][2], m[2][3]);
	printf("|%f, %f, %f, %f|\n", m[3][0], m[3][1], m[3][2], m[3][3]);
}