#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Bullet.h"
class Gun
{
public:
	Gun(std::string name, unsigned int fireRate, unsigned int bulletsPerShot, float spread, unsigned int bulletDamage, float bulletSpeed);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

private:
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

	std::string _name;

	unsigned int _fireRate; ///< Fire rate in frames
	unsigned int _bulletsPerShot;
	float _spread; ///< The accuracy of the gun

	float _bulletSpeed;
	unsigned int _bulletDamage;

	float _frameCounter;
};

