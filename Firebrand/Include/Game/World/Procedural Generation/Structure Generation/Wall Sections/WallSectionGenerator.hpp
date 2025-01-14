#ifndef __WALL_SECTION_GENERATOR_H__
#define __WALL_SECTION_GENERATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Room Designation/RoomDesignator.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include <set>

namespace WallSectionGenerator {

	typedef uint16_t WallSectionId;

	struct WallSectionCell : std::pair<RoomType, RoomType> {

		WallSectionCell(RoomType _first, RoomType _second, WallSectionId _sectionId);

		// Id of the wallSection that this cell is apart of
		WallSectionId sectionId;
	};

	using WallSectionGrid1D = std::vector<WallSectionCell>;
	using WallSectionGrid2D = std::vector<WallSectionGrid1D>;

	WallSectionGrid2D wallSectionsGet(const WallGrid2D wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize);
};

#endif