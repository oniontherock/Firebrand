#include "ObjectCell.hpp"
#include <cstdint>


void ObjectCell::idAdd(EntityId id) {
	idsSet.insert(id);
}
void ObjectCell::idRemove(EntityId id) {
	idsSet.erase(id);
}
bool ObjectCell::idHas(EntityId id) {
	return idsSet.count(id);
}
bool ObjectCell::hasAny() {
	return idsSet.size() > 0;
}
const std::set<EntityId>& ObjectCell::idsGet() {
	return idsSet;
}

