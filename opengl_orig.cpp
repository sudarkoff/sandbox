#include <windows.h>
#include <GL/gl.h> 
#include <GL/glu.h> 

CHAR szAppName[]="glDrawPixelsTest";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;
 
const unsigned int WIDTH = 512;
const unsigned int HEIGHT = 512;
static GLfloat* s_pixelRMap = new GLfloat[256];
static GLfloat* s_pixelGMap = new GLfloat[256];
static GLfloat* s_pixelBMap = new GLfloat[256];
unsigned char imgData[WIDTH * HEIGHT];

LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 
    WNDCLASS   wndclass; 
    wndclass.style         = 0; 
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc; 
    wndclass.cbClsExtra    = 0; 
    wndclass.cbWndExtra    = 0; 
    wndclass.hInstance     = hInstance; 
    wndclass.hIcon         = LoadIcon (hInstance, szAppName); 
    wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW); 
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wndclass.lpszMenuName  = szAppName; 
    wndclass.lpszClassName = szAppName; 
    if (!RegisterClass (&wndclass) ) return false; 
 
    ghWnd = CreateWindow (szAppName, "OpenGL Test", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
				CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL); 
    if (!ghWnd) return false; 
 
    ShowWindow (ghWnd, nCmdShow); 
    UpdateWindow (ghWnd); 
 
	MSG msg;
	BOOL bRet;
	while((bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{
		if (bRet == -1) break;
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

    return msg.wParam;
} 
 
LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    LONG    lRet = 1; 
    PAINTSTRUCT    ps; 
    RECT rect; 
	int i;

    switch (uMsg) 
    { 
 
    case WM_CREATE: 
        ghDC = GetDC(hWnd); 
		{
			PIXELFORMATDESCRIPTOR aSelectedPfd = {
				sizeof(PIXELFORMATDESCRIPTOR), 1, 
				  PFD_DRAW_TO_WINDOW            // support window 
				| PFD_SUPPORT_OPENGL            // support OpenGL 
				| PFD_DOUBLEBUFFER              // double buffered 
				| PFD_TYPE_RGBA                 // RGBA type 
				,
				24,                    // 24-bit color depth 
				0, 0, 0, 0, 0, 0,      // color bits ignored 
				0,                     // no alpha buffer 
				0,                     // shift bit ignored 
				0,                     // no accumulation buffer 
				0, 0, 0, 0,            // accum bits ignored 
				16,                    // 32-bit z-buffer     
				0,                     // no stencil buffer 
				0,                     // no auxiliary buffer 
				PFD_MAIN_PLANE,        // main layer 
				0,                     // reserved 
				0, 0, 0                // layer masks ignored 
			};

			int aSelectedPfId = ChoosePixelFormat(ghDC, &aSelectedPfd); 
			SetPixelFormat(ghDC, aSelectedPfId, &aSelectedPfd);
			
			// create image
			for(i = 0; i < WIDTH; i++)
				for(int j = 0; j < HEIGHT; j++)
					imgData[i*WIDTH+j] = j%256;
		}
 
        ghRC = wglCreateContext(ghDC);
        wglMakeCurrent(ghDC, ghRC);
        GetClientRect(hWnd, &rect);
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0);
    
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 
		glShadeModel(GL_FLAT);

		glViewport( 0, 0, rect.right, rect.bottom); 
		glMatrixMode( GL_PROJECTION ); 
		glLoadIdentity(); 
    
		glOrtho( 0.0, rect.right-1, 0.0, rect.bottom-1, -1.0,  1.0 );
		glMatrixMode( GL_MODELVIEW ); 

		for (i = 0; i < 256; ++i)
		{
			s_pixelRMap[i] = (float)(i / 255.0);
			s_pixelGMap[i] = (float)(i / 255.0);
			s_pixelBMap[i] = (float)(i / 255.0);
		}

		glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 256, s_pixelRMap);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 256, s_pixelGMap);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 256, s_pixelBMap);
//		glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
        break; 
 
    case WM_PAINT: 
        BeginPaint(hWnd, &ps); 
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glPushMatrix(); 
		glLoadIdentity();
		
		glRasterPos2d(0.0, 0.0);
		glPixelZoom(1.0, 1.0);
		glDrawPixels( static_cast<GLsizei>(WIDTH), static_cast<GLsizei>(HEIGHT), /*GL_RGB/*/GL_COLOR_INDEX/**/, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(imgData) );
		
		glPopMatrix();
		glFlush();

		SwapBuffers(ghDC);
        
		EndPaint(hWnd, &ps); 
        break; 
 
    case WM_CLOSE: 
        if (ghRC) wglDeleteContext(ghRC); 
        if (ghDC) ReleaseDC(hWnd, ghDC); 
        ghRC = 0; 
        ghDC = 0; 
 
        DestroyWindow (hWnd); 
        break; 
 
    case WM_DESTROY: 
        if (ghRC) wglDeleteContext(ghRC); 
        if (ghDC) ReleaseDC(hWnd, ghDC); 
 
        PostQuitMessage (0);
        break; 
     
    default: 
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet; 
} 
