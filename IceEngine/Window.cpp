#include "Window.h"
#include "Errors.h"

namespace IceEngine
{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}
		//Create Window
		_sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr)
			FATAL_ERROR("SDL Window cannot be created");
		//Bind OpenGL context to the window
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
			FATAL_ERROR("SDL glContext cannot be created");
		//Initialize GLEW 
		GLenum err = glewInit();
		if (err != GLEW_OK)
			FATAL_ERROR("Cannot initialize GLEW");

		//Print OpenGL version for debugging
		printf("***  OpenGL Version: %s  ***\n\n\n", glGetString(GL_VERSION));

		//Set clear color to a light blue
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

		//Enable VSYNC
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}
}