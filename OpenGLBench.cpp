#include <windows.h>
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <crtdbg.h>
#include <cmath>
#include <fstream>

CHAR szAppName[]="Win OpenGL"; 
HWND  ghWnd; 
HDC   ghDC; 
HGLRC ghRC; 
 
#define WIDTH           487 
#define HEIGHT          487 
 
LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM); 
BOOL bSetupPixelFormat(HDC); 
 
const unsigned int IterCount = 10;

struct TestCaseInfo
{
    char*   description;
    GLenum  pixelFormat;
    unsigned short byteCount;
    bool    pixelZoom;
    bool    withTexture;
    bool    testValid;
    double  results[IterCount];
};

/* OpenGL globals, defines, and prototypes */ 
GLvoid resize(GLsizei, GLsizei); 
GLvoid initializeGL(GLsizei, GLsizei); 
GLvoid drawScene( TestCaseInfo*, unsigned int ); 

LARGE_INTEGER PerformanceFrequency;

TestCaseInfo theTestCases[] =
{
{ "Rgb, no zoom, textured"  , GL_RGB    , 3, false, true  , true  },
{ "Rgb, zoom, textured"     , GL_RGB    , 3,  true, true  , true  },
{ "Bgr, no zoom, textured"  , GL_BGR_EXT, 3, false, true  , true  },
{ "Bgr, zoom, textured"     , GL_BGR_EXT, 3,  true, true  , true  },
{ "Rgba, no zoom, textured" , GL_RGBA   , 4, false, true  , true  },
{ "Rgba, zoom, textured."   , GL_RGBA   , 4,  true, true  , true  },
{ "Bgra, no zoom, textured" , GL_BGRA_EXT,4, false, true  , true  },
{ "Bgra, zoom, textured"    , GL_BGRA_EXT,4,  true, true  , true  },
{ "Rgb, no zoom"            , GL_RGB    , 3, false, false , true  },
{ "Rgb, zoom"               , GL_RGB    , 3,  true, false , true  },
{ "Bgr, no zoom"            , GL_BGR_EXT, 3, false, false , true  },
{ "Bgr, zoom"               , GL_BGR_EXT, 3,  true, false , true  },
{ "Rgba, no zoom"           , GL_RGBA   , 4, false, false , true  },
{ "Rgba, zoom"              , GL_RGBA   , 4,  true, false , true  },
{ "Bgra, no zoom"           , GL_BGRA_EXT,4, false, false , true  },
{ "Bgra, zoom"              , GL_BGRA_EXT,4,  true, false , true  },
{ "" }
};

const char * logFileName = "c:\\temp\\OpenGlBench.log";
std::ofstream logStream;

unsigned char rgbImage[WIDTH * HEIGHT * 3 * 2];
unsigned char rgbaImage[WIDTH * HEIGHT * 4 * 2];

GLuint rgbTextureName;
GLuint rgbaTextureName;
const double texRatioW = (double)WIDTH / 512.0;
const double texRatioH = (double)HEIGHT/ 512.0;

