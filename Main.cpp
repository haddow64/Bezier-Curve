#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <stdio.h>
#include "Header Files\Main.h"
#include "Header Files\resource.h"
#include "Header Files\Curve.h"
#include "Header Files\Camera.h"

//Camera Speed
#define SPEED	0.5f

bool	God = true;
bool	AtCruve = false;
float Height;

//**************************************************************//
//Global handles
//**************************************************************//
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;
//Time since last frame
float g_DT;

//**************************************************************//
//Skybox texture ID's
//**************************************************************//
#define BACK_ID		0
#define FRONT_ID	1
#define BOTTOM_ID	2
#define TOP_ID		3
#define LEFT_ID		4
#define RIGHT_ID	5
UINT g_Texture[MAX_TEXTURES] = {0};						//This holds the texture info, referenced by an ID

//**************************************************************//
//GL setup stuff
//**************************************************************//
GLfloat Light_Ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat Light_Position[]= {-50.0f, 0.0f, -50.0f, 1.0f};
GLfloat Light_Const[]= { 0.01f, 0.01f, 0.01f, 1.0f};
GLfloat Light_Linear[]= {0.02f, 0.02f, 0.02f, 1.0f};
GLfloat Light_Quad[]= {  0.0f, 0.0f, 0.0f, 1.0f};

//Camera object
Camera g_Camera;

//Curve object
Curve g_Curve;

BOOL MainDialog_OnCommand(HWND hWnd, WORD wCommand, WORD wNotify, HWND hControl)
{
		char number[20];
		int point;
		Vector3 Pos;
		bool check1, check2, check3, check4;

	switch (wCommand)
	{

	case IDOK:
		GetDlgItemText(hWnd, IDC_EDIT1, number, sizeof(number));
		sscanf(number, "%d", &point);
		GetDlgItemText(hWnd, IDC_EDIT2, number, sizeof(number));
		sscanf(number, "%f", &Pos.x);
		GetDlgItemText(hWnd, IDC_EDIT3, number, sizeof(number));
		sscanf(number, "%f", &Pos.y);
		GetDlgItemText(hWnd, IDC_EDIT4, number, sizeof(number));
		sscanf(number, "%f", &Pos.z);

		GetDlgItemText(hWnd, IDC_EDIT1, number, sizeof(number));
		if((!atoi(number))&&(atoi(number)>0)&&(atoi(number)<=7))
		{
			check1=false;
			MessageBox(hWnd, "Please choose a CP (1-7)", "Nope!", MB_ICONEXCLAMATION);
		}else{
			check1=true;
		}

		GetDlgItemText(hWnd, IDC_EDIT2, number, sizeof(number));
		if((!atof(number)&&(atoi(number) !=0)))
		{
			check2=false;
			MessageBox(hWnd, "Input X value", "Nope!", MB_ICONEXCLAMATION);
		}else{
			check2=true;
		}

		GetDlgItemText(hWnd, IDC_EDIT3, number, sizeof(number));
		if((!atof(number)&&(atoi(number) !=0)))
		{
			check3=false;
			MessageBox(hWnd, "Input Y value", "Nope!", MB_ICONEXCLAMATION);
		}else{
			check3=true;
		}

		GetDlgItemText(hWnd, IDC_EDIT4, number, sizeof(number));
		if((!atof(number)&&(atoi(number) !=0)))
		{
			check4=false;
			MessageBox(hWnd, "Input Z value", "Nope!", MB_ICONEXCLAMATION);
		}else{
			check4=true;
		}

		if((check1)&&(check2)&&(check3)&&(check4)){
		point--;
		g_Curve.MoveCP(point, Pos);
		EndDialog(hWnd, wCommand);
		}
		break;

	case IDCANCEL:
		EndDialog(hWnd, wCommand);
		break;
	}
	return TRUE;
}

BOOL MainDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	  case WM_COMMAND:
		return MainDialog_OnCommand(hWnd, LOWORD(wParam), HIWORD(wParam), (HWND)lParam);


	  case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}

//**************************************************************//
//Skybox
//**************************************************************//
void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	glBindTexture(GL_TEXTURE_2D, g_Texture[BACK_ID]);

	//centre skybox
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	//back
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
	glEnd();

	//front
	glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}

//**************************************************************//
//Init game window
//**************************************************************//
void Init(HWND hWnd)
{
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);


	//Loading in of .bmp's for the skybox
	CreateTexture(g_Texture, "textures/alpine_south.bmp",		BACK_ID		);
	CreateTexture(g_Texture, "textures/alpine_north.bmp",		FRONT_ID	);
	CreateTexture(g_Texture, "textures/alpine_down.bmp",		BOTTOM_ID	);
	CreateTexture(g_Texture, "textures/alpine_up.bmp",			TOP_ID		);
	CreateTexture(g_Texture, "textures/alpine_east.bmp",		LEFT_ID		);
	CreateTexture(g_Texture, "textures/alpine_west.bmp",		RIGHT_ID	);

	//Camera Starting pos
	g_Camera.PositionCamera( 80, 40, 80, 0, 20, -10, 0, 1, 0);
}


bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = GetTickCount() * 0.001f;
	static float elapsedTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	float deltaTime = currentTime - lastTime;
	float desiredFPS = 1.0f / desiredFrameRate;

	elapsedTime += deltaTime;
	lastTime = currentTime;

	if( elapsedTime > desiredFPS )
	{
		g_DT = desiredFPS;
		elapsedTime -= desiredFPS;
		return true;
	}
	return false;
}

//**************************************************************//
//Main loop
//**************************************************************//
WPARAM MainLoop()
{
	MSG msg;

	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)	
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
				//update the camera
				g_Camera.Update();
				//Draw the scene
				RenderScene();
		}
	}
	ShutdownOGL();
	return(msg.wParam);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear The Screen And The Depth Buffer
	glLoadIdentity();									//Reset The matrix

	// Give OpenGL our camera position
	g_Camera.Look();

	// Here we create our sky box.  It will be centred around (0, 0, 0) with
	// a width of 400, height of 200 and length of 400.  You might wonder why we
	// don't make it a perfect cube?  Well, if we make the height half the size
	// of the width and length it looks better.  Try putting the height to 400 and
	// you will notice that everything looks really close up.  You will want to tweak
	// this ratio most likely for every sky maps you use.  Some look better at different
	// perspectives and ratios.  Try changing the height to 100.  It looks more flat
	// and at a greater distance.

	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Light_Const);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, Light_Linear);
	glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Light_Quad);
	glEnable(GL_LIGHT1);

	//Draw the skybox
	glDisable(GL_LIGHTING);
	CreateSkyBox(g_Camera.m_vPosition.x, g_Camera.m_vPosition.y, g_Camera.m_vPosition.z, 400, 200, 400);
	g_Curve.MoveSphere();
	g_Curve.Draw();
	glEnable(GL_LIGHTING);
	if(AtCruve&&God){
		g_Camera.SetViewByMouse();
	}
	if(!God){
	g_Camera.PositionCamera(g_Curve.SpherePos.x, g_Curve.SpherePos.y+5, g_Curve.SpherePos.z,
		g_Curve.SpherePos.x+g_Curve.SphereDir.x, g_Curve.SpherePos.y+g_Curve.SphereDir.y, g_Curve.SpherePos.z+g_Curve.SphereDir.z,
							1,	0,	0);
	}else{
		g_Curve.DrawCurve();
	}

	SwapBuffers(g_hDC);
}

//**************************************************************//
//Handles windows msgs
//**************************************************************//
LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;
	int wmId, wmEvent;

	switch (uMsg)
	{
	case WM_COMMAND:
	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	switch (wmId)
	{
	case ID_EDIT_RESETPOINTS:
		g_Curve.Initialise();
		break;
	case ID_MENU_QUIT:
		PostQuitMessage(0);
		break;
	case ID_EDIT_POINTS:
		DialogBoxParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_DIALOG1), NULL,
		 (DLGPROC) MainDialogProc,0);

		break;
	case ID_VIEW_FREEROAMING:
		God = true;
		break;
	case ID_VIEW_FOLLOWCURVE:
		God = false;
		break;
	case ID_HELP_ABOUT:
		MessageBox(NULL,"Bezier Curve Coursework - Graeme Haddow(0600720)", "Bezier Curve Coursework",MB_OK | MB_ICONINFORMATION);
		break;
	}

	break;

	case WM_SIZE:
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));
			GetClientRect(hWnd, &g_rRect);
		break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		switch(wParam) {

			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case 'W':
				if(God)
				g_Camera.MoveCamera(SPEED, Height);
				break;
			case 'S':
				if(God)
				g_Camera.MoveCamera(-SPEED, Height);
				break;
			case 'A':
				if(God)
				g_Camera.StrafeCamera(-SPEED, Height);
				break;
			case 'D':
				if(God)
				g_Camera.StrafeCamera(SPEED, Height);
				break;
				}
				break;

	case WM_LBUTTONDOWN:
	AtCruve = true;
	break;
	case WM_LBUTTONUP:
	AtCruve = false;
	break;

	case WM_DESTROY:									//If the window is destroyed
		PostQuitMessage(0);								//Send a QUIT Message to the window
		break;

	default:
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
		break;
	}
	return lRet;
}