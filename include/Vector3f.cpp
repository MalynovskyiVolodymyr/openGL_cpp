#include "Vector3f.h"
#include "math_utils.h"

void Vector3f::Rotate(float Angle, const Vector3f& V){
	Quaternion RotateQ(Angle, V);
	Quaternion ConjugateQ = RotateQ.Conjugate();
	Quaternion W = RotateQ * (*this) * ConjugateQ;
	
	x = W.x;
	y = W.y;
	z = W.z;
}
