#include "Header Files\Main.h"
#include "Header Files\Camera.h"

//Camera speed
#define camSpeed	100.0f									

void CalculateFrameRate()
{
	static int framesPerSecond = 0;				//Stores FPS
	static float fpsTime = 0.0f;				//Amount of elapsed time until we update the FPS count
	char strFrameRate[50] = {0};


	fpsTime += g_DT;


	if( fpsTime > 1.0f )
	{
		fpsTime = 0.0f;
		sprintf(strFrameRate, "Current Frames Per Second: %d", framesPerSecond);
		SetWindowText(g_hWnd, strFrameRate);
		framesPerSecond = 0;
	}
	else
	{
		++framesPerSecond;
	}
}

//**************************************************************//
//Cross Product
//**************************************************************//
Vector3 Cross(Vector3 vVector1, Vector3 vVector2)
{
	Vector3 vNormal;	

	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;										 
}

//**************************************************************//
//Magnitude of vector
//**************************************************************//
float Magnitude(Vector3 vNormal)
{
	//magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) V=vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}

//**************************************************************//
//Normalize the vector (returns a vector of length equal to 1
//**************************************************************//
Vector3 Normalize(Vector3 vVector)
{
	float magnitude = Magnitude(vVector);				 
	vVector = vVector / magnitude;		
	return vVector;										
}

//**************************************************************//
//Constructor
//**************************************************************//
Camera::Camera()
{
	/*
	vZero: Sets up a 0 vector for camera position
	vView: initial view vector
	vUp: up direction
	*/
	Vector3 vZero = Vector3(-200.0, 5.0, -200.0);
	Vector3 vView = Vector3(0.0, -1.0, 1.0);
	Vector3 vUp   = Vector3(0.0, 0.0, 1.0);

	m_vPosition	= vZero;
	m_vView		= vView;
	m_vUpVector	= vUp;
}


void Camera::PositionCamera(float positionX, float positionY, float positionZ,
							 float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	Vector3 vPosition	= Vector3(positionX, positionY, positionZ);
	Vector3 vView		= Vector3(viewX, viewY, viewZ);
	Vector3 vUpVector	= Vector3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView     = vView;
	m_vUpVector = vUpVector;
}

//**************************************************************//
//Capture mouse movement for 1st person camera
//**************************************************************//
void Camera::SetViewByMouse()
{
	POINT mousePos;
	int middleX = SCREEN_WIDTH  >> 1;
	int middleY = SCREEN_HEIGHT >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentRotX = 0.0f;
	
	//Get current X,Y position of the mouse
	GetCursorPos(&mousePos);
	
	//if no movement
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;
	SetCursorPos(middleX, middleY);							

	//get movement direction
	angleY = (float)( (middleX - mousePos.x) ) / 500.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 500.0f;		

	currentRotX -= angleZ;  
	if(currentRotX > 1.0f)
		currentRotX = 1.0f;
	else if(currentRotX < -1.0f)
		currentRotX = -1.0f;
	else
	{
		Vector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);

		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}
	RotateView(angleY, 0, 1, 0);
}

void Camera::RotateView(float angle, float x, float y, float z)
{
	Vector3 vNewView;
	
	//Direction the camera is facing
	Vector3 vView = m_vView - m_vPosition;		

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	//New X pos
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	//New Y pos
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	//New Z pos
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	m_vView = m_vPosition + vNewView;
}

void Camera::StrafeCamera(float speed, float Height)
{	
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;

}

void Camera::MoveCamera(float speed, float Height)
{
	//Direction the camera is facing
	Vector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vPosition.y += vVector.y * speed;
	m_vView.x += vVector.x * speed;
	m_vView.y += vVector.y * speed;
	m_vView.z += vVector.z * speed;
}

void Camera::CheckForMovement()
{	
	float speed = camSpeed * g_DT;
}

void Camera::Update() 
{
	Vector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);
	m_vStrafe = Normalize(vCross);

	//Binds the mouse to camera rotation
	//SetViewByMouse();

	//Check for keyboard input
	CheckForMovement();
	
	CalculateFrameRate();
}


void Camera::Look()
{
	//Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}