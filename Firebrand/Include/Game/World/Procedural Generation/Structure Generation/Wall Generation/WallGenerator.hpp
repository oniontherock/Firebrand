#ifndef __WALL_GENERATOR_H__
#define __WALL_GENERATOR_H__

#include "../Grid/Structure Grid/StructureGrid.hpp"
#include "../Structure Types/StructureTypes.hpp"
#include <cstdint>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

typedef std::vector<bool> SurroundingWallsVector;

class StructureGenerator;


typedef sf::IntRect RoomRect;
typedef std::vector<RoomRect> RoomRectVector;

typedef std::vector<bool> WallGrid1D;
typedef std::vector<std::vector<bool>> WallGrid2D;

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
	static StructureCellType cellTypeGetFromWallType(WallType wallType);

	// returns a vector of bools, where each bool represents whether a face of the roomRect has a double wall.
	// the order is: right, top, left, bottom.
	static std::vector<bool> roomRectGetDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect);
	// returns whether a RoomRect has any double walls
	static bool roomRectHasDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect);
	// adjusts a RoomRect's dimensions to ensure no wall doubling occurs,
	// the roomRect's size may be adjusted to avoid doubling, the amount it can be resized can be controlled with resizeTolerance
	static RoomRect roomRectFixDoubleWalls(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect, const sf::Vector2u roomDesiredSize, const uint16_t resizeTolerance);

	static void roomWallsApplyToWallGrid(WallGrid2D& wallGrid, const sf::Vector2u structureSize, const RoomRect roomRect, const bool solid = false);

	// generates a room in the specified wallGrid,
	// 
	// @param wallGrid: the WallGrid2D to generate the room in.
	// @param roomSize: the size of the room that will be generated.
	// @param roomContactCount: the amount of faces of the room that need to be touching / on other walls.
	// @param fullContact: bool for how the contactCount is calculated, if true, a side of the room must have complete contact with other walls for it to count, if false, any contact on a side of the room counts
	static RoomRect roomGenerate(WallGrid2D& wallGrid, sf::Vector2u structureSize, sf::Vector2u roomSize, uint16_t roomContactCount, bool fullContact, uint16_t doubleWallTolerance);
	// generates the rooms in a wallGrid depending on the structure type
	static RoomRectVector roomsGenerate(WallGrid2D& wallGrid, StructureTypeBase* structureType, sf::Vector2u structureSize);

	// the RoomRectVector from the last call to wallsGenerate
	static RoomRectVector roomRectVectorLast;
	// returns the RoomRectVector from the last call to wallsGenerate
	static RoomRectVector roomsGetFromGeneration();

	static WallGrid2D wallsGenerate(StructureTypeBase* structuretype, sf::Vector2u structureSize);

	friend class StructureGenerator;
};

#endif