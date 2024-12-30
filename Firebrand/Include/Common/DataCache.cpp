#include "DataCache.hpp"




void DataCache::dataRemove(DataKey dataKey) {
	dataUMap.erase(dataKey);
}

bool DataCache::dataHas(DataKey dataKey) const {
	return dataUMap.count(dataKey) != 0;
}

void DataCache::dataClear() {
	dataUMap.clear();
}

