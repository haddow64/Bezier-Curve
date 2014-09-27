#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Header Files\Main.h"
#include "Header Files\CP.h"

Point::Point()
{
	Initialise();
}

Point::~Point()
{
}

void Point::Initialise()
{
}

void Point::Calculate(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{

float t = 0.0f;
int i = 0;

do{

	Subpoint[i].x =	((((1-t)*(1-t)*(1-t))*a.x) +		//(1-t)^3      a
					(((3*t)*((1-t)*(1-t)))*b.x) +		//3t(1-t)^2    b
					((3*(t*t)*(1-t))*c.x) +				//3(t^2) (1-t) c
					((t*t*t)*d.x));						//(t^3)        d

	Subpoint[i].y =	((((1-t)*(1-t)*(1-t))*a.y) +		//(1-t)^3      a
					(((3*t)*((1-t)*(1-t)))*b.y) +		//3t(1-t)^2    b
					((3*(t*t)*(1-t))*c.y) +				//3(t^2) (1-t) c
					((t*t*t)*d.y));						//(t^3)        d

	Subpoint[i].z =	((((1-t)*(1-t)*(1-t))*a.z) +		//(1-t)^3      a
					(((3*t)*((1-t)*(1-t)))*b.z) +		//3t(1-t)^2    b
					((3*(t*t)*(1-t))*c.z) +				//3(t^2) (1-t) c
					((t*t*t)*d.z));						//(t^3)        d
	i++;
	t+=0.05;
}while(t<=1.0);
}

void Point::Draw()
{
int i=0;

glBegin(GL_LINES);
do{
	glVertex3f(Subpoint[i].x, Subpoint[i].y, Subpoint[i].z);
	glVertex3f(Subpoint[i+1].x, Subpoint[i+1].y, Subpoint[i+1].z);
	
	i++;
}while(i<20);
glEnd();
}