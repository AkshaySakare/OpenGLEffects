   // common header files 

#include <windows.h> // win32 api
#include <stdio.h> // For file io
#include <stdlib.h> // for exit


// Opengl Header files
#include <GL/GLEW.h>
#include <gl/GL.h>
#include "vmath.h"
using namespace vmath;


#include "OGL.h" 


 //macros
#define WIN_WIDTH 800
#define WIN_HIGHT 600
//link with opengl libaray
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")


// Opengl related variable
//GLuint vertexShaderObject = 0;
//GLuint fragmentShaderObject = 0;
GLuint shaderProgramObject = 0;


enum
{
	AMC_ATTRIBUTE_POSITION = 0 ,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TANGET,



};
GLuint vao_pyramid = 0;

GLuint vboPosition_pyramid = 0;

GLuint vboColor_pyramid = 0;
GLuint vboTexure_pyramid = 0;

GLuint textureSampleUniform = 0;
GLuint defussionSampleUniform = 0;
 GLuint modelUniform =0;
GLuint viewUniform =0;
GLuint projectionUniform =0;
GLuint lightPosUniform =0;
GLuint viewPosUniform =0;

GLuint vao_cube = 0;
GLuint vboPosition_cube = 0;
GLuint vboColor_cube = 0;
GLuint vboTexure_cube = 0;
GLuint vboTagent_cube = 0;
GLuint vboNormal_cube = 0;
BOOL bResult;

// texture variable
GLuint	texture_stone = 0; //texture variable for stone texture
GLuint	texture_kundali = 0; //texture variable for kundali texture


mat4 orthographicProjectionMatrix;
mat4 ParspectivegraphicProjectionMatrix;


GLfloat tAngle = 0.0f;
GLfloat rAngle = -360.0f;

GLfloat pAngle = 0.0f;
GLfloat cAngle = 0.0f;


HDC ghdc = NULL;
HGLRC ghrc = NULL;
// global function declartion
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declartion

FILE* gpFILE = NULL;
int SH, SW;
int WindowHeight = 600;
int WindowWidth = 800;
HWND ghwnd = NULL;
BOOL gbActive= FALSE;

DWORD dwStyle = 0;
WINDOWPLACEMENT wpPre = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen = FALSE;
//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{// function declartions
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);
	void printGLInfo(void);

//local variable declartions
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] =TEXT("APSWindow");
	int iResult = 0;
	BOOL bdone = FALSE;

	


// code
// code
	/*gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		
	}*/
	if (fopen_s(&gpFILE, "Log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("LOGFILE CAN NOT OPEN"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else {
		fprintf(gpFILE, "Program Started Successfully\n");
		fprintf(gpFILE, "==========================\n");


	}
	// wndclassex initization 
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW |CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	// get center of screen
	SH = GetSystemMetrics(SM_CYSCREEN);
	SW = GetSystemMetrics(SM_CXSCREEN);
	
	// register wndclassex
	RegisterClassEx(&wndclass);

	//create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Akshay Padmakar Sakare"),
		WS_OVERLAPPEDWINDOW| WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,
		(SW / 2) - (WIN_WIDTH / 2),
		(SH / 2) - (WIN_HIGHT / 2),
		WIN_WIDTH,
		WIN_HIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;
	// initializetion
	iResult = initialize();
	if(iResult!=0){
	
		    MessageBox(hwnd, TEXT(" initialize() Fail"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd);
	}
	//show the window
	ShowWindow(hwnd, iCmdShow);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);


	
	// Game loop
	while (bdone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bdone = TRUE;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
		}
		else {

		
			if (gbActive == TRUE)
			{
				//render
				display();

				// update
				update();

			}
			

		}
	}
	
	//uninitialize
	uninitialize();

	// message loop
	/*while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
         
	}*/


	return ((int)msg.wParam);

}


// callback function
LRESULT CALLBACK WndProc(HWND hwnd , UINT iMsg, WPARAM wParam, LPARAM lParam)

