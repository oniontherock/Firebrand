#ifndef __GOAP_BLACKBOARD_H__
#define __GOAP_BLACKBOARD_H__

#include "../../Sensory Abstraction/ObjectAbstractor.hpp"
#include "../include/Common/DataCache.hpp"
#include <vector>

namespace Goap {
	struct Blackboard {

		Blackboard();

		ObjectAbstractor::ObjectDataVector objects;
		std::unordered_map<EntityId, ObjectAbstractor::ObjectDataIndex> objectIds;

		ObjectAbstractor::ObjectDataIndexVector Creatures;
		ObjectAbstractor::ObjectDataIndexVector Threats;
		ObjectAbstractor::ObjectDataIndexVector Allies;
		ObjectAbstractor::ObjectDataIndexVector Items;
		ObjectAbstractor::ObjectDataIndexVector Obstacles;

		template <typename T>
		void whiteDataSet(std::string dataKey, T dataValue) {
			whiteData.dataSet(dataKey, dataValue);
		}
		const DataUMap& whiteDataGet();

		ObjectAbstractor::ObjectDataIndex objectAdd(ObjectAbstractor::ObjectData& objectData);
		void objectRemove(ObjectAbstractor::ObjectDataIndex ind);
		bool objectHas(EntityId objectId) const;
		void objectUpdate(ObjectAbstractor::ObjectData& objectData);

	protected:
		std::set<ObjectAbstractor::ObjectDataIndex> availableObjectIds;

		// this data cache is used for data that is read only, and/or needs to be used in Action preconditions or effects
		DataCache whiteData;
	};
}

#endif