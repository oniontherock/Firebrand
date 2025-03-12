#include "MovementStateData.hpp"

std::vector<Movement::MovementStateData> Movement::MovementStateHolder::movementStateVector{};

void Movement::MovementStateHolder::movementStatesRegister() {

	movementStateVector.resize(uint32_t(MovementState::SIZE));

	movementStateVector[uint32_t(MovementState::Crouch)] = MovementStateData(25.f, 25.f);
	movementStateVector[uint32_t(MovementState::Walk)] = MovementStateData(50.f, 35.f);
	movementStateVector[uint32_t(MovementState::Run)] = MovementStateData(80.f, 70.f);
}
Movement::MovementStateData Movement::MovementStateHolder::movementStateGet(MovementState movementState) {
	return movementStateVector[uint32_t(movementState)];
}
