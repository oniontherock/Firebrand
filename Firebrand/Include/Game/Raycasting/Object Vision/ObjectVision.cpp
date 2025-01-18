#include "ObjectVision.hpp"




ObjectVision::ObjectVision() {
	objectsSeenCreate();
}
ObjectVision::ObjectVision(OcclusionGrid* _occlusionGrid) :
	ObjectVision()
{
	occlusionGrid = _occlusionGrid;
}


void ObjectVision::objectGridSet(ObjectGrid* objectGridNew) {
	objectGrid = objectGridNew;
}

void ObjectVision::objectsSeenCreate() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectsSeenVector.push_back(std::vector<EntityId>());
	}
}
void ObjectVision::objectsSeenClear() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectsSeenVector.at(i).clear();
	}
}

void ObjectVision::update(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {
	rayObjectCellPositionPrev = sf::Vector2u(999999999u, 999999999u);

	objectsSeenClear();

	// cast the rays, updating the objectsSeenVector
	raysCast(castPosition, castAngle, coneSize, rayMaxDist, rayCount);
}

ObjectIdVector& ObjectVision::objectsSeenGet() {
	return objectsSeenVector;
}

bool ObjectVision::rayUpdate(sf::Vector2f rayPosition) {
	// check if ray position is in the object grid
	if (!objectGrid->worldPosIsInGridFull(rayPosition)) return false;
	
	// position of the object cell the ray is on
	sf::Vector2u rayObjectCellPosition = objectGrid->coordinatesWorldToCell(rayPosition);

	// check if ray's current ObjectCell position is the same as the ray's previous ObjectCellPosition
	if (rayObjectCellPosition != rayObjectCellPositionPrev) {
		rayObjectCellPositionPrev = rayObjectCellPosition;

		// set of EntityIds in the ray's cell
		std::set<EntityId> cellIdsSet = objectGrid->cellIdsGet(rayObjectCellPosition);
		// cellIdsSet converted to a vector
		std::vector<EntityId> cellIdsVector(cellIdsSet.begin(), cellIdsSet.end());

		// iterate over the cellIdsVector, adding ObjectIds to the objectsSeenVector.
		for (uint16_t i = 0; i < cellIdsVector.size(); i++) {
			// get the ObjectType of the current ObjectId
			uint16_t objectType = static_cast<uint16_t>(ObjectRegistry::entityObjectTypeGet(cellIdsVector[i]));

			// if the ObjectId is not already in the objectsSeenVector, add the ObjectId to the objectsSeenVector
			if (std::count(objectsSeenVector[objectType].begin(), objectsSeenVector[objectType].end(), cellIdsVector[i]) <= 0) objectsSeenVector[objectType].push_back(cellIdsVector[i]);
		}
	}
	return true;
}






