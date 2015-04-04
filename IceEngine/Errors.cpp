#include "Errors.h"
#include <iostream>
#include <SDL\SDL.h>

namespace IceEngine
{
	//Called upon fatal error; Takes an error message
	void FATAL_ERROR(std::string errorString, bool log /*true*/)
	{
		std::cout << errorString << std::endl; //Print error message
		if (log)
		{
			//Wait for input so users can copy the error message
			std::cout << "Press Enter Key To Quit. . ." << std::endl;
			std::cin.ignore();
		}
		//Tell's SDL to close...
		SDL_Quit();
		//...Then close the program as there's no need to carry on if a fatal error has occured
		exit(404);
	}
}