void buildImage()
{
    const double xCenter = (double)WIDTH/2.0;
    const double yCenter = (double)HEIGHT/2.0;
    const double hDiagV  
        = 255.0 
        / (xCenter*xCenter + yCenter*yCenter);

    double x=0.0;
    double y=0.0;
    unsigned char r, g, b;

    for ( unsigned int i=0; i<WIDTH; ++i )
    {
        for ( unsigned int j=0; j<HEIGHT; ++j )
        {
            x=(double)i-xCenter;
            y=(double)j-yCenter;

            r = ((x<0.0)&&(y<0.0))
                ? static_cast<unsigned char>((x*x+y*y) * hDiagV) 
                : 0;
            g = ((x<0.0)&&(y>0.0))
                ? static_cast<unsigned char>((x*x+y*y) * hDiagV) 
                : 0;
            b = ((x>0.0)&&(y<0.0)) 
                ? static_cast<unsigned char>((x*x+y*y) * hDiagV) 
                : 0;

            rgbImage[(j*HEIGHT+i)*3]   
                = rgbaImage[(j*HEIGHT+i)*4]   
                = r;

            rgbImage[(j*HEIGHT+i)*3+1] 
                = rgbaImage[(j*HEIGHT+i)*4+1] 
                = g;

            rgbImage[(j*HEIGHT+i)*3+2] 
                = rgbaImage[(j*HEIGHT+i)*4+2] 
                = b;

            rgbaImage[(j*HEIGHT+i)*4+3] 
                = 255;

            rgbImage[(WIDTH * HEIGHT * 3) + (j*HEIGHT+i)*3]   
                = rgbaImage[(WIDTH * HEIGHT * 4) + (j*HEIGHT+i)*4]   
                = r;

            rgbImage[(WIDTH * HEIGHT * 3) + (j*HEIGHT+i)*3+1] 
                = rgbaImage[(WIDTH * HEIGHT * 4) + (j*HEIGHT+i)*4+1] 
                = g;

            rgbImage[(WIDTH * HEIGHT * 3) + (j*HEIGHT+i)*3+2] 
                = rgbaImage[(WIDTH * HEIGHT * 4) + (j*HEIGHT+i)*4+2] 
                = b;

            rgbaImage[(WIDTH * HEIGHT * 4) + (j*HEIGHT+i)*4+3] = 255;

        }
    }
}
 
bool analyzeOpenGlError( )
{
    const GLenum err = glGetError();

    switch ( err )
    {
    case GL_NO_ERROR :
        return true;
    case GL_INVALID_ENUM :
        _ASSERT( false );
        return false;
    case GL_INVALID_VALUE :
        _ASSERT( false );
        return false;
    case GL_INVALID_OPERATION :
        _ASSERT( false );
        return false;
    case GL_STACK_OVERFLOW :
        _ASSERT( false );
        return false;
    case GL_STACK_UNDERFLOW :
        _ASSERT( false );
        return false;
    case GL_OUT_OF_MEMORY :
        _ASSERT( false );
        return false;
    default:
        _ASSERT( false );
        return false;
    };
}

int WINAPI WinMain (
                    HINSTANCE hInstance, 
                    HINSTANCE hPrevInstance, 
                    LPSTR lpCmdLine, 
                    int nCmdShow) 
{ 
    if ( FALSE == QueryPerformanceFrequency( &PerformanceFrequency ) )
    {
        return false;
    }

    logStream.open( logFileName );
    if ( logStream.fail() )
    {
        return FALSE;
    }

    MSG        msg; 
    WNDCLASS   wndclass; 
 
    /* Register the frame class */ 
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
 
    if (!RegisterClass (&wndclass) ) 
    {
        return false; 
    }
 
    /* Create the frame */ 
    ghWnd = CreateWindow (szAppName, 
             "Generic OpenGL Sample", 
         WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
             CW_USEDEFAULT, 
             CW_USEDEFAULT, 
             WIDTH, 
             HEIGHT, 
             NULL, 
             NULL, 
             hInstance, 
             NULL); 
 
    /* make sure window was created */ 
    if (!ghWnd) 
    {
        return false; 
    }
 
    /* show and update main window */ 
    ShowWindow (ghWnd, nCmdShow); 
 
    UpdateWindow (ghWnd); 
 
    /* animation loop */ 
    while (1) 
    { 
        using std::endl;
        /* 
         *  Process all pending messages 
         */ 
 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } 
            else 
            { 
                break;
            } 
        } 

        // protect against empty tc list
        static unsigned int i=0;
        static TestCaseInfo* currentTestCase = &theTestCases[0];

        if ( i<IterCount )
        {
            if ( strlen( (currentTestCase)->description ) != 0 )
            {
                drawScene( currentTestCase++, i ); 
            }
            else
            {
                currentTestCase = &theTestCases[0];
                ++i;
            }
        }
        else
        {
            for ( 
                currentTestCase = &theTestCases[0]; 
                strlen(currentTestCase->description) != 0; 
                ++currentTestCase )
            {
                logStream 
                    << "Test case : " 
                    << currentTestCase->description 
                    << endl;

                if ( currentTestCase->testValid )
                {
                    double Total = 0.0;
                    double min = 100000000000.0;
                    double max = -1.0;

                    for ( unsigned int j=0; j<IterCount;++j )
                    {
                        if ( currentTestCase->results[j] < min )
                        {
                            min = currentTestCase->results[j];
                        }

                        if ( currentTestCase->results[j] > max )
                        {
                            max = currentTestCase->results[j];
                        }

                        Total += currentTestCase->results[j];
                    }

                    logStream 
                        << "   result : mean( " 
                        << Total / (double)IterCount << " ) min ( " 
                        << min << " ) max ( " 
                        << max << " )" << endl;
                }
                else
                {
                    logStream << "   invalid test." << endl;
                }
            }
            break;
        }
    } 

    logStream.close();
    return true;
} 
 
