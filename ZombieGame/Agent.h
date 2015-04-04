#pragma once

#include <glm/glm.hpp>

#include <IceEngine/SpriteBatch.h>

#include <vector>
#include <string>

class Human;
class Zombie;

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH * 0.5f;
class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string> &levelData,
		std::vector<Human*> &humans,
		std::vector<Zombie*> &zombies,
		float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string> &levelData);

	bool collideWithAgent(Agent* agent);

	void draw(IceEngine::SpriteBatch& _spriteBatch);

	///Retun true if we died
	bool applyDamage(float damage);

	glm::vec2 getPosition() const { return _position; }

	float getSize() const { return AGENT_WIDTH; }

protected:

	void checkTilePosition(const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	float _speed;
	IceEngine::ColorRGBA8 _color;
	float _health;
};

