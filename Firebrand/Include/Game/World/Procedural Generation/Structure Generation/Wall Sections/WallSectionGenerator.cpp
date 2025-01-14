#include "WallSectionGenerator.hpp"
#include <queue>
#include "Auxiliary/VectorMath.hpp"
#include "Auxiliary/Math.hpp"

WallSectionGenerator::WallSectionCell::WallSectionCell(RoomType _first, RoomType _second, WallSectionId _sectionId) {
	first = _first;
	second = _second;
	sectionId = _sectionId;

	if (sectionId + 1 > sectionIdCount) sectionIdCount = sectionId + 1;
}

uint16_t WallSectionGenerator::sectionIdCount = 0;

WallSectionGenerator::WallSectionGrid2D WallSectionGenerator::wallSectionsGet(const WallGrid2D wallGrid, RoomTypeGrid& roomTypeGrid, const sf::Vector2u structureSize) {

	WallSectionGrid2D wallSectionGrid = WallSectionGrid2D(structureSize.x, WallSectionGrid1D(structureSize.y, WallSectionCell(RoomType::Null, RoomType::Null, 0)));

	WallSectionId wallSectionIdCur = 0;

	// iterate over structure's interior cells, as we don't need to do anything with the edges
	for (uint16_t cellX = 1; cellX < structureSize.x - 1; cellX++) {
		for (uint16_t cellY = 1; cellY < structureSize.y - 1; cellY++) {

			// skip if not a wall
			if (!wallGrid[cellX][cellY]) continue;
			// skip if a section has already been applied
			if (wallSectionGrid[cellX][cellY].first != RoomType::Null && wallSectionGrid[cellX][cellY].second != RoomType::Null) continue;

			// if there are no walls to the left and and right, the section must be vertical (this assumes it's a valid section, it's validity is evaluated later)
			bool sectionIsVertical = !(wallGrid[uint16_t(cellX - 1)][cellY] || wallGrid[uint16_t(cellX + 1)][cellY]);
			// if there are no walls above and below, the section must be horizontal (this assumes it's a valid section, it's validity is evaluated later)
			bool sectionIsHorizontal = !(wallGrid[cellX][uint16_t(cellY - 1)] || wallGrid[cellX][uint16_t(cellY + 1)]);

			// if just one of the section directions is true, then this is a valid wall section,
			// it's not valid if neither of the directions are true (I.E. just one lone cell), or if both of them are true (I.E. a wall intersection)
			bool isValidSection = (sectionIsVertical != sectionIsHorizontal);

			// skip if cell is not a valid section 
			if (!isValidSection) continue;

			// offset that this cell must apply to get neighboring cells in this cell's section.
			// basically, if the cell is a vertical section, the sideOffsets.first == (0, -1) and sideOffsets.second == (0, 1)
			std::pair<sf::Vector2i, sf::Vector2i> frontOffsets(sf::Vector2i(-1, 0), sf::Vector2i(1, 0));

			if (sectionIsVertical) {
				// rotate frontOffsets
				frontOffsets.first = Vector2iMath::rotate(frontOffsets.first, Mathf::PI / 2.f);
				frontOffsets.second = Vector2iMath::rotate(frontOffsets.second, Mathf::PI / 2.f);
			}

			// offset that this cell must apply to get neighboring, non-section cells.
			// this is set to frontOffsets but rotated by -90 degrees, which results in the offsets to get the normal vectors of the wall section.
			// note that the rotation could be +90 degrees, i just do -90 because we may rotate frontOffsets by +90 earlier, and i think it will be more consistent this way
			std::pair<sf::Vector2i, sf::Vector2i> sideOffsets(Vector2iMath::rotate(frontOffsets.first, -Mathf::PI / 2.f), Vector2iMath::rotate(frontOffsets.second, -Mathf::PI / 2.f));

			// get WallSectionCell using the sideOffsets and cellX and cellY.
			WallSectionCell cellBase = WallSectionCell(
				roomTypeGrid.cellGet(cellX + sideOffsets.first.x, cellY + sideOffsets.first.y).type,
				roomTypeGrid.cellGet(cellX + sideOffsets.second.x, cellY + sideOffsets.second.y).type,
				wallSectionIdCur
			);

			// cells in this section that still need to be evaluated
			std::queue<sf::Vector2u> cellsToEvaluate;
			// set of cells already evaluated
			std::set<sf::Vector2u, Vector2uLessThan> cellsEvaluated;

			cellsToEvaluate.push(sf::Vector2u(cellX, cellY));

			while (!cellsToEvaluate.empty()) {

				// position of the current cell in this section
				sf::Vector2u sectionCellPosition = cellsToEvaluate.front();
				cellsToEvaluate.pop();

				uint16_t sectionCellX = uint16_t(sectionCellPosition.x);
				uint16_t sectionCellY = uint16_t(sectionCellPosition.y);

				cellsEvaluated.insert(sf::Vector2u(sectionCellX, sectionCellY));

				// if there are no walls to the left and and right, the current cell section must be vertical
				bool sectionCellIsVertical = !(wallGrid[uint16_t(sectionCellX - 1)][sectionCellY] || wallGrid[uint16_t(sectionCellX + 1)][sectionCellY]);
				// if there are no walls above and below, the current cell section must be horizontal
				bool sectionCellIsHorizontal = !(wallGrid[sectionCellX][uint16_t(sectionCellY - 1)] || wallGrid[sectionCellX][uint16_t(sectionCellY + 1)]);

				// if sectionCell does not have the same section data as the initial section cell, then this is a different section, so we skip
				if ((sectionIsVertical != sectionCellIsVertical) || (sectionIsHorizontal != sectionCellIsHorizontal)) continue;

				// get WallSectionCell using the sideOffsets and cellX and cellY.
				WallSectionCell cellCur = WallSectionCell(
					roomTypeGrid.cellGet(sectionCellX + sideOffsets.first.x, sectionCellY + sideOffsets.first.y).type,
					roomTypeGrid.cellGet(sectionCellX + sideOffsets.second.x, sectionCellY + sideOffsets.second.y).type,
					wallSectionIdCur
				);

				// if the two room types for the current cell are not that of cellBase, then we have entered a different section, so we skip
				if ((cellCur.first != cellBase.first) || (cellCur.second != cellBase.second)) continue;

				// set the WallSectionCell in the wallSectionGrid if all the checks were passed
				wallSectionGrid[sectionCellX][sectionCellY] = cellCur;
				// add current cell to the set of evaluated cells

				// get the cell in front and behind this cell in the section
				sf::Vector2u cellFront = sf::Vector2u(sectionCellX + sideOffsets.first.x, sectionCellY + sideOffsets.first.y);
				sf::Vector2u cellBack = sf::Vector2u(sectionCellX + sideOffsets.second.x, sectionCellY + sideOffsets.second.y);

				bool cellFrontIsOnEdge = (cellFront.x <= 0 || cellFront.x >= structureSize.x - 1) || (cellFront.y >= 0 || cellFront.y >= structureSize.y - 1);
				bool cellBackIsOnEdge = (cellBack.x <= 0 || cellBack.x >= structureSize.x - 1) || (cellBack.y >= 0 || cellBack.y >= structureSize.y - 1);

				// if cellFront has yet to be evaluated, queue it for evaluation
				if (!cellsEvaluated.contains(cellFront) && !cellFrontIsOnEdge) cellsToEvaluate.push(cellFront);
				// if cellBack has yet to be evaluated, queue it for evaluation
				if (!cellsEvaluated.contains(cellBack) && !cellBackIsOnEdge) cellsToEvaluate.push(cellBack);
			}

			wallSectionIdCur++;
		}
	}

	return wallSectionGrid;
}