{ //function dec
	void ToggleFullScreen(void);
	void resize(int, int);
	//code 
	
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;
	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return(0);
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam)) {
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;
	case WM_CHAR:
		switch (LOWORD(wParam)) {
		case 'F':
		case 'f':
			if (gbFullscreen == FALSE) {
				ToggleFullScreen();
				gbFullscreen = TRUE;
				if (gpFILE)
				{
					fprintf(gpFILE, "Program FullScreen Successfully\n");
				}
			}
			else {
				ToggleFullScreen();
				gbFullscreen = FALSE;
				if (gpFILE)
				{
					fprintf(gpFILE, "Program Normal size Successfully\n");
				}
			}
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case  WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd,iMsg,wParam,lParam));
}
void ToggleFullScreen(void)
{
	//local variable declartion
	MONITORINFO mi = { sizeof(MONITORINFO) };
	//code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPre) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);


			}
		}
		ShowCursor(FALSE);
	}
	else {
		SetWindowPlacement(ghwnd, &wpPre);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);


	}
}
int initialize(void) {
	void uninitialize(void);
	void resize(int , int );
	void printGLInfo(void);
	BOOL loadGlTextures(GLuint*, TCHAR[]);

//function declartions
	PIXELFORMATDESCRIPTOR pfd;
	int iPixcelFormatIndex = 0;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	// initializtion of pixcel format descriptor
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // size of the 
	pfd.nVersion = 1; // opengl version
	pfd.dwFlags = PFD_DRAW_TO_WINDOW| PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; 
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAccumRedBits = 8 ;
	pfd.cAccumGreenBits = 8 ;
	pfd.cAccumBlueBits = 8;
	pfd.cAlphaBits = 8 ;
	pfd.cDepthBits = 32;
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "getdc Fail\n");

		return(-1);
	}

	iPixcelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixcelFormatIndex == 0)
	{
		fprintf(gpFILE, "Chose PixcelFormat Fail\n");
		return(-2);
	}
	if (SetPixelFormat(ghdc, iPixcelFormatIndex, &pfd) == FALSE) {
		fprintf(gpFILE, "SetPixelFormat Fail\n");
		return (-3) ;
	}
	// tell bridging lib fron this device context 
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContext Fail\n");
		return(-4);
	}
		// make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent Fail\n");
		return (-5);
	}

	// init GLEW 
	if (glewInit() != GLEW_OK) {
		fprintf(gpFILE, "glewInit Failed \n");
		return (-6);
	}
	printGLInfo();
	//  vertex sheder
	const GLchar* vertexShederSourceCode =
	"#version 460 core " \
			"\n "\
		"layout(location = 0) in vec3 aPosition; "  
"layout(location = 1) in vec3 aNormal; "    
"layout(location = 2) in vec2 aTexCoord; "  
"layout(location = 3) in vec3 aTangent; "   

"out vec2 texCoords;  "     // Pass to fragment shader
"out vec3 pos; "           // Fragment position in world space
"out vec3 tangentLightPos; "   // Light position in tangent space
"out vec3 tangentViewPos;"     // Camera position in tangent space
"out vec3 tangentFragPos; "    // Fragment position in tangent space

"uniform mat4 uModelMatrix;"
"uniform mat4 uViewMatrix;"
"uniform mat4 uProjectionMatrix;"
"uniform vec3 lightPos;"
"uniform vec3 uViewPos;"

"void main() {"
    // Transform the vertex position to world space
    "vec4 worldPos = uModelMatrix * vec4(aPosition, 1.0);"
"    pos = vec3(worldPos);"

    // Calculate the TBN (Tangent, Bitangent, Normal) matrix
    "vec3 T = normalize(mat3(uModelMatrix) * aTangent);"
   " vec3 N = normalize(mat3(uModelMatrix) * aNormal);"
    "vec3 B = cross(N, T);"
    "mat3 TBN = mat3(T, B, N);"

    // Transform positions to tangent space
    "tangentLightPos = TBN * (lightPos - pos);"
    "tangentViewPos = TBN * (uViewPos - pos);"
    "tangentFragPos = TBN * pos;"

    "texCoords = aTexCoord;"
    "gl_Position = uProjectionMatrix * uViewMatrix * worldPos;"
"}";
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1,(const GLchar **)&vertexShederSourceCode,NULL);
	glCompileShader(vertexShaderObject);
	//check vertex shader error
	//a 
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar * szInfoLog = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0) {
				szInfoLog = (GLchar*)malloc(infoLogLength);
				if (szInfoLog != NULL)
				{

					glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
					fprintf(gpFILE, "Vertex shader compilation log: %s\n", szInfoLog);
					free(szInfoLog);
					szInfoLog = NULL;
					

					//return (-7);
				}
				uninitialize();
			}
		}
	// Fragment Shader
		const GLchar* fragmentShederSourceCode =
			"#version 460 core " \
			"\n "\
			"in vec2 texCoords;"
