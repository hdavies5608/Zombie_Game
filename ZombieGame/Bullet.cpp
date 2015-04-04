#include "Bullet.h"

#include <IceEngine/ResourceManager.h>
#include <IceEngine/Vertex.h>

#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed):
_position(pos),
_direction(dir),
_damage(damage),
_speed(speed)
{
	//Empty
}


Bullet::~Bullet()
{
	//Empty
}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;
	return collidieWithWorld(levelData);
}

void Bullet::draw(IceEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	IceEngine::ColorRGBA8 color;
	color.r = 41;
	color.b = 46;
	color.g = 45;
	color.a = 255;
	spriteBatch.draw(destRect, uvRect, IceEngine::ResourceManager::getTexture("Textures/circle.png").ID, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collidieWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPos;
	gridPos.x = (int)floor(_position.x / (float)TILE_WIDTH);
	gridPos.y = (int)floor(_position.y / (float)TILE_WIDTH);

	if (gridPos.x < 0 || (unsigned)gridPos.x >= levelData[0].length() ||
		gridPos.y < 0 || (unsigned)gridPos.y >= levelData.size())
	{
		return true;
	}

	return (levelData[gridPos.y][gridPos.x] != '.');
}