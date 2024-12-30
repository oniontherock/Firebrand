#include "SaveOperations.hpp"
#include "ECSRegistry.hpp"

std::ofstream& operator<< (std::ofstream& str, std::string& item) {
	size_t itemLen = item.size();
	str << itemLen;

	str.write(item.data(), itemLen * sizeof(char));

	return str;
}
std::ifstream& operator>> (std::ifstream& str, std::string& item) {
	size_t itemLen;
	str >> itemLen;
	item.resize(itemLen);

	str.read(item.data(), itemLen * sizeof(char));

	return str;
}


std::ofstream& operator<< (std::ofstream& str, GameLevel& item) {

	str << item.exampleVariable;


	return str;
}
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str >> item.exampleVariable;

	return str;
}
#pragma region Components
void EntityComponents::ComponentRotateToMouse::save(std::ofstream& str) {
	str << turnSpeed;
}
void EntityComponents::ComponentRotateToMouse::load(std::ifstream& str) {
	str >> turnSpeed;
}
void EntityComponents::ComponentMoveByInput::save(std::ofstream&) {}
void EntityComponents::ComponentMoveByInput::load(std::ifstream&) {}

void EntityComponents::ComponentSprite::save(std::ofstream&) {}
void EntityComponents::ComponentSprite::load(std::ifstream&) {}

void EntityComponents::ComponentPosition::save(std::ofstream&) {}
void EntityComponents::ComponentPosition::load(std::ifstream&) {}

void EntityComponents::ComponentRotation::save(std::ofstream&) {}
void EntityComponents::ComponentRotation::load(std::ifstream&) {}

void EntityComponents::ComponentViewFollow::save(std::ofstream&) {}
void EntityComponents::ComponentViewFollow::load(std::ifstream&) {}
#pragma endregion