/* main window procedure */ 
LONG WINAPI MainWndProc ( 
    HWND    hWnd, 
    UINT    uMsg, 
    WPARAM  wParam, 
    LPARAM  lParam) 
{ 
    LONG    lRet = 1; 
    PAINTSTRUCT    ps; 
    RECT rect; 

    switch (uMsg) 
    { 
 
    case WM_CREATE: 
        ghDC = GetDC(hWnd); 
        if (!bSetupPixelFormat(ghDC)) 
        {
            PostQuitMessage (0); 
        }
 
        ghRC = wglCreateContext(ghDC); 
        wglMakeCurrent(ghDC, ghRC); 
        {
            const GLubyte* const aVendor 
                = glGetString( GL_VENDOR );
            const GLubyte* const aRenderer 
                = glGetString( GL_RENDERER );
            const GLubyte* const aVersion 
                = glGetString(GL_VERSION );
            
            using std::endl;
            using std::flush;
            logStream << "Current gl driver: " << endl
                <<       "   vendor    : " << aVendor << endl
                <<       "   renderer  : " << aRenderer << endl
                <<       "   version   : " << aVersion << endl <<
flush;
        }
        GetClientRect(hWnd, &rect); 
        initializeGL(rect.right, rect.bottom); 
        break; 
 
    case WM_PAINT: 
        BeginPaint(hWnd, &ps); 
        EndPaint(hWnd, &ps); 
        break; 
 
    case WM_SIZE: 
        GetClientRect(hWnd, &rect); 
        resize(rect.right, rect.bottom); 
        break; 
 
    case WM_CLOSE: 
        if (ghRC) 
        {
            wglDeleteContext(ghRC); 
        }
        if (ghDC) 
        {
            ReleaseDC(hWnd, ghDC); 
        }
        ghRC = 0; 
        ghDC = 0; 
 
        DestroyWindow (hWnd); 
        break; 
 
    case WM_DESTROY: 
        if (ghRC) 
        {
            wglDeleteContext(ghRC); 
        }
        if (ghDC) 
        {
            ReleaseDC(hWnd, ghDC); 
        }
 
        PostQuitMessage (0); 
        break; 
     
    default: 
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet; 
} 

unsigned int RatePixelFormat( const PIXELFORMATDESCRIPTOR& pd_in )
{
    if (   ( 0 == ( pd_in.dwFlags & PFD_SUPPORT_OPENGL ) )
        || ( 0 == ( pd_in.dwFlags & PFD_DRAW_TO_WINDOW ) )
        || ( 0 == ( pd_in.dwFlags & PFD_DOUBLEBUFFER   ) )
        || ( pd_in.cDepthBits <= 8 ) 
        || ( pd_in.iPixelType != PFD_TYPE_RGBA )
        )
    {
        return 0;
    }
    
    unsigned int score = 1;

    score += pd_in.cDepthBits;

    /*

    mcd :     PFD_GENERIC_FORMAT and     PFD_GENERIC_ACCELERATED
    soft:     PFD_GENERIC_FORMAT but not PFD_GENERIC_ACCELERATED
    icd : not PFD_GENERIC_FORMAT and not PFD_GENERIC_ACCELERATED
    */  
    if ( pd_in.dwFlags & PFD_GENERIC_FORMAT )
    {
        if ( pd_in.dwFlags & PFD_GENERIC_ACCELERATED )
        {
            // mcd
            score += 512;
        }
        else
        {
            // soft
        }
    }
    else
    {
        // icd
        _ASSERT( !(pd_in.dwFlags & PFD_GENERIC_ACCELERATED) );
        score += 1024;
    }
    
    return score;
}
 
std::ostream& operator<<( std::ostream& s, const
PIXELFORMATDESCRIPTOR& p )
{
    using std::endl;

    s   << "Pixel format descriptor:" << endl
        << "------------------------" << endl
        << " Flags                  " << endl
        << "    PFD_DRAW_TO_WINDOW       : " 
        << ((p.dwFlags&PFD_DRAW_TO_WINDOW)      ? "yes":"no") << endl
        << "    PFD_SUPPORT_OPENGL       : " 
        << ((p.dwFlags&PFD_SUPPORT_OPENGL)      ? "yes":"no") << endl
        << "    PFD_DOUBLEBUFFER         : " 
        << ((p.dwFlags&PFD_DOUBLEBUFFER)        ? "yes":"no") << endl
        << "    PFD_GENERIC_FORMAT       : " 
        << ((p.dwFlags&PFD_GENERIC_FORMAT)      ? "yes":"no") << endl
        << "    PFD_GENERIC_ACCELERATED  : " 
        << ((p.dwFlags&PFD_GENERIC_ACCELERATED) ? "yes":"no") << endl
        << endl
        << "    PFD_SUPPORT_GDI          : " 
        << ((p.dwFlags&PFD_SUPPORT_GDI)         ? "yes":"no") << endl
        << "    PFD_DRAW_TO_BITMAP       : " 
        << ((p.dwFlags&PFD_DRAW_TO_BITMAP)      ? "yes":"no") << endl
        << "    PFD_NEED_PALETTE         : " 
        << ((p.dwFlags&PFD_NEED_PALETTE)        ? "yes":"no") << endl
        << "    PFD_NEED_SYSTEM_PALETTE  : " 
        << ((p.dwFlags&PFD_NEED_SYSTEM_PALETTE) ? "yes":"no") << endl
        << "    PFD_STEREO               : " 
        << ((p.dwFlags&PFD_STEREO)              ? "yes":"no") << endl
        << "    PFD_SWAP_LAYER_BUFFERS   : " 
        << ((p.dwFlags&PFD_SWAP_LAYER_BUFFERS)  ? "yes":"no") << endl
        << endl
        << " Pixel type     : " << ((p.iPixelType==PFD_TYPE_RGBA)
            ?"PFD_TYPE_RGBA":
            ((p.iPixelType==PFD_TYPE_COLORINDEX)
                ?"PFD_TYPE_COLORINDEX":"Unknown")) << endl
        << " Color bits     : " << (int)p.cColorBits << " ( " 
            << (int)p.cRedBits << " " << (int)p.cGreenBits 
            << " " << (int) p.cBlueBits << " )" << endl
        << " Depth bits     : " << (int)p.cDepthBits << endl;
    return s;
}

BOOL bSetupPixelFormat(HDC hdc) 
{ 
    unsigned int aSelectedScore = 0;
    int aSelectedPfId = 0;

    PIXELFORMATDESCRIPTOR aSelectedPfd = 
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
        1,                              // version number 
          PFD_DRAW_TO_WINDOW            // support window 
        | PFD_SUPPORT_OPENGL            // support OpenGL 
        | PFD_DOUBLEBUFFER              // double buffered 
        | PFD_TYPE_RGBA                 // RGBA type 
        | PFD_DEPTH_DONTCARE            // don't need depth
        ,
        24,                    // 24-bit color depth 
        0, 0, 0, 0, 0, 0,      // color bits ignored 
        0,                     // no alpha buffer 
        0,                     // shift bit ignored 
        0,                     // no accumulation buffer 
        0, 0, 0, 0,            // accum bits ignored 
        0,                     // 32-bit z-buffer     
        0,                     // no stencil buffer 
        0,                     // no auxiliary buffer 
        PFD_MAIN_PLANE,        // main layer 
        0,                     // reserved 
        0, 0, 0                // layer masks ignored 
    };

    const int aPixelFormatCount = ::DescribePixelFormat( hdc, 1, 0, 0
);
    if ( 0 == aPixelFormatCount )
    {
        return FALSE;
    }

    {
        PIXELFORMATDESCRIPTOR aPfd;
        
        using std::endl;
        for ( int aPfId = 1; aPfId <= aPixelFormatCount; aPfId++ )
        {
            ::DescribePixelFormat( 
                hdc, 
                aPfId, 
                sizeof( PIXELFORMATDESCRIPTOR ), 
                &aPfd );
            const unsigned int aScore = RatePixelFormat( aPfd );
            logStream 
                << "Rating pf with id " 
                << aPfId 
                << " gave " 
                << aScore << endl;

            logStream << std::endl << aPfd << std::endl;

            if ( aScore > aSelectedScore )
            {
                aSelectedPfId = aPfId;
                aSelectedScore = aScore;
                aSelectedPfd = aPfd;
            }
        }
    }

    if ( 0 == aSelectedPfId )
    { 
        MessageBox(NULL, "Selecting a PF failed", "Error", MB_OK); 
        return false; 
    } 

    logStream 
        << std::endl << "Choosen pf id : " 
        << aSelectedPfId << "(scoring " << aSelectedScore << ")" 
        << std::endl;
    logStream << std::endl << aSelectedPfd << std::endl;

    if (SetPixelFormat(hdc, aSelectedPfId, &aSelectedPfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true; 
} 
 
/* OpenGL code */ 
 
GLvoid resize( GLsizei width, GLsizei height ) 
{ 
    glViewport( 0, 0, width, height ); 
 
    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity(); 
    glOrtho( 0.0, width-1, 0.0, height-1, -1.0,  1.0 );
    _ASSERT(analyzeOpenGlError());

    glMatrixMode( GL_MODELVIEW ); 
}     
 
GLvoid initializeGL(GLsizei width, GLsizei height) 
{ 
    glClearColor( 0.0, 0.0, 0.0, 0.0); 
    glClearDepth( 1.0 ); 

    glPixelStorei( GL_UNPACK_ALIGNMENT , 1 );

    glGenTextures( 1, &rgbTextureName );
    glBindTexture( GL_TEXTURE_2D, rgbTextureName );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
#if 0 // set to one to have linear.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
);
#else
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
);
#endif
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, 
        GL_RGB, GL_UNSIGNED_BYTE, rgbImage );
    _ASSERT(analyzeOpenGlError());

    glGenTextures( 1, &rgbaTextureName );
    glBindTexture( GL_TEXTURE_2D, rgbaTextureName );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
