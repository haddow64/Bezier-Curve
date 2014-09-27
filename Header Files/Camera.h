#ifndef _CAMERA_H
#define _CAMERA_H

#include "Maths.h"

//**************************************************************//
//Camera Class
//**************************************************************//
class Camera {

public:
	Camera();
	//~Camera();

	//Camera Local Data
	Vector3 Position()	{	return m_vPosition;		}
	Vector3 View()		{	return m_vView;			}
	Vector3 UpVector()	{	return m_vUpVector;		}
	Vector3 Strafe()	{	return m_vStrafe;		}
	
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);


	void RotateView(float angle, float X, float Y, float Z);

	//Gets mouse movement
	void SetViewByMouse(); 

	void RotateAroundPoint(Vector3 vCenter, float X, float Y, float Z);
	void StrafeCamera(float speed, float Height);
	void MoveCamera(float speed, float Height);
	void CheckForMovement();

	void Update();
	void Look();
	//Camera position & view
	Vector3 m_vPosition;
	Vector3 m_vView;
private:
	//Up vector
	Vector3 m_vUpVector;		
	//Strafe vector
	Vector3 m_vStrafe;						
};
#endif