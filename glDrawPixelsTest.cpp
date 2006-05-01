#include <windows.h>
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <time.h>
#include <fstream>

CHAR szAppName[] = "glDrawPixelsTest";
HWND  ghWnd;
HDC   ghDC;
HGLRC ghRC;
 
const unsigned int WIDTH = 1024;
const unsigned int HEIGHT = 960;
static GLfloat* s_pixelRMap = new GLfloat[256];
static GLfloat* s_pixelGMap = new GLfloat[256];
static GLfloat* s_pixelBMap = new GLfloat[256];
unsigned char imgData[WIDTH * HEIGHT * 3];

const char* logFileName = "c:\\glDrawPixelsTest.log";
std::ofstream s_logFile;

LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM);
void createImage();
void createPixelMap();

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 
    s_logFile.open(logFileName);
    if (s_logFile.fail()) return false;

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
    if (!RegisterClass(&wndclass)) return false;
 
    ghWnd = CreateWindow (szAppName, "OpenGL Test", WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
				CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL); 
    if (!ghWnd) return false; 
 
    ShowWindow (ghWnd, nCmdShow); 
    UpdateWindow (ghWnd); 
 
	MSG msg;
	BOOL bRet;
	while((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
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
	double duration;

    switch (uMsg) 
    { 
 
    case WM_CREATE: 
        ghDC = GetDC(hWnd); 
		{
			PIXELFORMATDESCRIPTOR aSelectedPfd = 
			{   sizeof(PIXELFORMATDESCRIPTOR), 1, 
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
				24, // 24-bit color depth
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				16, // 32-bit z-buffer
				0, 0, 
				PFD_MAIN_PLANE, // main layer
				0, 0, 0, 0 };

			int aSelectedPfId = ChoosePixelFormat(ghDC, &aSelectedPfd); 
			SetPixelFormat(ghDC, aSelectedPfId, &aSelectedPfd);
		}
		createImage();
 
        ghRC = wglCreateContext(ghDC);
        wglMakeCurrent(ghDC, ghRC);
        GetClientRect(hWnd, &rect);
		
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClearDepth(1.0);

		glViewport(0, 0, rect.right, rect.bottom);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(0.0, rect.right-1, 0.0, rect.bottom-1, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
        break;
 
    case WM_PAINT: 
        BeginPaint(hWnd, &ps); 
		glClear(GL_COLOR_BUFFER_BIT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPushMatrix();
		glLoadIdentity();
		
		glRasterPos2d(0.0, 0.0);
		glPixelZoom(1.0, 1.0);

		clock_t start, finish;
		start = clock();
		glDrawPixels(static_cast<GLsizei>(WIDTH), static_cast<GLsizei>(HEIGHT), GL_RGB, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(imgData));
		finish = clock();
		duration = (double)(finish-start) / CLOCKS_PER_SEC;
		s_logFile << "glDrawPixels with 1024x960 image: " << duration << std::endl;

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
	    s_logFile.close();
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

void createImage()
{
	for(int i = 0; i < 960; i++) {
		for(int j = 0; j < 256; j++) {
			imgData[(i*1024*3) + (j*4*3)] = j;
			imgData[(i*1024*3) + (j*4*3) + 1] = j;
			imgData[(i*1024*3) + (j*4*3) + 2] = j;
			imgData[(i*1024*3) + (j*4*3) + 3] = j;
			imgData[(i*1024*3) + (j*4*3) + 4] = j;
			imgData[(i*1024*3) + (j*4*3) + 5] = j;
			imgData[(i*1024*3) + (j*4*3) + 6] = j;
			imgData[(i*1024*3) + (j*4*3) + 7] = j;
			imgData[(i*1024*3) + (j*4*3) + 8] = j;
			imgData[(i*1024*3) + (j*4*3) + 9] = j;
			imgData[(i*1024*3) + (j*4*3) + 10] = j;
			imgData[(i*1024*3) + (j*4*3) + 11] = j;
		}
	}
}
