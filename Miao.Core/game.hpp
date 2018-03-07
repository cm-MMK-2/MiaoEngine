#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <cmath>

#include "MResourceLoader.hpp"
#include "MRenderQueue.hpp"
#include "MInput.hpp"
#include <windows.h>
#include <GL/wglext.h>

class MGameBase
{

public:
	virtual void OnInit() = 0;

	virtual void OnUpdate(float deltaTime) = 0;
};

class Game
{
public: //--------------------------------------------------------public
	~Game()
	{
		// Terminate GLFW, clearing any resources allocated by GLFW.
		glfwTerminate();
	}

	//screen size
	GLuint	   width, height; 

	//frames per sec
	int fps; 

	//game run time
	double time;

	//time cost for each frame
	double deltaTime;

	//for log
	static std::shared_ptr<spdlog::logger> rotating_logger;

	glm::mat4 projection;

	MRenderQueue renderer;

	//create game instance
	static int Create(GLuint width = 800, GLuint height = 600, const char* title = u8"Game", MGameBase* gameApp = nullptr);

	/*
	 * VSync switch
	 */
	void SetVSync(bool sync)
	{
		// Function pointer for the wgl extention function we need to enable/disable vsync
		typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
		// this is pointer to function which returns pointer to string with list of all wgl extensions
		PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

		// determine pointer to wglGetExtensionsStringEXT function
		_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

		if (strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") == NULL)
		{
			// string was not found
			return;
		}

		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapIntervalEXT)
				wglSwapIntervalEXT(sync);
		}
	}

	void SetKeyEventListener(KeyEvent onKeyDown, KeyEvent onKeyUp = nullptr)
	{
		MInput::OnKeyDown = onKeyDown;
		MInput::OnKeyUp = onKeyUp;
	}

private: //------------------------------------------------------private
	MText fpsCounter;
	MInput* mInput;
	MGameBase* mGameApp;
	GLFWwindow* mWindow;
	// Constructor/Destructor
	Game(GLuint _width, GLuint _height)
	{
		this->width = _width;
		this->height = _height;
	}

	int CreateGameWindow(const char* title);

	Game(Game&) = delete;//disable copy
};

Game* game = nullptr;
//extern std::unique_ptr<Game> game;
std::shared_ptr<spdlog::logger> Game::rotating_logger;

//-------------implementations
int Game::Create(GLuint width, GLuint height, const char* title, MGameBase* gameApp)
{
	if (!game)
	{
		game = new Game(width, height);
		game->mGameApp = gameApp;
		//rotating_logger = spdlog::rotating_logger_mt("file_logger", "logs/log", 1048576 * 5, 3);
		//rotating_logger->info("Start Game");
		return game->CreateGameWindow(title);
	}
	else
	{
		//game already created
		std::cout << "Error::Game: instance already created" << std::endl;
		//rotating_logger->error("Error::Game: instance already created");
		//spdlog::get("file_logger")->flush();
		return -1;
	}
}

int Game::CreateGameWindow(const char* title)
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mWindow);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	glViewport(0, 0, width, height);



	// Set OpenGL options
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	mInput = new MInput(mWindow, MINPUT_KEYBOARD | MINPUT_JOYSTICK);

	renderer.Init();

	// Initialize game
	//GameInit();
	//spine bone 0,0 at top left
	spBone_setYDown(true);
	//game projection
	projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width),
		static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);

	mGameApp->OnInit();

	// DeltaTime variables
	double lastFrame = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(mWindow))
	{
		// Calculate delta time
		time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Update Game state
		//Update(deltaTime);
		mGameApp->OnUpdate(deltaTime);
		// Render
		// Clear the colorbuffer
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.Render(deltaTime);

		// Swap the screen buffers
		glfwSwapBuffers(mWindow);
	}

	renderer.Release();
	return 0;
}