#if 0 // set to one to have linear.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
);
#else
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST
);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
);
#endif
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, rgbaImage );
    _ASSERT(analyzeOpenGlError());
    
    glDisable(GL_DEPTH_TEST); 
    glDisable( GL_DITHER );

    buildImage();
    resize( width, height );
} 
 
GLvoid drawScene( TestCaseInfo* const tc, const unsigned int iter ) 
{ 
    if ( false == tc->testValid )
    {
        return;
    }

    LARGE_INTEGER startCounter;
    LARGE_INTEGER stopCounter;

    QueryPerformanceCounter( &startCounter );

    for (int i=0 ; i< 10 ; i++) 
    {
        glClear( GL_COLOR_BUFFER_BIT ); 
        glPushMatrix(); 
        glLoadIdentity();
        
        _ASSERT(analyzeOpenGlError());
        
        if ( false == tc->withTexture )
        {
            glRasterPos2d(0.0, 0.0);
            _ASSERT(analyzeOpenGlError());
            
            if ( tc->pixelZoom )
            {
                glPixelZoom( 1.01f, 1.06f );
            }
            else
            {
                glPixelZoom( 1.0f, 1.0f );
            }
            
            GLboolean aValidRasterPos = GL_FALSE;
            glGetBooleanv( 
                GL_CURRENT_RASTER_POSITION_VALID, 
                &aValidRasterPos );
            _ASSERT( GL_TRUE == aValidRasterPos );
            
            if ( tc->byteCount == 3 )
            {
                glDrawPixels( static_cast<GLsizei>( WIDTH ), static_cast<GLsizei>( HEIGHT ), tc->pixelFormat, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(rgbImage + 3*iter*(WIDTH + 1)) );
            }
            else if ( tc->byteCount == 4 )
            {
                glDrawPixels( static_cast<GLsizei>( WIDTH ), static_cast<GLsizei>( HEIGHT ), tc->pixelFormat, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(rgbaImage + 4*iter*(WIDTH + 1)) );
            }
            
            _ASSERT( analyzeOpenGlError() );
        }
        else
        {
            glEnable( GL_TEXTURE_2D );
            
            float quadCoordX = WIDTH-1;
            float quadCoordY = HEIGHT-1;
            
            if ( tc->pixelZoom )
            {
                quadCoordX *= 1.01f;
                quadCoordY *= 1.06f;
            }
            
            if ( tc->byteCount == 3 )
            {
                glBindTexture( GL_TEXTURE_2D, rgbTextureName );
                
                glTexSubImage2D( 
                    GL_TEXTURE_2D, 0, 
                    0, 0, WIDTH, HEIGHT, 
                    tc->pixelFormat, GL_UNSIGNED_BYTE, 
                    static_cast<const GLvoid*>(rgbImage + (tc->byteCount)*iter*(WIDTH + 1))
                    );
                _ASSERT( analyzeOpenGlError() );
                
                glBegin( GL_QUADS );
                {
                    glTexCoord2f   ( 0.0, 0.0 );
                    glVertex3f     ( 0.0, 0.0, 0.0 );
                    glTexCoord2f   ( texRatioW, 0.0 );
                    glVertex3f     ( quadCoordX, 0.0, 0.0 );
                    glTexCoord2f   ( texRatioW, texRatioH );
                    glVertex3f     ( quadCoordX, quadCoordY, 0.0 );
                    glTexCoord2f   ( 0.0, texRatioH );
                    glVertex3f     ( 0.0, quadCoordY, 0.0 );
                }
                glEnd();
            }
            else if ( tc->byteCount == 4 )
            {
                glBindTexture( GL_TEXTURE_2D, rgbaTextureName );
                glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, tc->pixelFormat, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(rgbaImage + (tc->byteCount)*iter*(WIDTH + 1)) );
                _ASSERT( analyzeOpenGlError() );
                
                glBegin( GL_QUADS );
                {
                    glTexCoord2f   ( 0.0, 0.0 );
                    glVertex3f     ( 0.0, 0.0, 0.0 );
                    glTexCoord2f   ( texRatioW, 0.0 );
                    glVertex3f     ( WIDTH-1, 0.0, 0.0 );
                    glTexCoord2f   ( texRatioW, texRatioH );
                    glVertex3f     ( WIDTH-1, HEIGHT-1, 0.0 );
                    glTexCoord2f   ( 0.0, texRatioH );
                    glVertex3f     ( 0.0, HEIGHT-1, 0.0 );
                }
                glEnd();
            }
            
            glDisable( GL_TEXTURE_2D );
        }
        
        if ( false == analyzeOpenGlError() )
        {
            tc->testValid = false;
        }
        
        glPopMatrix();
        
        SwapBuffers(ghDC);
        
        
    }
    QueryPerformanceCounter( &stopCounter );
    
    const LONGLONG aDiff = stopCounter.QuadPart - startCounter.QuadPart;
    const double aMilliSecCount = static_cast<double>( aDiff ) / static_cast<double>( PerformanceFrequency.QuadPart ) * 1000.0;
    
    tc->results[iter] = aMilliSecCount/10.0;
}
