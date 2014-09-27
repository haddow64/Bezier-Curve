#ifndef _CURVE_H
#define _CURVE_H

#include "Maths.h"

//**************************************************************//
//Curve class
//**************************************************************//
class Curve {

public:

	Curve();
	~Curve();

	Vector3 Subpoint[41];
	Vector3 Point[7];
	Vector3 SpherePos;
	Vector3 Delta;

	float length;
	float gravity, acceleration;
	
	Vector3 SphereDir;

	int counter, movement, delay, count;
	int SphereMove;
	void Initialise();
	void New();
	void Reset();
	void Working();
	void Calculate(	Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	void DrawCurve();
	void Continuity();
	void MoveCP(int point, Vector3 Pos);
	void MoveSphere();
	void Draw();

};
#endif