"in vec3 tangentLightPos;"
"in vec3 tangentViewPos;"
"in vec3 tangentFragPos;"
"out vec4 oColor;"

"uniform sampler2D diffuseMap;" // Base color texture
"uniform sampler2D normalMap; " // Normal map texture

			"void main() {"
    // Sample the normal map and convert it from [0,1] to [-1,1]
   " vec3 normal = texture(normalMap, texCoords).rgb;"
    "normal = normalize(normal * 2.0 - 1.0);"

    // Calculate lighting
    "vec3 lightDir = normalize(tangentLightPos - tangentFragPos);"
    "float diff = max(dot(lightDir, normal), 0.0);"

    // Calculate view direction and specular component
    "vec3 viewDir = normalize(tangentViewPos - tangentFragPos);"
 " vec3 reflectDir = reflect(-lightDir, normal); "
   "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);"

    // Combine lighting components
   " vec3 diffuse = diff * vec3(texture(diffuseMap, texCoords));"
   " vec3 specular = spec * vec3(0.2);" // Specular intensity
    "vec3 result = diffuse + specular;"

 " oColor = vec4(result, 1.0);"
"}";
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const char**)&fragmentShederSourceCode, NULL);
	glCompileShader(fragmentShaderObject);

	//check fragment shader error
	//a 
	  status = 0;
	  infoLogLength = 0;
      szInfoLog = NULL;

	  glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
			if (infoLogLength > 0) {
				szInfoLog = (GLchar*)malloc(infoLogLength);
				if (szInfoLog != NULL)
				{

					glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
					fprintf(gpFILE, "Fragment shader compilation log: %s\n", szInfoLog);
					free(szInfoLog);
					szInfoLog = NULL;
				

					//return (-7);
				}
				uninitialize();
			}
		}

	// shader program
	shaderProgramObject = glCreateProgram();
	//vertexShaderObject attached
	glAttachShader(shaderProgramObject, vertexShaderObject);
	//fragmentShaderObject attached
   glAttachShader(shaderProgramObject, fragmentShaderObject);
   
   // Bind attribute location 
   glBindAttribLocation(shaderProgramObject,AMC_ATTRIBUTE_POSITION, "aPosition"); //
 glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");
 //AMC_ATTRIBUTE_TANGET
  glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TANGET, "aTangent");
   // Bind the color location
   glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
   // link program object
   glLinkProgram(shaderProgramObject);
   // 
   status = 0;
   infoLogLength = 0;
   szInfoLog = NULL;

   glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	   if (status == GL_FALSE)
	   {
		   glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		   if (infoLogLength > 0) {
			   szInfoLog = (GLchar*)malloc(infoLogLength);
			   if (szInfoLog != NULL)
			   {

				   glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				   fprintf(gpFILE, " shader Program linking error log: %s\n", szInfoLog);
				   free(szInfoLog);
				   szInfoLog = NULL;
				  

				   //return (-7);
			   }
			   uninitialize();
		   }
	   }


	   defussionSampleUniform = glGetUniformLocation(shaderProgramObject, "diffuseMap");
	   textureSampleUniform = glGetUniformLocation(shaderProgramObject, "normalMap");
	   viewUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");
	   projectionUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	   modelUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");
	   lightPosUniform = glGetUniformLocation(shaderProgramObject, "lightPos");
	   viewPosUniform = glGetUniformLocation(shaderProgramObject, "viewPos");
	   //vao_pyramid
	  

	  
	   // VAO
	
	   //==========Square

	   GLfloat cubeVertices[] =
	   {
		   // front
			1.0f,  1.0f,  1.0f, // top-right of front
		   -1.0f,  1.0f,  1.0f, // top-left of front
		   -1.0f, -1.0f,  1.0f, // bottom-left of front
			1.0f, -1.0f,  1.0f, // bottom-right of front

			// right
			 1.0f,  1.0f, -1.0f, // top-right of right
			 1.0f,  1.0f,  1.0f, // top-left of right
			 1.0f, -1.0f,  1.0f, // bottom-left of right
			 1.0f, -1.0f, -1.0f, // bottom-right of right

			 // back
			  1.0f,  1.0f, -1.0f, // top-right of back
			 -1.0f,  1.0f, -1.0f, // top-left of back
			 -1.0f, -1.0f, -1.0f, // bottom-left of back
			  1.0f, -1.0f, -1.0f, // bottom-right of back

			  // left
			  -1.0f,  1.0f,  1.0f, // top-right of left
			  -1.0f,  1.0f, -1.0f, // top-left of left
			  -1.0f, -1.0f, -1.0f, // bottom-left of left
			  -1.0f, -1.0f,  1.0f, // bottom-right of left

			  // top
			   1.0f,  1.0f, -1.0f, // top-right of top
			  -1.0f,  1.0f, -1.0f, // top-left of top
			  -1.0f,  1.0f,  1.0f, // bottom-left of top
			   1.0f,  1.0f,  1.0f, // bottom-right of top

			   // bottom
				1.0f, -1.0f,  1.0f, // top-right of bottom
			   -1.0f, -1.0f,  1.0f, // top-left of bottom
			   -1.0f, -1.0f, -1.0f, // bottom-left of bottom
				1.0f, -1.0f, -1.0f, // bottom-right of bottom
	   };

	   GLfloat cubeTexcoords[] =
	   {
		   // front
		   1.0f, 1.0f, // top-right of front
		   0.0f, 1.0f, // top-left of front
		   0.0f, 0.0f, // bottom-left of front
		   1.0f, 0.0f, // bottom-right of front

		   // right
		   1.0f, 1.0f, // top-right of right
		   0.0f, 1.0f, // top-left of right
		   0.0f, 0.0f, // bottom-left of right
		   1.0f, 0.0f, // bottom-right of right

		   // back
		   1.0f, 1.0f, // top-right of back
		   0.0f, 1.0f, // top-left of back
		   0.0f, 0.0f, // bottom-left of back
		   1.0f, 0.0f, // bottom-right of back

		   // left
		   1.0f, 1.0f, // top-right of left
		   0.0f, 1.0f, // top-left of left
		   0.0f, 0.0f, // bottom-left of left
		   1.0f, 0.0f, // bottom-right of left

		   // top
		   1.0f, 1.0f, // top-right of top
		   0.0f, 1.0f, // top-left of top
		   0.0f, 0.0f, // bottom-left of top
		   1.0f, 0.0f, // bottom-right of top

		   // bottom
		   1.0f, 1.0f, // top-right of bottom
		   0.0f, 1.0f, // top-left of bottom
		   0.0f, 0.0f, // bottom-left of bottom
		   1.0f, 0.0f, // bottom-right of bottom
	   };
		  
