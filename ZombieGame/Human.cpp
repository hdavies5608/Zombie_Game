#include "Human.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
Human::Human()
	:_rotationFrames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_speed = speed;
	_position = pos;
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	_color.r = 150;
	_color.g = 111;
	_color.b = 214;
	_color.a = 255;

	//We don't want the direction to be zero or the humans won't move
	if (_direction.length() == 0) _direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);
	_health = 20.0f;
}

void Human::update(const std::vector<std::string> &levelData,
	std::vector<Human*> &humans,
	std::vector<Zombie*> &zombies,
	float deltaTime)
{
	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	static std::uniform_real_distribution<float> randRot(-40.0f, 40.0f);
	static std::uniform_int_distribution<int> rotFrameNum(20, 120);

	_position += _direction * _speed * deltaTime;
	//Randomy change direction every X frames
	if (_rotationFrames >= rotFrameNum(randomEngine))
	{
		_direction = glm::rotate(_direction, randRot(randomEngine));
		_rotationFrames = 0;
	}
	else
	{
		_rotationFrames++;
	}
	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRot(randomEngine));
	}
}