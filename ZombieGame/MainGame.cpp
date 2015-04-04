#include "MainGame.h"

#include <IceEngine/IceEngine.h>
#include <IceEngine/Errors.h>

#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED         = 1.0f;
const float ZOMBIE_SPEED        = 1.3f;
const float PLAYER_SPEED        = 6.0f;
const float PLAYER_SPRINT_SPEED = 8.0f;

MainGame::MainGame() :
_screenWidth(1024),
_screenHeight(768),
_fps(0),
_gameState(GameState::PLAY),
_player(nullptr),
_numHumansKilled(0),
_numZombiesKilled(0),
_currentLevel(0)
{
	// Empty
}

MainGame::~MainGame()
{	
	/// Remove all of the levels from the vector
	for (unsigned int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}

	/// Remove all of the humans
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		delete _humans[i];
	}

	/// Remove all the zombies
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		delete _zombies[i];
	}
}

// Function to begin setup of the game will call all functions needed to begin
//then will run the game loop.
// \return void
void MainGame::run()
{
	initSystems(); /// Set up display / shaders / spritebatch / camera etc.

	initLevel("level" + std::to_string(_currentLevel) + ".txt"); /// Load the first level

	gameLoop(); /// Begin the game

}

// Function to initialize all of the systems used in the game including:
//  - The engine
//  - The window
//  - Shaders
//  - The camera
// \return void
void MainGame::initSystems()
{
	SDL_DisplayMode current; /// Create a display mode to get fullscreen

	IceEngine::init(); /// Initialze the engine

	/// We want to loop through all displays
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

		if (should_be_zero != 0)
		{
			// In case of error...
			//SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
		}
		else
		{
			// On success, print the current display mode
			//SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);
		}
	}

    _screenWidth  = 1280;//current.w; /// Store the current width...
    _screenHeight = 720;//current.h; /// ... and height of the screen

	/// Create the window with title, width, height and display mode
	_window.create("Zombie Game", 1280, 720, IceEngine::NONE);

	///Set the clear color of the background
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f); /// Darkish grey

	/// Initialize the shaders for use in the game
	initShaders();

	/// Initialize spritebatching for agents
	_agentSpriteBatch.init();

	/// Initialize the camera with the screen width and height
	_camera.init(_screenWidth, _screenHeight);

}

// Initialize the level given by the filename.
// load's the given level into the _levels vector also creates the player, zombies 
// and humans as well as giving the player guns
// \return true if level loads false if not
bool MainGame::initLevel(std::string fileName)
{
	/// Push back a new level with the given filename
    int status = 0;
	_levels.push_back(new Level("Levels/" + fileName, _currentLevel, status));
    if (status == -1)
    {
        return false;
    }
	/// We want to check if SDL was initialized
	if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
	{
		return false;
	}

	/// We need to create and initialize a new player and set their position in the level
	/// We also need to give them access to the input manager, camera and bullets.
	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	/// We need the player to be the first element in the human vector.
	_humans.push_back(_player);

	/// We use mt19937 as it was more random than the default
	std::mt19937 randomEngine; /// Create a new random engine
	randomEngine.seed((unsigned long)time(nullptr)); /// Seed the engine with the time

	/// Positions for the humans to be placed in the level, we take 2 off each 'end'  of the 
	/// axis to ensure they both aren't outside or embedded in a wall.
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2); 
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);

	/// Add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human); /// Make a humans
		glm::vec2 nPos(randX(randomEngine)* TILE_WIDTH, randY(randomEngine) * TILE_WIDTH); /// Make it's position
		_humans.back()->init(HUMAN_SPEED, nPos); /// Initialize the human
	}


	/// Add all of the zombies
	/// We need a vector of zombie positions as they aren't randomly generated
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePos();
	for (unsigned int i = 0; i < zombiePositions.size(); i++)
	{
		_zombies.push_back(new Zombie); /// Make a zombie
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]); /// Initialize it with the position
	}

	/// Set up the players guns and add them to the vector
	/// <Name>, <Rire Rate>, <Bullets/Shot>, <Spread>, <Damage>, <Speed>
	_player->addGun(new Gun("Magnum", 40, 1, 5, 50, 50.0f));
	_player->addGun(new Gun("Shotgun", 120, 20, 30, 40, 15.0f));
	_player->addGun(new Gun("Holy Black Death", 0, 3, 20, 10, 20.0f));
	_player->addGun(new Gun("Lag Bomb", 360, 1000, 360, 100, 5.0f));

	/// If everything succeeded return true
	return true;
}

// Function to deinitialize parts of a level to allow for creation 
// of a new level
// \return void
void MainGame::deInitLevel()
{
	/// We need to remove all of the humans including the player
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		delete _humans[i];
	}
	/// And also remove all zombies
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		delete _zombies[i];
	}
	/// Then clear the vectors
	_zombies.clear();
	_humans.clear();
    _bullets.clear();
	/// If this isn't done then they will persist when creating a new level, 
	/// This created a lot of lag as, even thought they weren't being rendered,
	/// all calculations were still happening.
}

