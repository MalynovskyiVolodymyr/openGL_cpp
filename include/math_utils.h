#ifndef MATH_UTILS
#define MATH_UTILS

#include "Vector3f.h"
#include <cmath>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Quaternion{
	float x, y, z, w;
	
	Quaternion(float Angle, const Vector3f &V){
		float HalfAngleInRadians = ToRadian(Angle/2);
		
		float SinHalfAngle = sinf(HalfAngleInRadians);
		float CosHalfAngle = cosf(HalfAngleInRadians);
		
		x = V.x * SinHalfAngle;
		y = V.y * SinHalfAngle;
		z = V.z * SinHalfAngle;
		w = CosHalfAngle;
	}
	
	Quaternion(float _x, float _y, float _z, float _w){
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	
	void Normalize(){
		float Length = sqrtf(x * x + y * y + z * z + w * w);
		
		x /= Length;
		y /= Length;
		z /= Length;
		w /= Length;
	}
	
	Quaternion Conjugate() const{
		Quaternion ret(-x,-y,-z,w);
		return ret;
	}
	
	Vector3f ToDegrees(){
		float f[3];
		
		f[0] = atan2(x * z + y * w, x * w - y * z);
		f[1] = acos(-x * x - y * y - z * z - w * w);
		f[2] = atan2(x * z - y * w, x * w + y * z);
		
		f[0] = ToDegree(f[0]);
		f[1] = ToDegree(f[1]);
		f[2] = ToDegree(f[2]);
		
		return Vector3f(f);
	}
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);






















#endif




