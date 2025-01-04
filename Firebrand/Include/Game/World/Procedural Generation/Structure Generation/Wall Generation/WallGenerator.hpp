#ifndef __WALL_GENERATOR_H__
#define __WALL_GENERATOR_H__

#include "../Grid/StructureCell.hpp"
#include "../StructureTypes.hpp"
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

typedef std::vector<bool> SurroundingWallsVector;

class StructureGenerator;

// class specialized in generating wall grids, used by the StructureGrid.
// this class really only exists so we don't have a ridiculous amount of wall related stuff in StructureGrid.
class WallGenerator {

	enum WallType {
		None,
		Single,
		Straight,
		Corner,
		JunctionT,
		JunctionPlus,
	};
	// gets wall data from surroundings,
	// so for example, it checks for straight walls vertically or horizontally, and returns a rotation and type according to that,
	// it does a bunch of checks like that.
	static std::pair<WallType, float> wallDataGetFromSurroundings(SurroundingWallsVector wallStates);
	// returns a wall
	static StructureCellType cellTypeGetFromWallType(WallType wallType);


	typedef std::vector<bool> WallGrid1D;
	typedef std::vector<std::vector<bool>> WallGrid2D;

	typedef std::pair<sf::Vector2u, sf::Vector2u> RoomSizePair;
	typedef std::pair<uint16_t, uint16_t> RoomCountPair;
	typedef std::pair<uint16_t, uint16_t> RoomContactCountPair;

	// returns the minimum and maximum size for rooms depending on a structure's type
	static RoomSizePair roomMinMaxSizeGetFromType(StructureType structureType);
	// returns the minimum and maximum amount of possible rooms in a structure depending on a structure's type
	static RoomCountPair roomMinMaxCountGetFromType(StructureType structureType);
	// returns the minimum and maximum amount of possible rooms a room must be contacting to be placed at a position depending on a structure's type
	static RoomContactCountPair roomMinMaxContactCountGetFromType(StructureType structureType);


	// returns a random size for a room in a building depending on the type
	static sf::Vector2u roomSizeGetFromType(StructureType structureType);
	// returns a random count of rooms in a building depending on the type
	static uint16_t roomCountGetFromType(StructureType structureType);
	// returns a random count of rooms that a room must contact to be valid depending on the type
	static uint16_t roomContactCountGetFromType(StructureType structureType);

	// generates a room in the specified wallGrid,
	// 
	// @param wallGrid: the WallGrid2D to generate the room in.
	// @param roomSize: the size of the room that will be generated.
	// @param roomContactCount: the amount of faces of the room that need to be touching / on other walls.
	static sf::IntRect roomGenerate(WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u roomSize, uint16_t roomContactCount);
	// generates the rooms in a wallGrid depending on the structure type
	static void roomsGenerate(WallGrid2D& wallGrid, StructureType structureType, sf::Vector2u structureSize);

	static WallGrid2D wallsGenerate(StructureType structureType, sf::Vector2u structureSize);

	friend class StructureGenerator;
};

#endif