GLfloat normals[] =     {0.0f, 0.0f, -1.0f,            /* Back */
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, 1.0f,            /* Front */
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        -1.0f, 0.0, 0.0f,            /* Left */
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,             /* Right */
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,             /* Top */
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f,            /* Bottom */
                        0.0f, -1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f
};
	 
	 GLfloat tagent[] =
	  {-1.0f, 0.0f, 0.0f,            /* Back */
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,                /* Front */
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f,                /* Left */
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, -1.0f,                /* Right */
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,
                        1.0f, 0.0f, 0.0f,                /* Top */
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,                /* Bottom */
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f
};

	   //
	   //GLuint vao_Square = 0;
	  // GLuint vboPosition_Square = 0;
	  // VAO
	   glGenVertexArrays(1, &vao_cube);
	   glBindVertexArray(vao_cube);
	   //vbo for Position
	   glGenBuffers(1, &vboPosition_cube);
	   glBindBuffer(GL_ARRAY_BUFFER, vboPosition_cube);

	   glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	   glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	   glBindBuffer(GL_ARRAY_BUFFER, 0);
	  

	   glGenBuffers(1, &vboTexure_cube);
	   glBindBuffer(GL_ARRAY_BUFFER, vboTexure_cube);
	   glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexcoords), cubeTexcoords, GL_STATIC_DRAW);
	   glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	   glBindBuffer(GL_ARRAY_BUFFER, 0);


	   glGenBuffers(1, &vboTagent_cube);
	   glBindBuffer(GL_ARRAY_BUFFER, vboTagent_cube);
	   glBufferData(GL_ARRAY_BUFFER, sizeof(tagent), tagent, GL_STATIC_DRAW);
	   glVertexAttribPointer(AMC_ATTRIBUTE_TANGET, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(AMC_ATTRIBUTE_TANGET);
	   glBindBuffer(GL_ARRAY_BUFFER, 0);

	   glGenBuffers(1, &vboNormal_cube);
	   glBindBuffer(GL_ARRAY_BUFFER, vboNormal_cube);
	   glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	   glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	   glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
	   glBindBuffer(GL_ARRAY_BUFFER, 0);
	   
	   glBindVertexArray(0);



	// compulsory 
	glClearDepth(1.0f);// set 
	glEnable(GL_DEPTH_TEST);// enable depth test
	glDepthFunc(GL_LEQUAL);//
	
	

	//set color of window to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//create texture 
	bResult = loadGlTextures(&texture_stone, MAKEINTRESOURCE(MY_STONE_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loading stone texture Fail\n");
		return (-6);
	}
	bResult = loadGlTextures(&texture_kundali, MAKEINTRESOURCE(MY_KUNDALI_BITMAP));

	if (bResult == FALSE)
	{
		fprintf(gpFILE, "loading kundali texture Fail\n");
		return (-7);
	}
	// tell opengl to enable to texture
	glEnable(GL_TEXTURE_2D);


	ParspectivegraphicProjectionMatrix = vmath::mat4::identity();
	resize(WIN_WIDTH, WIN_HIGHT);



	return 0;
}
void printGLInfo(void) {
	// variable dec
	GLint numExtenstion;
	GLint i;
	//code
	fprintf(gpFILE, "==========================\n");
	fprintf(gpFILE, "Open GL Vendor : %s  \n",glGetString(GL_VENDOR));
	fprintf(gpFILE, "Open GL Render : %s  \n", glGetString(GL_RENDERER));
	fprintf(gpFILE, "Open GL Version : %s  \n", glGetString(GL_VERSION));
	fprintf(gpFILE, "Open GL GLSL Version : %s  \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	// listing of supported extenstion
	 glGetIntegerv(GL_NUM_EXTENSIONS, &numExtenstion);
	 for (i = 0; i < numExtenstion; i++) {
		 fprintf(gpFILE, " %s\n", glGetStringi(GL_EXTENSIONS,i));
	 }
	 fprintf(gpFILE, "==========================\n");
}


void resize(int width, int height) {
	// code
	if (height <= 0) 
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // viewport meaning binocular | bioscope
	// set Parspective projection matrix
	ParspectivegraphicProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);



	

	
	}
	


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramObject);
	//==============

	vmath::mat4 modelViewMatrix = vmath::mat4::identity();
		vmath::mat4 ViewMatrix = vmath::mat4::identity();
	vmath::mat4 translationMatrix = vmath::mat4::identity();
	
	
	 translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	 vmath::mat4 scaleMatrix = vmath::mat4::identity();
	 scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	 vmath::mat4 rotationMatrix1 = vmath::mat4::identity();
	  rotationMatrix1 = vmath::rotate(cAngle, 1.0f, 0.0f, 0.0f);
	  vmath::mat4 rotationMatrix2 = vmath::mat4::identity();
	  rotationMatrix2 = vmath::rotate(cAngle, 0.0f, 1.0f, 0.0f);
	  vmath::mat4 rotationMatrix3 = vmath::mat4::identity();
	 rotationMatrix3 = vmath::rotate(cAngle, 0.0f, 0.0f, 1.0f);
	 vmath::mat4 rotationMatrix = vmath::mat4::identity();

	 rotationMatrix =  rotationMatrix1 * rotationMatrix2 * rotationMatrix3;
	 modelViewMatrix = translationMatrix* scaleMatrix * rotationMatrix ;
	 vmath::mat4 modelViewProjectionMatrix = vmath::mat4::identity();
	// modelViewProjectionMatrix = ParspectivegraphicProjectionMatrix * modelViewMatrix;// order is VIMP he ffp internally karay ch
	// push above mvp vertex shaders mvp uniform

	// GLuint textureSampleUniform = 0;
