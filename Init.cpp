#include "Header Files\Main.h"
#include "Header Files\resource.h"

//**************************************************************//
//Initialisation Stuff
//Windows & OpenGL
//**************************************************************//


//**************************************************************//
//Create Window
//**************************************************************//
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, HINSTANCE hInstance)
{
	HWND hWnd;
	WNDCLASS wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASS));	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wndclass.lpszClassName = "Bezier Curve Coursework";
	wndclass.lpszMenuName  = (LPCSTR) IDR_MENU1; 

	RegisterClass(&wndclass);

	if(!dwStyle)
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	g_hInstance = hInstance;

	RECT rWindow;
	rWindow.left	= 50;
	rWindow.right	= width;
	rWindow.top	    = 50;
	rWindow.bottom	= height;

	AdjustWindowRect( &rWindow, dwStyle, false);
												
	hWnd = CreateWindow("Bezier Curve Coursework", strWindowName, dwStyle, 0, 0,
						rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top, 
						NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);	
	SetFocus(hWnd);
	return hWnd;
}

//**************************************************************//
//Register the window
//**************************************************************//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{	
	HWND hWnd;
	hWnd = CreateMyWindow("Bezier Curve Coursework", SCREEN_WIDTH, SCREEN_HEIGHT, 0, hInstance);

	if(hWnd == NULL) return TRUE;
	Init(hWnd);													

	return MainLoop();						
}

//**************************************************************//
//OpenGL Initialisation
//**************************************************************//
void InitializeOpenGL(int width, int height) 
{  
	g_hDC = GetDC(g_hWnd);
	if (!bSetupPixelFormat(g_hDC))
		PostQuitMessage (0);

	g_hRC = wglCreateContext(g_hDC);					//Rendering context from hdc
	wglMakeCurrent(g_hDC, g_hRC);						//Sets rendering context created above to in use
	glEnable(GL_TEXTURE_2D);							//Texture mapping
	glEnable(GL_DEPTH_TEST);							//Depth Testing

	SizeOpenGLScreen(width, height);
}

//**************************************************************//
//Releases OpenGL & posts quit message
//**************************************************************//
void ShutdownOGL()
{
	if (g_hRC)											
	{
		wglMakeCurrent(NULL, NULL);						//This frees our rendering memory and sets everything back to normal
		wglDeleteContext(g_hRC);						//Delete our OpenGL Rendering Context	
	}
	
	if (g_hDC) 
		ReleaseDC(g_hWnd, g_hDC);						//Release our HDC from memory

	UnregisterClass("Bezier Curve Coursework", g_hInstance);		//Free the window class
	PostQuitMessage (0);								//Post a QUIT message to the window
}

//Creates OpenGL texture
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	FILE *pFile = NULL;

	if(!strFileName)
		return;
	pFile = fopen(strFileName,"r");
	if(pFile){
		pBitmap = auxDIBImageLoad(strFileName);}
	else{												
		MessageBox(NULL, "Couldn't find a texture!", "Error!", MB_OK);
		exit(0);}

	//Generate the texture
	glGenTextures(1, &textureArray[textureID]);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (pBitmap)
	{
		if (pBitmap->data)
		{
			free(pBitmap->data);
		}
		free(pBitmap);
	}
}

//**************************************************************//
//Sets up pixel format for OpenGL
//**************************************************************//
bool bSetupPixelFormat(HDC hdc) 
{ 
	PIXELFORMATDESCRIPTOR pfd = {0}; 
	int pixelformat; 
 
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			//Set the size of the structure
	pfd.nVersion = 1;									//Always set this to 1
														//Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	pfd.dwLayerMask = PFD_MAIN_PLANE;					//We want the standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;						//We want RGB and Alpha pixel type
	pfd.cColorBits = SCREEN_DEPTH;						//Here we use our #define for the colour bits
	pfd.cDepthBits = SCREEN_DEPTH;						//Depthbits is ignored for RGBA, but we do it anyway
	pfd.cAccumBits = 0;									//No special bitplanes needed
	pfd.cStencilBits = 0;								//We desire no stencil bits
 
	//This gets us a pixel format that best matches the one passed in from the device
	if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE ) 
	{ 
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
		return FALSE; 
	} 
 
	//This sets the pixel format that we extracted from above
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) 
	{ 
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
		return FALSE; 
	} 
	return TRUE;										//Return a success!
}

//**************************************************************//
//Resize OpenGL window
//**************************************************************//
void SizeOpenGLScreen(int width, int height)			//Initialize The GL Window
{
	if (height==0)										//Prevent A Divide By Zero error
	{
		height=1;										//Make the Height Equal One
	}
	/*Make our viewport the whole window
	We could make the view smaller inside
	Our window if we wanted too.
	The glViewport takes (x, y, width, height)
	This basically means, what are our drawing boundaries*/
	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  the closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  the farthest distance before it stops drawing).
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}