// Function to load all shaders, add all their attributes, and link them
// \return void
void MainGame::initShaders()
{
	// Compile our color shader
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

// The main function of the game
// An infinite loop that will run until the gameState isn't play, this can then
// run another loop depending on the game state, but won't exit the function.
// \return void (exited when no longer playing)
void MainGame::gameLoop()
{
	const float DESIRED_FPS     = 60.0f;
	const int MAX_PHYSICS_STEPS = 6; /// Maximum steps to calcualte before render

	_fpsLimiter.setTargetFPS(DESIRED_FPS);

	const float CAMERA_SCALE = 1.0f / 1.3f;
	_camera.setScale(CAMERA_SCALE);

	const float MS_PER_SEC        = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS;
	const float MAX_DELTA_TIME    = 1.0f;

	float previousTicks = (float)SDL_GetTicks();

	while (_gameState == GameState::PLAY)
	{
		float newTicks       = (float)SDL_GetTicks();
		float frameTime      = newTicks - previousTicks;
		previousTicks        = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		_fpsLimiter.beginFrame();

        if (checkConditions() == -1)
        {
            break;
        }

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			updateAgents(deltaTime);

			updateBullets(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}


		_camera.setPosition(_player->getPosition() + glm::vec2(_player->getSize() * 0.5, _player->getSize() * 0.5));
		_camera.update();

		drawGame();

		_fps = _fpsLimiter.endFrame();
	}
    std::cout << "Game Exiting..." << std::endl;
}

void MainGame::updateAgents(float deltaTime)
{
	//Update all humans
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	// Don't forget to update zombies
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}

	//Update zombie collision
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
		//Collide with other zombies
		for (unsigned int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//Collide with humans
		for (unsigned int j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->collideWithAgent(_humans[j]))
			{
				//Add the new zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//Delete the human	
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		if (_zombies[i]->collideWithAgent(_humans[0]))
		{
			exitGame("You Lose!");
		}
	}

	//Update Human collisions
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
		//Collide with other humans
		for (unsigned int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}

}

void MainGame::updateBullets(float deltaTime)
{
	//Update and collide with world
	for (unsigned int i = 0; i < _bullets.size();)
	{
		//If this is true then a collision occured
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else
		{
			i++;
		}
	}

	bool wasBulletRemoved;
	//Collide with agents
	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		wasBulletRemoved = false;
		//Loop through all zombies
		for (unsigned int j = 0; j < _zombies.size();)
		{
			if (_bullets[i].collideWithAgent(_zombies[j]))
			{
				//Damage the zombie and kill it if it's out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage()))
				{
					//If the zombie died remove it
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else
				{
					j++;
				}
				//Remove the bullet
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--; //We don't want to skip a bullet as one has just been deleted
				//Since there is no more bullet it would be a waste of CPU ticks to keep itterating through the zombies
				break;
			}
			else
			{
				j++;
			}
		}

		//Loop through all humans
		if (!wasBulletRemoved)
		{
			for (unsigned int j = 1; j < _humans.size();)
			{
				if (_bullets[i].collideWithAgent(_humans[j]))
				{
					//Damage the human and kill it if it's out of health
					if (_humans[j]->applyDamage(_bullets[i].getDamage()))
					{
						static std::mt19937 randomEngine((unsigned int)time(nullptr));
						std::uniform_int_distribution<int> turn(0, 100);
						if (turn(randomEngine) <= 25)
						{
							_zombies.push_back(new Zombie);
							_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition(), 20.0f);
						}
						//If the human died remove it
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else
					{
						j++;
					}
					//Remove the bullet
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					wasBulletRemoved = true;
					i--; //We don't want to skip a bullet as one has just been deleted
					//Since there is no more bullet it would be a waste of CPU ticks to keep iterating through the humans
					break;
				}
				else
				{
					j++;
				}
			}
		}
	}
}

int MainGame::checkConditions()
{
	//If all zombies are dead then the game is over
	if (_zombies.empty())
	{
		std::printf("*** You Win the Level! ***\nYou killed:\n %d Humans\n %d Zombies\nThere are %d/%d Humans Left\n\n\n", _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		deInitLevel();
		_currentLevel++;
		if (!initLevel("level" + std::to_string(_currentLevel) + ".txt"))
		{
			exitGame("Game over");
			return -1;
		}
	}
	return 0;
}

void MainGame::processInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {	
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
        case SDL_QUIT:
            exitGame("Quitting game");
		}
	}
    if (_inputManager.isKeyPressed(SDLK_LSHIFT))
    {
        if (_player->getSpeed() <= PLAYER_SPEED + PLAYER_SPRINT_SPEED)
            _player->setSpeed(PLAYER_SPEED + PLAYER_SPRINT_SPEED);
    }
    else
    {
        _player->setSpeed(PLAYER_SPEED);
    }
	if (_inputManager.isKeyPressed(SDLK_ESCAPE))
	{
		exitGame("Quitting Game");
	}
}

void MainGame::drawGame()
{
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Drawing the game
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	///Draw the current level
	_levels[_currentLevel]->draw();

	//Begin drawing agents
	_agentSpriteBatch.begin();

    // Dimensions of the agents
    const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	///Draw all humans
	for (unsigned int i = 0; i < _humans.size(); i++)
	{
        if (_camera.isBoxInView(_humans[i]->getPosition(), agentDims))
        {
            _humans[i]->draw(_agentSpriteBatch);
        }
		
	}

	///Draw all zombies
	for (unsigned int i = 0; i < _zombies.size(); i++)
	{
        if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDims))
        {
    		_zombies[i]->draw(_agentSpriteBatch);
        }
	}

	///Draw bullets
	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
        if (_camera.isBoxInView(_bullets[i].getPosition(), glm::vec2((float)BULLET_RADIUS)))
        {
		    _bullets[i].draw(_agentSpriteBatch);
        }
	}
	//Stop drawing agents
	_agentSpriteBatch.end();

	//Render all to screen
	_agentSpriteBatch.renderBatch();

	_textureProgram.unuse();
	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}

void MainGame::exitGame(std::string message)
{
	std::cout << "***" << message << "***\n\n" << std::endl; //Print message
	
	//Tell's SDL to close...
	SDL_Quit();
	_gameState = GameState::EXIT;
}