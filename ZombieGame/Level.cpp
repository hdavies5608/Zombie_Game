#include "Level.h"

#include <IceEngine/Errors.h>
#include <IceEngine/ResourceManager.h>
#include <SDL/SDL.h>
#include <fstream>
#include <iostream>

Level::Level(const std::string& fileName, int levelNumber, int &stat)
{

	std::ifstream file;
	file.open(fileName);
	//Check for errors
	if (file.fail() && levelNumber == 0)
	{
		IceEngine::FATAL_ERROR("Failed to load the level file " + fileName);
	}
	else if (file.fail())
	{
        stat = -1;
	}

	//Throw away the first string in tmp
	std::string tmp;
	file >> tmp >> _numHumans;

	std::getline(file, tmp); ///< Throw away the rest of the line
	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}

	_spriteBatch.init();

	_spriteBatch.begin();

	glm::vec4 uvRect(0, 0, 1, 1);
	IceEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	//Render all the tiles
	for (unsigned int y = 0; y < _levelData.size(); y++)
	{
		for (unsigned int x = 0; x < _levelData[y].size(); x++)
		{
			//Grab the tile
			char tile = _levelData[y][x];
			//Get dest rect
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			switch (tile)
			{
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, 
									uvRect, 
									IceEngine::ResourceManager::getTexture("Textures/red_bricks.png").ID,
									0.0f,
									whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
					uvRect,
					IceEngine::ResourceManager::getTexture("Textures/glass.png").ID,
					0.0f,
					whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
					uvRect,
					IceEngine::ResourceManager::getTexture("Textures/light_bricks.png").ID,
					0.0f,
					whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPosition.x = x*(float)TILE_WIDTH;
				_startPlayerPosition.y = y*(float)TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected Symbol %c at (%d,%d)\n", tile, x, y);
				break;
			}
		}
	}
	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}