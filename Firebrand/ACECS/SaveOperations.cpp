#include "SaveOperations.hpp"
#include "ECSRegistry.hpp"

std::ofstream& operator<< (std::ofstream& str, std::string& item) {
	size_t itemLen = item.size();
	str << itemLen;

	str.write(item.data(), itemLen * sizeof(char));

	return str;
}
std::ifstream& operator>> (std::ifstream& str, std::string& item) {
	size_t itemLen;
	str >> itemLen;
	item.resize(itemLen);

	str.read(item.data(), itemLen * sizeof(char));

	return str;
}


std::ofstream& operator<< (std::ofstream& str, GameLevel& item) {

	str << item.levelSize;
	//str << item.dynamicSpriteEntityIds;
	//str << item.staticSpriteEntityIds;
	//str << item.firstRun;
	//str << item.distortionGrid;
	str << item.objectGrid;
	str << item.aStarGrid;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str >> item.levelSize;
	//str >> item.dynamicSpriteEntityIds;
	//str >> item.staticSpriteEntityIds;
	//str >> item.firstRun;
	//str >> item.distortionGrid;
	str >> item.objectGrid;
	str >> item.aStarGrid;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarCell& item) {
	str << item.cellPositionGrid;
	str << item.cellPositionWorld;
	str << item.costF;
	str << item.costG;
	str << item.costH;
	str << item.valid;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarCell& item) {
	str >> item.cellPositionGrid;
	str >> item.cellPositionWorld;
	str >> item.costF;
	str >> item.costG;
	str >> item.costH;
	str >> item.valid;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectCell& item) {
	str << item.idsSet;
	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectCell& item) {
	str >> item.idsSet;
	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

#pragma region Components

void EntityComponents::ComponentPosition::save(std::ofstream& str) {
	str << position;
}
void EntityComponents::ComponentPosition::load(std::ifstream& str) {
	str >> position;
}

void EntityComponents::ComponentSprite::save(std::ofstream& str) {
	str << fileName;
	str << fileExtension;
}
void EntityComponents::ComponentSprite::load(std::ifstream& str) {
	str >> fileName;
	str >> fileExtension;
	textureInitialize();
}

void EntityComponents::ComponentMoveByInput::save(std::ofstream& str) {
	str << moveSpeed;
}
void EntityComponents::ComponentMoveByInput::load(std::ifstream& str) {
	str >> moveSpeed;
}

void EntityComponents::ComponentRotateToMouse::save(std::ofstream& str) {
	str << turnSpeed;
}
void EntityComponents::ComponentRotateToMouse::load(std::ifstream& str) {
	str >> turnSpeed;
}

void EntityComponents::ComponentViewFollow::save(std::ofstream& str) {
	str << panelViewsToFollow;
}
void EntityComponents::ComponentViewFollow::load(std::ifstream& str) {
	str >> panelViewsToFollow;
}

void EntityComponents::ComponentObjectGridInhabiterRadius::save(std::ofstream& str) {
	str << radius;
	str << positionPrev;
}
void EntityComponents::ComponentObjectGridInhabiterRadius::load(std::ifstream& str) {
	str >> radius;
	str >> positionPrev;
}

void EntityComponents::ComponentRotation::save(std::ofstream& str) {
	str << rotation;
}
void EntityComponents::ComponentRotation::load(std::ifstream& str) {
	str >> rotation;
}

void EntityComponents::ComponentObjectTypeAssigner::save(std::ofstream& str) {
	str << objectType;
}
void EntityComponents::ComponentObjectTypeAssigner::load(std::ifstream& str) {
	str >> objectType;
}

#pragma endregion