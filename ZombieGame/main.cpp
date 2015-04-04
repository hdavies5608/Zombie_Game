#include "MainGame.h"
#include <iostream>

/// Although unused params need to be included or SDL won't compile as SDL_Main takes them 
/// as parameters
int main(int argc, char *args[] ) 
{

    /// Void command-line arguments to remove an unused variable compiler warning
    (void)argc;
    (void)args;

    MainGame mainGame;
    mainGame.run();

	std::cout << "Press Enter Key To Quit. . ." << std::endl;
	std::cin.ignore();

    return 0;
}