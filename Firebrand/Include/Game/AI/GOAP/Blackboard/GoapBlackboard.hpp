#ifndef __GOAP_BLACKBOARD_H__
#define __GOAP_BLACKBOARD_H__

#include "../../Sensory Abstraction/ObjectAbstractor.hpp"
#include "../include/Common/DataCache.hpp"
#include <vector>

namespace BlackboardWhiteDataManager {
	void whiteDataObtain(Goap::Blackboard& blackboard);
}

namespace Goap {
	struct Blackboard {

		Blackboard();

		ObjectAbstractor::ObjectDataVector objects;
		std::unordered_map<EntityId, ObjectAbstractor::ObjectDataIndex> objectIds;

		ObjectAbstractor::ObjectDataIndexVector creatures;
		ObjectAbstractor::ObjectDataIndexVector threats;
		ObjectAbstractor::ObjectDataIndexVector allies;
		ObjectAbstractor::ObjectDataIndexVector items;
		ObjectAbstractor::ObjectDataIndexVector obstacles;

		template <typename T>
		void whiteDataSet(DataKey dataKey, T dataValue) {
			whiteData.dataSet(dataKey, dataValue);
		}
		template <typename T>
		T whiteDataGet(DataKey dataKey) {
			return whiteData.dataGet<T>(dataKey);

		}
		const DataUMap& whiteDataMapGet();
		ObjectAbstractor::ObjectDataIndex objectAdd(ObjectAbstractor::ObjectData& objectData);
		void objectRemove(ObjectAbstractor::ObjectDataIndex ind);
		bool objectHas(EntityId objectId) const;
		void objectUpdate(ObjectAbstractor::ObjectData& objectData);

		friend void whiteDataObtain(Goap::Blackboard& blackboard);

	protected:
		std::set<ObjectAbstractor::ObjectDataIndex> availableObjectIds;

		// this data cache is used for data that is read only, and/or needs to be used in Action preconditions or effects
		DataCache whiteData;
	};
}

#endif