// GLuint defussionSampleUniform = 0;
//  GLuint modelUniform =0;
// GLuint viewUniform =0;
// GLuint projectionUniform =0;
// GLuint lightPosUniform =0;
// GLuint viewPosUniform =0;
GLfloat materialdefuse[] = { 1.0f, 2.0f, 3.0f,1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 5.0f,1.0f };

	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelViewMatrix);
glUniformMatrix4fv(viewUniform, 1, GL_FALSE, ViewMatrix);
glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, ParspectivegraphicProjectionMatrix);
glUniform3fv(lightPosUniform,1, materialdefuse);
   glUniform3fv(viewPosUniform, 1,lightPosition);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_kundali);
	glUniform1i(textureSampleUniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_kundali);
	glUniform1i(defussionSampleUniform, 1);

	glBindVertexArray(vao_cube);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

//	
	SwapBuffers(ghdc);
	//code 
}
void update(void) {
	pAngle = pAngle + 0.1f;
	if (pAngle >= 360.0f) {
		pAngle = -360.0f;
	}
	cAngle = cAngle + 0.1f;
	if (cAngle >= 360.0f) {
		cAngle = -360.0f;
	}


//code
}
void uninitialize(void) {
	//function declartions
	void ToggleFullScreen(void);

	//code
	// shaderProgramObject
	if (shaderProgramObject) {
		glUseProgram(shaderProgramObject);
		GLint numShaders = 0;
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
		if (numShaders > 0) {
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
			if (pShaders != NULL) {
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
				for(GLint i = 0; i < numShaders; i++) {
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;

				}
				free(pShaders);
				pShaders = NULL;
			}
		}
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

	//=== square
	if (vboColor_cube) {
		glDeleteBuffers(1, &vboColor_cube);
		vboColor_cube = 0;
	}
	if (vboPosition_cube) {
		glDeleteBuffers(1, &vboPosition_cube);
		vboPosition_cube = 0;
	}
	if (vao_cube) {
		glDeleteVertexArrays(1, &vao_cube);
		vao_cube = 0;
	}
	//Triangle  vao_pyramid
	if (vboColor_pyramid){
		glDeleteBuffers(1, &vboColor_pyramid);
		vboColor_pyramid = 0;
	}

	if (vboPosition_pyramid){
		glDeleteBuffers(1, &vboPosition_pyramid);
		vboPosition_pyramid = 0;
	}
	if (vao_pyramid){
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}
	if(vboTexure_pyramid){
		glDeleteBuffers(1, &vboTexure_pyramid);
		vboTexure_pyramid = 0;
	}
	if (vboTexure_cube) {
		glDeleteBuffers(1, &vboTexure_cube);
		vboTexure_cube = 0;
	}

	if (texture_kundali)
	{
		glDeleteTextures(1, &texture_kundali);
		texture_kundali = 0;
		fprintf(gpFILE, "texture_kundali Ended Successfully\n");

	}
	if (texture_stone)
	{
		glDeleteTextures(1, &texture_stone);
		texture_stone = 0;
		fprintf(gpFILE, "texture_stone Ended Successfully\n");

	}




	
	// unuse
	
	// if application is exiting full screen that time below code work
	if (gbFullscreen == TRUE)
	{
		ToggleFullScreen();
		gbFullscreen = FALSE;
	}

	//make the hdc as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);

	}
	// delet rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		
	}
	// relese the hdc
	if (ghdc)
	{
		ReleaseDC(ghwnd,ghdc);
		
		ghdc = NULL;

	}

	
	//distory window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;

	}


	//close log file
	if (gpFILE)
	{
		fprintf(gpFILE, "==========================\n");
		fprintf(gpFILE, "Program Ended Successfully\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}
	


}




BOOL loadGlTextures(GLuint* texture, TCHAR imageResourceID[])
{
	// local variable 
	HBITMAP hBitmap = NULL;
	BITMAP bmp; // bitmap win32 sdk structure
	// load image 
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		fprintf(gpFILE, "LoadImage Fail\n");
		return FALSE;
	}
	//get image data
	GetObject(hBitmap, sizeof(BITMAP), &bmp);
	// create opengl texture
	glGenTextures(1, texture);
	// bind to generate texture
	glBindTexture(GL_TEXTURE_2D, *texture);// 1st is binding point it is define in state machine
	//image pixcel Unpack and alingment 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//pixel storage mode, in FF it is 4  for 4 is good for aligment and 1 is for performace
	// set texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,bmp.bmBits);
	glGenerateMipmap(GL_TEXTURE_2D);
	// un bind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//delete obj
	DeleteObject(hBitmap);
	hBitmap = NULL;
	return TRUE;
}


