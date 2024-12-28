#ifndef __SAVE_OPERATIONS_H__
#define __SAVE_OPERATIONS_H__

#include <fstream>
#include <iostream>
#include <vector>
#include <World/Level.hpp>
#include "GameLevel.hpp"

// reinterpret_casts T& into a char*. just used to write quicker, so we don't have to manually type reinterpret_cast every time.
template <typename T>
char* asByte(T& t) {
	return reinterpret_cast<char*>(t);
}
// reinterpret_casts T into a char*. just used to write quicker, so we don't have to manually type reinterpret_cast every time.
template <typename T>
char* asByte(T t) {
	return reinterpret_cast<char*>(t);
}

// T out
template <typename T>
std::ofstream& operator<< (std::ofstream& str, T& item) {
	str.write(asByte(&item), sizeof(item));

	return str;
}
// T in
template <typename T>
std::ifstream& operator>> (std::ifstream& str, T& item) {
	str.read(asByte(&item), sizeof(item));

	return str;
}

// vector<T> out
template <typename T>
std::ofstream& operator<< (std::ofstream& str, std::vector<T>& item) {
	size_t itemSize = item.size();
	str << itemSize;

	str.write(asByte(item.data()), itemSize * sizeof(T));

	return str;
}
// vector<T> in
template <typename T>
std::ifstream& operator>> (std::ifstream& str, std::vector<T>& item) {
	size_t itemSize;
	str >> itemSize;
	item.resize(itemSize);

	str.read(asByte(item.data()), itemSize * sizeof(T));

	return str;
}

// string out
std::ofstream& operator<< (std::ofstream& str, std::string& item);
// string in
std::ifstream& operator>> (std::ifstream& str, std::string& item);

// BaseLevelPtr / GameLevel Out
std::ofstream& operator<< (std::ofstream& str, GameLevel& item);
// BaseLevelPtr / GameLevel in
std::ifstream& operator>> (std::ifstream& str, GameLevel& item);



#endif