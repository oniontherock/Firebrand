#ifndef __OBJECT_CELL_H__
#define __OBJECT_CELL_H__

#include <Auxiliary/TimeHandler.hpp>
#include <Auxiliary/Cooldown.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <ECS/TypeDefinitions.hpp>
#include <set>
#include <vector>

// an object that contains a list of EntityIds.
// for moving objects, they must remove their previous position from the object grid,
// move, and add their new position.
struct ObjectCell {

	// adds an EntityId to the idsVector
	void idAdd(EntityId id);
	// removes an EntityId from the idsVector
	void idRemove(EntityId id);
	// check if the cell contains the id
	bool idHas(EntityId id);
	// returns whether the cell has any ids
	bool hasAny();
	// returns a const reference to the idsVector
	const std::set<EntityId>& idsGet();

	friend std::ofstream& operator<<(std::ofstream& str, ObjectCell& item);
	friend std::ifstream& operator>>(std::ifstream& str, ObjectCell& item);

private:
	// vector of EntityIds,
	std::set<EntityId> idsSet;
};

#endif