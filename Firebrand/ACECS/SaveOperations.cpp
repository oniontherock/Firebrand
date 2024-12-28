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

void EntityComponents::ComponentExample::save(std::ofstream& str) {
	str << var;
}
void EntityComponents::ComponentExample::load(std::ifstream& str) {
	str >> var;
}

#pragma endregion