#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxRenderWindow.h"
#include "LuxEventListener.h"
#include "LuxEventListenerOGL.h"
#include "LuxRenderWindowOGL.h"
#include "LuxErrorCheckOGL.h"
#include "LuxKey.h"
#include "LuxBufferOGL.h"
#include "LuxVertexArrayOGL.h"
#include "LuxShader.h"
#include "LuxShaderOGL.h"
#include "LuxMesh.h"
#include "LuxMeshOGL.h"
#include "LuxModel.h"
#include "LuxModelOGL.h"

bool Lux::Core::Internal::RenderWindowOGL::Initialize(Utility::AppInitOptions& a_AppInitOptions) 
{
	glfwWindowHint(GLFW_SAMPLES, a_AppInitOptions.m_AntiAliasing);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context 
	if (a_AppInitOptions.m_FullScreen)
	{
		m_WindowHandle = glfwCreateWindow(a_AppInitOptions.m_WindowWidth, a_AppInitOptions.m_WindowHeight, a_AppInitOptions.m_WindowCaption.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		m_WindowHandle = glfwCreateWindow(a_AppInitOptions.m_WindowWidth, a_AppInitOptions.m_WindowHeight, a_AppInitOptions.m_WindowCaption.c_str(), nullptr, nullptr);
	}
	
	if (m_WindowHandle == nullptr)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize specified OpenGL " << 4 << "." << 5 << " Context.";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_WindowHandle);
	Utility::Internal::CheckOGLError();

	// Initialize GLEW 
	glewExperimental = GL_TRUE;
	int initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize GLEW. " << "Error code: " << initResult;
		return false;
	}
	Utility::Internal::ClearAllOGLErrors();

	// Create a default listener
	m_Input = new EventListenerOGL(this);

	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	glDepthRange(0.0f, 1.0f);
	glClearDepth(1.0f);
	glClearColor(WINDOW_CLEAR_COLOR.x, WINDOW_CLEAR_COLOR.y, WINDOW_CLEAR_COLOR.z, WINDOW_CLEAR_COLOR.w);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	Utility::Internal::CheckOGLError();

	glfwSwapInterval(a_AppInitOptions.m_ScreenSwapInterval);

	// Windows being a bitch, as always
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	// Turn on vertical screen sync under Windows.
	// (I.e. it uses the WGL_EXT_swap_control extension)
	typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(a_AppInitOptions.m_ScreenSwapInterval);
#endif

	m_WindowWidth = a_AppInitOptions.m_WindowWidth;
	m_WindowHeight = a_AppInitOptions.m_WindowHeight;
	m_WindowResized = false;
	m_Fullscreen = a_AppInitOptions.m_FullScreen;
	m_SwapInterval = a_AppInitOptions.m_ScreenSwapInterval;
	m_Caption = a_AppInitOptions.m_WindowCaption;
	m_AntiAliasing = a_AppInitOptions.m_AntiAliasing;

	return true;
}

Lux::Core::EventListener* Lux::Core::Internal::RenderWindowOGL::GetEventListener()
{
	return m_Input;
}

Lux::Core::Internal::RenderWindowOGL::RenderWindowOGL() : m_WindowHandle(0), m_Input(0)
{

}

Lux::Core::Internal::RenderWindowOGL::~RenderWindowOGL()
{
	glfwDestroyWindow(m_WindowHandle);
	Utility::SafePtrDelete(m_Input);
}

void Lux::Core::Internal::RenderWindowOGL::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}

void Lux::Core::Internal::RenderWindowOGL::PollEvents()
{
	glfwPollEvents();
}

void Lux::Core::Internal::RenderWindowOGL::Render(Mesh* a_Mesh)
{
	MeshOGL* subMesh = (MeshOGL*)a_Mesh;
	unsigned int numIndices = subMesh->GetNumIndices();
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Lux::Core::Internal::RenderWindowOGL::Clear()
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(m_WindowHandle, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	
	if (m_WindowWidth != width || m_WindowHeight != height)
	{
		m_WindowWidth = width;
		m_WindowHeight = height;
		m_WindowResized = true;
	}
	else
	{
		m_WindowResized = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

const bool Lux::Core::Internal::RenderWindowOGL::IsWindowResized()
{
	return m_WindowResized;
}

const int Lux::Core::Internal::RenderWindowOGL::GetHeight()
{
	return m_WindowHeight;
}

const int Lux::Core::Internal::RenderWindowOGL::GetWidth()
{
	return m_WindowWidth;
}
