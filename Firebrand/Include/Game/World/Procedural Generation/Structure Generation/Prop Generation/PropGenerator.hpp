#ifndef __PROP_GENERATOR_H__
#define __PROP_GENERATOR_H__

#include "../Grid/Room Designation Grid/RoomTypeGrid.hpp"
#include "../Prop Types/PropTypeRegistry.hpp"
#include "../Prop Types/PropTypes.hpp"
#include "../Room Types/RoomTypeRegistry.hpp"
#include "../Room Types/RoomTypes.hpp"
#include "../Wall Generation/WallGenerator.hpp"
#include <set>

namespace PropGenerator {
	void roomPropsGenerate(const WallGrid2D& wallGrid, const sf::Vector2u structureSize, RoomTypeGrid& roomTypeGrid, const RoomRect roomRect, const RoomType roomType);
	// generates all the props in all the rooms
	PropTypeGrid2D propsGenerate(const WallGrid2D& wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize, const RoomRectVector roomRectsVector);
};

#endif