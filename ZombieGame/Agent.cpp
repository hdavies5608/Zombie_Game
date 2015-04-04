#include "Agent.h"
#include "Level.h"

#include <IceEngine/ResourceManager.h>
#include <algorithm>
Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const std::vector<std::string> &levelData)
{
	std::vector<glm::vec2> coillideTilePositions;

	//Check the four corners
	//First corner
	checkTilePosition(levelData, coillideTilePositions, _position.x, _position.y);
	checkTilePosition(levelData, coillideTilePositions, _position.x + AGENT_WIDTH, _position.y);
	checkTilePosition(levelData, coillideTilePositions, _position.x, _position.y + AGENT_WIDTH);
	checkTilePosition(levelData, coillideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

	if (coillideTilePositions.size() == 0)
	{
		//No collisions
		return false; 
	}

	for (unsigned int i = 0; i < coillideTilePositions.size(); i++)
	{
		collideWithTile(coillideTilePositions[i]);
	}
	 
	//A collision has occured
	return true;
}

bool Agent::collideWithAgent(Agent* agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + AGENT_RADIUS;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE-distance;

	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVect = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVect * 0.5f;
		agent->_position -= collisionDepthVect * 0.5f;
		return true;
	}
	return false;
}

void Agent::draw(IceEngine::SpriteBatch& _spriteBatch)
{
	static int textureID = IceEngine::ResourceManager::getTexture("./Textures/circle.png").ID;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;


	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

bool Agent::applyDamage(float damage)
{
	_health -= damage;
	if (_health <= 0)
	{
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePositions, float x, float y)
{
	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	//We want to exit the function here if an agent has been spawned outside of our level
	if (gridPos.x < 0 || gridPos.x >= levelData[0].length() ||
		gridPos.y < 0 || gridPos.y >= levelData.size())
	{
		return;
	}

	// If this is not an air tile, we should collide with it
	if (levelData[(unsigned)gridPos.y][(unsigned)gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

void Agent::collideWithTile(glm::vec2 tilePos)
{
	
	const float TILE_RADIUS = TILE_WIDTH * 0.5f;
	// The minimum distance before a collision occurs
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	// Center position of the agent
	glm::vec2 centerAgentPos = _position + glm::vec2(AGENT_RADIUS);
	// Vector from the agent to the tile
	glm::vec2 distVec = centerAgentPos - tilePos;

	// Get the depth of the collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	// If either the depths are > 0, then we collided
	if (xDepth > 0 && yDepth > 0) {

		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			// X collsion depth is smaller so we push in X direction
			if (distVec.x < 0) {
				_position.x -= xDepth;
			}
			else {
				_position.x += xDepth;
			}
		}
		else {
			// Y collsion depth is smaller so we push in X direction
			if (distVec.y < 0) {
				_position.y -= yDepth;
			}
			else {
				_position.y += yDepth;
			}
		}
	}
}