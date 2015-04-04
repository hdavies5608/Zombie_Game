#pragma once
#include <string>
#include <SDL\SDL.h>
#include <GL\glew.h>
namespace IceEngine
{
	enum WindowFlags
	{
		NONE = 0x0,
		INVISIBLE = 0x1, 
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4
	};


	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return _screenWidth; };
		int getScreenHeight() { return _screenHeight; };

	private:
		SDL_Window* _sdlWindow;
		int _screenWidth;
		int _screenHeight;
	};
}
