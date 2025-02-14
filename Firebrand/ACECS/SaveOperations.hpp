#ifndef __SAVE_OPERATIONS_H__
#define __SAVE_OPERATIONS_H__

#include "../Include/Game/World/Physics/Collision/CollisionShape.hpp"
#include "GameLevel.hpp"
#include <ECS/Entities/Entity.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <World/Level.hpp>

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

// set<T> out
template <typename T>
std::ofstream& operator<< (std::ofstream& str, std::set<T>& item) {
	std::vector<T> vector(item.begin(), item.end());
	str << vector;
	return str;
}
// set<T> in
template <typename T>
std::ifstream& operator>> (std::ifstream& str, std::set<T>& item) {
	std::vector<T> vector;
	str >> vector;
	item = std::set<T>(vector.begin(), vector.end());
	return str;
}


// sf::Vector2<T> out
template <typename T>
std::ofstream& operator<< (std::ofstream& str, sf::Vector2<T>& item) {
	str << item.x;
	str << item.y;

	return str;
}
// sf::Vector2<T> in
template <typename T>
std::ifstream& operator>> (std::ifstream& str, sf::Vector2<T>& item) {
	str >> item.x;
	str >> item.y;

	return str;
}
// sf::Vector3<T> out
template <typename T>
std::ofstream& operator<< (std::ofstream& str, sf::Vector3<T>& item) {
	str << item.x;
	str << item.y;
	str << item.z;

	return str;
}
// sf::Vector3<T> in
template <typename T>
std::ifstream& operator>> (std::ifstream& str, sf::Vector3<T>& item) {
	str >> item.x;
	str >> item.y;
	str >> item.z;

	return str;
}

// Entity out
std::ofstream& operator<< (std::ofstream& str, Entity& item);
// Entity in
std::ifstream& operator>> (std::ifstream& str, Entity& item);

// string out
std::ofstream& operator<< (std::ofstream& str, std::string& item);
// string in
std::ifstream& operator>> (std::ifstream& str, std::string& item);

// BaseLevelPtr / GameLevel Out
std::ofstream& operator<< (std::ofstream& str, GameLevel& item);
// BaseLevelPtr / GameLevel in
std::ifstream& operator>> (std::ifstream& str, GameLevel& item);

// AStarCell out
std::ofstream& operator<< (std::ofstream& str, AStarCell& item);
// AStarCell in
std::ifstream& operator>> (std::ifstream& str, AStarCell& item);

// AStarGrid out
std::ofstream& operator<< (std::ofstream& str, AStarGrid& item);
// AStarGrid in
std::ifstream& operator>> (std::ifstream& str, AStarGrid& item);

// ObjectCell out
std::ofstream& operator<< (std::ofstream& str, ObjectCell& item);
// ObjectCell in
std::ifstream& operator>> (std::ifstream& str, ObjectCell& item);

// ObjectGrid out
std::ofstream& operator<< (std::ofstream& str, ObjectGrid& item);
// ObjectGrid in
std::ifstream& operator>> (std::ifstream& str, ObjectGrid& item);

// CollisionShapeBase out
std::ofstream& operator<< (std::ofstream& str, CollisionShapeBase& item);
// CollisionShapeBase in
std::ifstream& operator>> (std::ifstream& str, CollisionShapeBase& item);
// CollisionShapePolygon out
std::ofstream& operator<< (std::ofstream& str, CollisionShapePolygon& item);
// CollisionShapePolygon in
std::ifstream& operator>> (std::ifstream& str, CollisionShapePolygon& item);

// PathGenerator out
std::ofstream& operator<< (std::ofstream& str, PathGenerator& item);
// PathGenerator in
std::ifstream& operator>> (std::ifstream& str, PathGenerator& item);

#endif