#ifndef __DATA_CACHE_H__
#define __DATA_CACHE_H__

#include <any>
#include <string>
#include <unordered_map>

// variant of data types that can be contained in an DataUMap.
typedef std::any DataType;
// key of an BlackboardDataType, used in DataUMap.
typedef std::string DataKey;
// map of names and data.
typedef std::unordered_map<DataKey, DataType> DataUMap;

struct DataCache {
	// removes the element from the dataUMap whose key is dataKey
	void dataRemove(DataKey dataKey);
	// gets the data associated with dataKey, T is the type of the data that is being obtained.
	template <typename T>
	T dataGet(DataKey dataKey) {
		return std::any_cast<T>(dataUMap[dataKey]);
	}
	// sets the value of dataKey to value.
	template <typename T>
	void dataSet(DataKey dataKey, T value) {
		dataUMap[dataKey] = value;
	}
	// returns whether the specified dataKey exists in the dataUMap.
	bool dataHas(DataKey dataKey) const;
	// clears all data from the dataUMap
	void dataClear();

	const DataUMap& dataUMapGet() const;
private:

	// map of all data contained in the cache
	DataUMap dataUMap;
};

#endif
