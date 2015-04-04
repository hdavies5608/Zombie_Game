#include "Zombie.h"
#include "Human.h"
#include <IceEngine/Vertex.h>
#include <random>
#include <ctime>
Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos, float health)
{
	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_int_distribution<int> special(0, 100);
	if (special(randomEngine) >= 99)
	{
		_speed = 0.5;
		_position = pos;
		_color.r = 255;
		_color.g = 179;
		_color.b = 71;
		_color.a = 255;
		_health = 1000.0f;
	}
	else if (special(randomEngine) <= 5)
	{
		_speed = 5.0f;
		_position = pos;
		_color.r = 194;
		_color.g = 59;
		_color.b = 34;
		_color.a = 255;
		_health = 50.0f;
	}
	else
	{
		_speed = speed;
		_position = pos;

		_color = IceEngine::ColorRGBA8(119, 190, 119, 255);
		_health = health;
	}
}

void Zombie::update(const std::vector<std::string> &levelData,
	std::vector<Human*> &humans,
	std::vector<Zombie*> &zombies,
	float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr)
	{
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed * deltaTime;
	}

	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*> &humans)
{
	Human * closestHuman = nullptr;
	float smallestDistance = 99999999999999.0f;

	for (unsigned int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}