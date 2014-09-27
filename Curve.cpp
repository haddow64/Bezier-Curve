#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Header Files\Main.h"
#include "Header Files\Curve.h"

float Get_Magnitude(Vector3 SphereDir)
{
	return (float)sqrt((SphereDir.x * SphereDir.x) + 
						(SphereDir.y * SphereDir.y) + 
						(SphereDir.z * SphereDir.z) );
}

Vector3 Make_Normal(Vector3 vVector)
{
	float length = Get_Magnitude(vVector);				

	//Divides the vector by the magnitude so it equals 1
	vVector = vVector / length;		

	return vVector;										
}

Curve::Curve()
{
	Initialise();
}

Curve::~Curve()
{
}

void Curve::Initialise()
{
	Reset();

	Point[0].x = 5;		Point[0].y = 60;	Point[0].z = 0;
	Point[1].x = 40;	Point[1].y = 40;	Point[1].z = 10;
	Point[2].x = 34;	Point[2].y = 0;		Point[2].z = 20;
	Point[3].x = 0;		Point[3].y = 10;	Point[3].z = 20;
	Point[5].x = 30;	Point[5].y = 10;	Point[5].z = 0;
	Point[6].x = 15;	Point[6].y = 0;		Point[6].z = 10;

	SpherePos = Point[0];
	Continuity();
	
}

void Curve::New()
{
	counter = 0;
	Initialise();
}

void Curve::Reset()
{
	count = 1;
	counter = 0;
	SphereMove = 0;
	SphereDir.x = 0;
	SphereDir.y = 0;
	SphereDir.z = 0;
	acceleration = 0;
	gravity = 0;
}

void Curve::Working()
{
	Calculate(Point[0], Point[1], Point[2], Point[3]);
	Calculate(Point[3], Point[4], Point[5], Point[6]);
}

void Curve::Calculate(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{

float t = 0.0f;

do{
	Subpoint[counter] =		(a.operator *(((1-t)*(1-t)*(1-t))) +		//(1-t)^3      a
							(b.operator *((3*t)*((1-t)*(1-t)))) +		//3t(1-t)^2    b
							(c.operator *(3*(t*t)*(1-t))) +				//3(t^2) (1-t) c
							(d.operator *(t*t*t)));						//(t^3)        d
	
	counter++;
	t+=0.05;
}while(t<=1.0);
Subpoint[40] = Point[6];

}

void Curve::DrawCurve()
{
glTranslatef(SpherePos.x, SpherePos.y, SpherePos.z);
glColor3f(1.0,1.0,1.0);
glScalef(1, 1, 1);
gluSphere(gluNewQuadric(), 1, 20,20);
glTranslatef(-SpherePos.x, -SpherePos.y, -SpherePos.z);
glColor3f(1.0,1.0,1.0);
}

void Curve::Continuity()
{
	Point[4].x = 2*Point[3].x - Point[2].x;
	Point[4].y = 2*Point[3].y - Point[2].y;
	Point[4].z = 2*Point[3].z - Point[2].z;

	counter = 0;

	Working();
}

void Curve::MoveCP(int point, Vector3 Pos)
{
	Point[point] = Pos;
	
	Reset();
	SpherePos = Point[0];
	Continuity();
}

void Curve::MoveSphere()
{
		if( 
		((SpherePos.x>(Subpoint[SphereMove].x-0.25)))&&
		((SpherePos.x<(Subpoint[SphereMove].x+0.25)))&&
		((SpherePos.y>(Subpoint[SphereMove].y-0.25)))&&
		((SpherePos.y<(Subpoint[SphereMove].y+0.25)))&&
		((SpherePos.z>(Subpoint[SphereMove].z-0.25)))&&
		((SpherePos.z<(Subpoint[SphereMove].z+0.25)))) 
		{
		SpherePos = Subpoint[SphereMove];
		SphereMove++;
	if(SphereMove == 41) { SpherePos = Point[0]; Reset(); }
	
	SphereDir = Subpoint[SphereMove] - Subpoint[SphereMove-1];
	SphereDir = Make_Normal(SphereDir);
	Delta = Subpoint[SphereMove-1] - Subpoint[SphereMove];
	
	acceleration  = Delta.y;
	acceleration /= (float)abs((float)sqrt((	Delta.x * Delta.x) +
												Delta.z * Delta.z));
		}

	gravity+=0.0002*acceleration;
	if(gravity<=0.0020) gravity=0.0020;
	if(gravity>=0.981) gravity = 0.981;
	SpherePos = (SpherePos + (SphereDir.operator *(gravity)));

	
}

void Curve::Draw()
{
int i=0, j=0;

glLineWidth(3);
glBegin(GL_LINES);
	do{
	glColor3f(1.0,0.0,0.0);
	glVertex3f(Subpoint[i].x, Subpoint[i].y, Subpoint[i].z);
	glVertex3f(Subpoint[i+1].x, Subpoint[i+1].y, Subpoint[i+1].z);
	
	i++;
		}while(i<40);
	i++;

	do{
	glColor3f(0.5,1.0,0.5);
	glVertex3f(Subpoint[i].x, Subpoint[i].y, Subpoint[i].z);
	glVertex3f(Subpoint[i+1].x, Subpoint[i+1].y, Subpoint[i+1].z);
	i++;
	}while(i<47);
	glEnd();

	do{
	glTranslatef(Point[j].x,Point[j].y,Point[j].z);
	glColor3f(1.0,1.0,1.0);
	glScalef(1, 1, 1);
	gluSphere(gluNewQuadric(), 0.5, 20,20);
	glTranslatef(-Point[j].x,-Point[j].y,-Point[j].z);
	j++;
	}while(j<7);

glColor3f(1.0,1.0,1.0);

}