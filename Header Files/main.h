#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GL\GL.H";
#include "GL\GLAux.h";
#include "GL\GLU.H"

//Stops VS giving warning C4996
#define _CRT_SECURE_NO_DEPRECATE

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800
#define SCREEN_DEPTH 16	

#define MAX_TEXTURES 50
//For the skybox
#define GL_CLAMP_TO_EDGE	0x812F


extern HWND  g_hWnd;										// This is the handle for the window
extern RECT  g_rRect;										// This holds the window dimensions
extern HDC   g_hDC;											// General HDC - (handle to device context)
extern HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
extern HINSTANCE g_hInstance;								// This holds our window hInstance
extern float g_DT;											// Global float that stores the elapsed time between the current and last frame


//**************************************************************//
//Windows setup
//**************************************************************//
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, HINSTANCE hInstance);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// This initializes the whole program
void Init(HWND hWnd);

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WPARAM MainLoop();


//**************************************************************//
//OpenGL Functions
//**************************************************************//
void InitializeOpenGL(int width, int height);
void ShutdownOGL();
void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID);
bool bSetupPixelFormat(HDC hdc);
void SizeOpenGLScreen(int width, int height);

//Draws the scene
void RenderScene();
void WalkAround();

#endif