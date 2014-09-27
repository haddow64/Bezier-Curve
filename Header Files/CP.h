#ifndef _POINT_H
#define _POINT_H

#include "Maths.h"

class Point {

public:

	Point();
	~Point();
	
	Vector3 Subpoint[25];

	void Initialise();
	void Calculate(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	void Draw();
};
#endif