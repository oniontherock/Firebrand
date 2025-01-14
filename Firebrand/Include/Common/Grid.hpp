#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <fstream>
#include <SFML/System/Vector2.hpp>
#include <iostream>

template <class C>
class Grid {
protected:

	using CellCoordinate = uint32_t;
	using CellVector = sf::Vector2<CellCoordinate>;
	using WorldCoordinate = float;
	using WorldVector = sf::Vector2<WorldCoordinate>;

	// 2d array of cells
	std::vector<std::vector<C>> cells;
	// amount of cells in the grid in the x and y axes
	sf::Vector2u gridSize;
	// total amount of cells in the grid
	uint32_t cellCount;
	// size of cells
	sf::Vector2f cellSize;
	// size of the whole grid, including the size of the cells
	sf::Vector2f gridSizeFull;

public:

	void cellsInitialize() {
		cells = std::vector<std::vector<C>>(gridSize.x, std::vector<C>(gridSize.y, C()));
	}
	template <typename... Args>
	void cellsInitialize(Args... args) {
		cells = std::vector<std::vector<C>>(gridSize.x, std::vector<C>(gridSize.y, C(args...)));
	}

	void cellsInitializeNoCopy() {
		std::vector<std::vector<C>> rows(gridSize.x);

		for (uint32_t x = 0; x < gridSize.x; x++) {

			std::vector<C> columns(gridSize.y);
			for (uint32_t y = 0; y < gridSize.y; y++) {
				columns[y] = C();
			}

			rows[x] = std::move(columns);
		}

		cells = std::move(rows);
	}
	template <typename... Args>
	void cellsInitializeNoCopy(Args... args) {
		std::vector<std::vector<C>> rows(gridSize.x);

		for (uint32_t x = 0; x < gridSize.x; x++) {

			std::vector<C> columns(gridSize.y);
			for (uint32_t y = 0; y < gridSize.y; y++) {
				columns[y] = C(args...);
			}

			rows[x] = std::move(columns);
		}

		cells = std::move(rows);
	}
	
	Grid() :
		gridSize(sf::Vector2u(0, 0)),
		cellCount(gridSize.x * gridSize.y),
		cellSize(sf::Vector2f(0, 0)),
		gridSizeFull(sf::Vector2f(gridSize.x * cellSize.x, gridSize.y * cellSize.y))
	{}

	Grid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
		gridSize(sf::Vector2u(gridSizeX, gridSizeY)),
		cellCount(gridSize.x * gridSize.y ),
		cellSize(sf::Vector2f(cellSizeX, cellSizeY)),
		gridSizeFull(sf::Vector2f(gridSize.x * cellSize.x, gridSize.y * cellSize.y))
	{}

	CellVector coordinatesWorldToCell(WorldCoordinate worldX, WorldCoordinate worldY) {
		return CellVector(static_cast<CellCoordinate>(trunc(worldX / cellSize.x)), static_cast<CellCoordinate>(trunc(worldY / cellSize.y)));
	}
	CellVector coordinatesWorldToCell(WorldVector worldPos) {
		return coordinatesWorldToCell(worldPos.x, worldPos.y);
	}
	
	WorldVector coordinatesCellToWorld(CellCoordinate cellX, CellCoordinate cellY) {
		return WorldVector(cellX * cellSize.x, cellY * cellSize.y);
	}
	WorldVector coordinatesCellToWorld(CellVector cellPos) {
		return coordinatesCellToWorld(cellPos.x, cellPos.y);
	}


	C& cellGet(CellCoordinate cellX, CellCoordinate cellY) {
		return cells[cellX][cellY];
	}
	C& cellGet(CellVector cellPos) {
		return cellGet(cellPos.x, cellPos.y);
	}
	C& cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
		return cellGet(coordinatesWorldToCell(worldX, worldY));
	}
	C& cellGetFromWorld(WorldVector worldPos) {
		return cellGetFromWorld(worldPos.x, worldPos.y);
	}

	void cellSet(CellCoordinate cellX, CellCoordinate cellY, C cell) {
		cells[cellX][cellY] = cell;
	}
	void cellSet(CellVector cellPos, C cell) {
		cells[cellPos.x][cellPos.y] = cell;
	}
	void cellSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, C cell) {
		CellVector cellPos = coordinatesWorldToCell(worldX, worldY);
		cells[cellPos.x][cellPos.y] = cell;
	}
	void cellSetFromWorld(WorldVector worldPos, C cell) {
		CellVector cellPos = coordinatesWorldToCell(worldPos);
		cells[cellPos.x][cellPos.y] = cell;
	}

	// uses ints instead of uints because the cells may go negative
	bool cellPosIsInGrid(int32_t cellX, int32_t cellY) {
		return !(cellX < 0 || cellX >= int32_t(gridSize.x) || cellY < 0 || cellY >= int32_t(gridSize.y));
	}
	bool cellPosIsInGrid(sf::Vector2i cellPos) {
		return cellPosIsInGrid(cellPos.x, cellPos.y);
	}
	bool cellPosIsInGrid(CellCoordinate cellX, CellCoordinate cellY) {
		return cellPosIsInGrid(int32_t(cellX), int32_t(cellY));
	}
	bool cellPosIsInGrid(CellVector cellPos) {
		return cellPosIsInGrid(cellPos.x, cellPos.y);
	}
	bool worldPosIsInGrid(WorldCoordinate worldX, WorldCoordinate worldY) {
		return cellPosIsInGrid(coordinatesWorldToCell(worldX, worldY));
	}
	bool worldPosIsInGrid(WorldVector worldPos) {
		return worldPosIsInGrid(worldPos.x, worldPos.y);
	}

	bool worldPosIsInGridFull(WorldCoordinate worldX, WorldCoordinate worldY) {
		return !(worldX < 0 || worldX >= gridSizeFull.x || worldY < 0 || worldY >= gridSizeFull.y);
	}
	bool worldPosIsInGridFull(WorldVector worldPos) {
		return worldPosIsInGridFull(worldPos.x, worldPos.y);
	}
	bool cellPosIsInGridFull(CellCoordinate cellX, CellCoordinate cellY) {
		return worldPosIsInGridFull(coordinatesCellToWorld(cellX, cellY));
	}
	bool cellPosIsInGridFull(CellVector cellPos) {
		return worldPosIsInGridFull(coordinatesCellToWorld(cellPos));
	}

	sf::Vector2u gridGetSize() {
		return gridSize;
	}
	uint32_t gridGetSizeX() {
		return gridSize.x;
	}
	uint32_t gridGetSizeY() {
		return gridSize.y;
	}

	sf::Vector2f gridGetSizeFull() {
		return gridSizeFull;
	}
	uint32_t gridGetSizeFullX() {
		return gridSizeFull.x;
	}
	uint32_t gridGetSizeFullY() {
		return gridSizeFull.y;
	}

	sf::Vector2f cellsGetSize() {
		return cellSize;
	}
	float cellsGetSizeX() {
		return cellSize.x;
	}
	float cellsGetSizeY() {
		return cellSize.y;
	}
};


#endif