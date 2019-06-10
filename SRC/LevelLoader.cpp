#include "LevelLoader.h"

LevelLoader::LevelLoader()
{
}

LevelLoader::~LevelLoader()
{
}

// call this function and return the loaded level
std::vector<BaseBlockObject*> LevelLoader::LoadLevel(std::string filename)
{

	// reset levelBlocks if required
	if (!levelBlocks_.empty())
	{
		levelBlocks_.clear();
	}

	// parse file
	ParseFile(filename);

	return BuildLevel();
}

// call this function to parse the .csv file
bool LevelLoader::ParseFile(std::string filename)
{
	std::ifstream level;
	
	level.open(filename.c_str(), std::ifstream::in);

	if (!level)
	{
		// failed to load
		std::cout << "Failed to open file : " << filename << std::endl;
		return false;
	}
	else
	{
		// success
		std::cout << "Loading : " << filename << std::endl;
	}

	// file imported successfully, let's parse
	int index = 0;

	std::string str;
	blockData newDataPoint;

	// loop through entire file
	while (std::getline(level, str, ','))
	{

		if (index % 3 == 0)
		{
			// xPos
			newDataPoint.xPos = std::stof(str);
		}
		else if (index % 3 == 1)
		{
			// yPos
			newDataPoint.yPos = std::stof(str);
		}
		else if (index % 3 == 2)
		{
			// block ID
			newDataPoint.blockID = std::stoi(str);

			// data point complete, add to vector
			levelBlocks_.push_back(newDataPoint);
		}

		index++;
	}

	level.close();

	return true;
}

// build the level data from the parsed file
std::vector<BaseBlockObject*> LevelLoader::BuildLevel()
{
	std::vector<BaseBlockObject*> blocks;

	for (auto blockData : levelBlocks_)
	{
		BaseBlockObject* nextBlock;

		switch (blockData.blockID)
		{
		case 0 :
			nextBlock = new StandardBlock(sf::Vector2f(blockData.xPos,blockData.yPos));
			break;
		case 1 :
			nextBlock = new ChangerBlock(sf::Vector2f(blockData.xPos, blockData.yPos));
			break;
		case 2:
			nextBlock = new SplitterBlock(sf::Vector2f(blockData.xPos, blockData.yPos));
			break;
		default:
			nextBlock = new StandardBlock(sf::Vector2f(blockData.xPos, blockData.yPos));
			break;
		}

		blocks.push_back(nextBlock);
	}

	return blocks;
}