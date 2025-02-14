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
	str << item.pathGenerator;
	str << item.objectGrid;
	str << item.aStarGrid;
	str << item.hasUpdated;
	str << item.pathPoints;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str >> item.levelSize;
	//str >> item.dynamicSpriteEntityIds;
	//str >> item.staticSpriteEntityIds;
	//str >> item.firstRun;
	//str >> item.distortionGrid;
	str >> item.pathGenerator;
	str >> item.objectGrid;
	str >> item.aStarGrid;
	str >> item.hasUpdated;
	str >> item.pathPoints;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, PathGenerator& item) {

	size_t pathPointsSize = item.pathPoints.size();
	str << pathPointsSize;
	for (uint32_t i = 0; i < item.pathPoints.size(); i++) {
		str << *item.pathPoints[i];
	}
	size_t connectionsSize = item.connections.size();
	str << connectionsSize;
	for (uint32_t i = 0; i < item.connections.size(); i++) {
		str << item.connections[i].first;
		str << item.connections[i].second;
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, PathGenerator& item) {

	size_t pathPointsSize;
	str >> pathPointsSize;
	item.pathPoints.resize(pathPointsSize);
	for (uint32_t i = 0; i < item.pathPoints.size(); i++) {
		PathPoint* pathPoint = new PathPoint();
		str >> *pathPoint;
		item.pathPoints[i] = pathPoint;
	}
	size_t connectionsSize;
	str >> connectionsSize;
	item.connections.resize(connectionsSize);
	for (uint32_t i = 0; i < item.connections.size(); i++) {
		str >> item.connections[i].first;
		str >> item.connections[i].second;
	}

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

std::ofstream& operator<< (std::ofstream& str, Entity& item) {
	str << item.updateType;
	str << item.levelId;

	for (EntityComponents::ComponentTypeID componentIdCur = 0; componentIdCur < EntityComponents::totalComponents; componentIdCur++) {
		if (item.entityComponentHasAtIndex(componentIdCur)) {
			str << componentIdCur;
			item.entityComponentGetAtIndex(componentIdCur)->save(str);
		}
	}
	// put in super high value so that the component loading loop exits
	str << EntityComponents::totalComponents;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, Entity& item) {
	str >> item.updateType;
	str >> item.levelId;

	EntityComponents::ComponentTypeID componentIdCur;
	str >> componentIdCur;
	do {

		// get the component of the current type that the entity saved.
		EntityComponents::Component* componentToDuplicate = EntityComponents::componentsAll[componentIdCur].get();
		// duplicate the component that the entity saved
		ComponentUniquePtr componentDuplicated = Duplicatable::duplicateAndConvertToType<EntityComponents::Component>(componentToDuplicate);
		// raw pointer to the duplicate that was duplicated
		EntityComponents::Component* componentDuplicatedRaw = componentDuplicated.get();
		// release unique ptr to duplicated component
		componentDuplicated.release();

		// load the data that the entity saved for this component
		componentDuplicatedRaw->load(str);

		// add the duplicated type to the entity
		item.entityComponentAddAtIndex(componentDuplicatedRaw, componentIdCur);

		str >> componentIdCur;
	} while (componentIdCur < EntityComponents::totalComponents);

	return str;
}

std::ofstream& operator<< (std::ofstream& str, CollisionShapeBase& item) {
	str << item.rotation;
	str << item.center;
	str << item.shapeMaxDist;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, CollisionShapeBase& item) {
	str >> item.rotation;
	str >> item.center;
	str >> item.shapeMaxDist;

	return str;
}
std::ofstream& operator<< (std::ofstream& str, CollisionShapePolygon& item) {
	str << item.rotation;
	str << item.center;
	str << item.shapeMaxDist;
	str << item.vertices;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, CollisionShapePolygon& item) {
	str >> item.rotation;
	str >> item.center;
	str >> item.shapeMaxDist;
	str >> item.vertices;

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
	str << isDynamic;
	str << drawOrder;
}
void EntityComponents::ComponentSprite::load(std::ifstream& str) {
	str >> fileName;
	str >> fileExtension;
	str >> isDynamic;
	str >> drawOrder;
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

void EntityComponents::ComponentObjectVision::save(std::ofstream& str) {
	str << cooldownVisionUpdate;
}
void EntityComponents::ComponentObjectVision::load(std::ifstream& str) {
	str >> cooldownVisionUpdate;
}
void EntityComponents::ComponentSpriteColor::save(std::ofstream& str) {
	str << color;
}
void EntityComponents::ComponentSpriteColor::load(std::ifstream& str) {
	str >> color;
}
void EntityComponents::ComponentSpriteOrigin::save(std::ofstream& str) {
	str << origin;
}
void EntityComponents::ComponentSpriteOrigin::load(std::ifstream& str) {
	str >> origin;
}
void EntityComponents::ComponentSpriteFlip::save(std::ofstream& str) {
	str << flipX;
	str << flipY;
}
void EntityComponents::ComponentSpriteFlip::load(std::ifstream& str) {
	str >> flipX;
	str >> flipY;
}
void EntityComponents::ComponentOcclusionRadius::save(std::ofstream& str) {
	str << radius;
}
void EntityComponents::ComponentOcclusionRadius::load(std::ifstream& str) {
	str >> radius;
}
void EntityComponents::ComponentOcclusionRectangles::save(std::ofstream& str) {
	str << rectangles;
}
void EntityComponents::ComponentOcclusionRectangles::load(std::ifstream& str) {
	str >> rectangles;
}
void EntityComponents::ComponentObjectGridInhabiterRectangles::save(std::ofstream& str) {
	str << rectangles;
}
void EntityComponents::ComponentObjectGridInhabiterRectangles::load(std::ifstream& str) {
	str >> rectangles;
}
void EntityComponents::ComponentCollisionPolygons::save(std::ofstream& str) {

	size_t shapesSize = shapes.size();
	str << shapesSize;
	for (uint16_t i = 0; i < shapes.size(); i++) {
		str << shapes[i];
	}

	str << shapesMaxDist;
}
void EntityComponents::ComponentCollisionPolygons::load(std::ifstream& str) {
	
	size_t shapesSize;
	str >> shapesSize;
	shapes.resize(shapesSize);
	for (uint16_t i = 0; i < shapes.size(); i++) {

		str >> shapes[i];
	}

	shapesPtrs.resize(shapes.size());

	for (uint16_t i = 0; i < shapes.size(); i++) {
		shapesPtrs[i] = &shapes[i];
	}

	str >> shapesMaxDist;
}
void EntityComponents::ComponentBatchSprite::save(std::ofstream& str) {
	str << fileName;
	str << fileExtension;
}
void EntityComponents::ComponentMass::save(std::ofstream& str) {
	str << mass;
}
void EntityComponents::ComponentMass::load(std::ifstream& str) {
	str >> mass;
}
void EntityComponents::ComponentBatchSprite::load(std::ifstream& str) {
	str >> fileName;
	str >> fileExtension;
}
void EntityComponents::ComponentHingeOnPoint::save(std::ofstream& str) {
	str << hingeOffset;
	str << hingePoint;
}
void EntityComponents::ComponentHingeOnPoint::load(std::ifstream& str) {
	str >> hingeOffset;
	str >> hingePoint;
}
void EntityComponents::ComponentObserver::save(std::ofstream& str) {
	str << observationDistance;
}
void EntityComponents::ComponentObserver::load(std::ifstream& str) {
	str >> observationDistance;
}

#pragma endregion