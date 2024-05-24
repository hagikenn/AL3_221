#include "MathUtilityForText.h"

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) { Vector3 dm;
	dm = scale;
	dm = rot;

	Matrix4x4 result{
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f, 
			translate.x, 
			translate.y, 
			translate.z,
			1.0f
		};
	
	return result; 
}
