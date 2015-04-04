#pragma once

#include <IceEngine/SpriteBatch.h>
#include <glm/glm.hpp>
#include <vector>

class Human;
class Zombie;
class Agent;
const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed);
	~Bullet();

	//When update returns true then delete the bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	void draw(IceEngine::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return _damage; }

    glm::vec2 getPosition() { return _position; }
private:

	bool collidieWithWorld(const std::vector<std::string>& levelData);

	glm::vec2 _position;
	glm::vec2 _direction;
	float _damage;
	float _speed;
};

