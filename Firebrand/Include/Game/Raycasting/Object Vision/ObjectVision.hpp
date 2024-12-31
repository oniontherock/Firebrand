#ifndef __OBJECT_VISION_H__
#define __OBJECT_VISION_H__

#include "../../World/Objects/ObjectRegistry.hpp"
#include "../../World/Objects/ObjectTypes.hpp"
#include "../ACECS/GameLevel.hpp"
#include "../RayCaster.hpp"
#include <cstdint>
#include <ECS/TypeDefinitions.hpp>
#include <set>
#include <vector>

// vector of vectors of EntityIds,
// entities are placed into the sub-vectors based off of their ObjectType.
// so if an entity is an object of type Door, which is, for example, at position 4 in the ObjectType enum, the entity's Id will be placed in sub-vector 4.
typedef std::vector<std::vector<EntityId>> ObjectIdVector;

struct ObjectVision : public RayCaster {

	ObjectVision();

	void objectGridSet(ObjectGrid* objectGridNew);
	void update(sf::Vector2f castPosition, float castAngle, float coneSize, float rayMaxDist, uint32_t rayCount) override;

	ObjectIdVector& objectsSeenGet();

	friend std::ofstream& operator<<(std::ofstream& str, ObjectVision& item);
	friend std::ifstream& operator>>(std::ifstream& str, ObjectVision& item);

protected:

	// set of EntityIdObjectTypePairs
	ObjectIdVector objectsSeenVector;
	// the object grid to sample
	ObjectGrid* objectGrid;
	// rayObjectCell of last rayUpdate, used so that the same cell isn't check twice (or more) in two (or more) subsequent steps 
	sf::Vector2u rayObjectCellPositionPrev = sf::Vector2u(999999999u, 999999999u);


	void objectsSeenCreate();
	void objectsSeenClear();
	// checks for any ObjectIds at the rays position and adds them to objectsSeenVector if there are any
	bool rayUpdate(sf::Vector2f rayPosition) override;
};

#endif