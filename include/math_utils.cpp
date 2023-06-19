#include "math_utils.h"
#include "Vector3f.h"

Quaternion operator*(const Quaternion &l, const Quaternion &r){
	float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);
	
	Quaternion ret(x, y, z, w);
	return ret; 	
}

Quaternion operator*(const Quaternion &q, const Vector3f &v){
	float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);
	
	Quaternion ret(x,y,z,w);
	return ret;
}
