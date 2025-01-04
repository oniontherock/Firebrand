#ifndef __WALL_GENERATOR_H__
#define __WALL_GENERATOR_H__

#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

typedef std::vector<bool> SurroundingWallsVector;

class StructureGenerator;

class WallGenerator {

	enum WallType {
		None,
		Single,
		Straight,
		Corner,
		JunctionT,
		JunctionPlus,
	};

	static std::pair<WallType, float> wallDataGetFromSurroundings(SurroundingWallsVector wallStates);
	static const char* cellTypeGetFromWallType(WallType wallType);

	static std::vector<std::vector<bool>> wallsGenerate();

public:
	friend class StructureGenerator;
};

#endif