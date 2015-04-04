#pragma once

#include <vector>
#include <string>

#include <IceEngine/SpriteBatch.h>
const int TILE_WIDTH = 64;
class Level
{
public:
	//Load the level
	Level(const std::string& fileName, int levelNumber, int &stat);
	~Level();

	void draw();


	//Getter functions
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	int getNumHumans() const { return _numHumans; }
	glm::vec2 getStartPlayerPos() const { return _startPlayerPosition; }
	const std::vector<glm::vec2> getStartZombiePos() const { return _zombieStartPositions; }
	const std::vector<std::string>& getLevelData() const { return _levelData; }

private:
	std::vector<std::string> _levelData;
	int _numHumans;

	IceEngine::SpriteBatch _spriteBatch;

	glm::vec2 _startPlayerPosition;
	std::vector<glm::vec2> _zombieStartPositions;
};

