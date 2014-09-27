#ifndef _MATHS_H
#define _MATHS_H

struct Vector3
{
public:
	//Default constructor
	Vector3() {}
	Vector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	//+ operator overloaded to add vectors
	Vector3 operator+(Vector3 vVector)
	{
		return Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}
	//- operator overloaded to subtract vectors
	Vector3 operator-(Vector3 vVector)
	{
		return Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	//* operator overloaded for multiplication by a scalar
	Vector3 operator*(float num)
	{
		return Vector3(x * num, y * num, z * num);
	}
	//Divide(/) operator overloaded for division by a scalar
	Vector3 operator/(float num)
	{
		return Vector3(x / num, y / num, z / num);
	}
	float x, y, z;
};
#endif