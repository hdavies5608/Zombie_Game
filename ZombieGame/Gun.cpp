#include "Gun.h"
#include <random>
#include <ctime>
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, unsigned int fireRate, unsigned int bulletsPerShot, float spread, unsigned int bulletDamage, float bulletSpeed) :
_name(name),
_fireRate(fireRate),
_bulletsPerShot(bulletsPerShot),
_spread(spread),
_bulletDamage(bulletDamage),
_bulletSpeed(bulletSpeed),
_frameCounter((float)_fireRate)
{
	///Empty
}


Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	if (_frameCounter >= _fireRate && isMouseDown)
	{
		fire(direction, position, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	for (unsigned int i = 0; i < _bulletsPerShot; i++)
	{
		static std::mt19937 randomEngine((unsigned int)time(nullptr));
		std::uniform_real_distribution<float> randRot(-_spread, _spread);

		bullets.emplace_back(position, glm::rotate(direction, randRot(randomEngine)), _bulletDamage, _bulletSpeed);
	}
}