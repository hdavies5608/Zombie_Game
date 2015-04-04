#pragma once

#include <IceEngine/Window.h>
#include <IceEngine/GLSLProgram.h>
#include <IceEngine/Camera2D.h>
#include <IceEngine/InputManager.h>
#include <IceEngine/SpriteBatch.h>
#include <IceEngine/GLTexture.h>
#include <IceEngine/ResourceManager.h>
#include <IceEngine/Timing.h>

#include <vector>

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

enum class GameState
{
	EXIT,
	PLAY
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	/// Runs the game
	void run();

private:
	/// Initializes the core systems
	void initSystems();

	/// Initializes a level
	bool initLevel(std::string fileName);

	/// DeInitializes the level and clears up any unneeded objects
	void deInitLevel();

	/// Initializes the shaders
	void initShaders();

	/// Main game loop for the program
	void gameLoop();

	/// Updates all of the agents
	void updateAgents(float deltaTime);

	/// Updates all bullets
	void updateBullets(float deltaTime);

	/// Checks all victory and loss conditions and preforms an action
	int checkConditions();

	/// Processes the input
	void processInput();

	/// Renders the game
	void drawGame();

	///A function to end the game nicely
	void exitGame(std::string message);

	/// Engine Member Variables
	IceEngine::Window _window; ///< The game window

	IceEngine::GLSLProgram _textureProgram; ///< The shader program

	IceEngine::InputManager _inputManager; ///< Handles input

	IceEngine::Camera2D _camera; ///< Main Camera

	IceEngine::FPSLimiter _fpsLimiter;

	IceEngine::SpriteBatch _agentSpriteBatch;

	/// Non-Engine Member Variables
	GameState _gameState; ///< Store the current game state

	std::vector<Level*> _levels;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;
	Player* _player;
	int _screenWidth, _screenHeight;

	float _fps;

	int _currentLevel;

	int _numHumansKilled; ///< Humans killed by player
	int _numZombiesKilled; ///< Zombies killed by player

	bool playerIsInit;
};

