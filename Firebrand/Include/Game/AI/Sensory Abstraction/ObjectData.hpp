#ifndef __OBJECT_DATA_H__
#define __OBJECT_DATA_H__

#include "../include/Common/DataCache.hpp"
#include "../include/Game/World/Objects/ObjectTypes.hpp"
#include "../Teams/TeamRelationHolder.hpp"
#include <cstdint>
#include <ECS/Entities/Entity.hpp>

struct ObjectData {

	void dataClear();

	uint32_t timeMemorized = 0;

	EntityId objectId = 0;
	ObjectType objectType = ObjectType::Null;
	
	sf::Vector2f position;
	float rotation = 0.f;
	DataCache state;

	bool isAnimate = false;
	bool hasSenseSight = false;

	Teams::ThreatLevel threatLevel = Teams::ThreatLevel::Neutral;
};


#endif