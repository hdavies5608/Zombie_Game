#include "Player.h"

#include <SDL/SDL.h>
#include "Gun.h"
#include "Bullet.h"
Player::Player() :
_currentGunIndex(-1)
{
	//Empty
}


Player::~Player()
{
	//Empty
}

void Player::init(float speed, glm::vec2 pos, IceEngine::InputManager* inputManager, IceEngine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;
	_color.r = 119;
	_color.g = 158;
	_color.b = 203;
	_color.a = 255;
}

void Player::addGun(Gun* gun)
{
	//Add the gun to inventory
	_guns.push_back(gun);

	//If no gun equipped then equip gun
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string> &levelData,
	std::vector<Human*> &humans,
	std::vector<Zombie*> &zombies,
	float deltaTime)
{
	if (_inputManager->isKeyPressed(SDLK_w))
	{
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed * deltaTime;
	}

	if (_inputManager->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed * deltaTime;
	}
	else if (_inputManager->isKeyPressed(SDLK_d))
	{
		_position.x += _speed * deltaTime;
	}
	
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2)
	{
		_currentGunIndex = 2;
	}
	else if (_inputManager->isKeyPressed(SDLK_4) && _guns.size() >= 3)
	{
		_currentGunIndex = 3;
	}

	if (_currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreenCoordsToWorldCoords(mouseCoords);

		glm::vec2 centerPos = _position + AGENT_RADIUS;

		glm::vec2 direction = glm::normalize(mouseCoords - centerPos);
		_guns[_currentGunIndex]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT), centerPos, direction, *_bullets, deltaTime);
	}

	collideWithLevel(levelData);
}