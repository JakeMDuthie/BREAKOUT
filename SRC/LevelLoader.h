#pragma once

// This class contains a .csv parser for taking level data, creating blocks, and returning a vector of those blocks to the play state

#include "GameObjects/Blocks.h"
#include <vector>

// includes for file writing
#include <fstream>
#include <iostream>

struct blockData
{
	float xPos;
	float yPos;
	int blockID;
};

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	std::vector<BaseBlockObject*> LoadLevel(std::string filename);

private:
	bool ParseFile(std::string filename);
	std::vector<BaseBlockObject*> BuildLevel();

	std::vector<blockData> levelBlocks_;

};