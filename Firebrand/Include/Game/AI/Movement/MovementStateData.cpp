#include "MovementStateData.hpp"

std::vector<MovementStates::MovementStateData> MovementStates::MovementStateHolder::movementStateVector{};

void MovementStates::MovementStateHolder::movementStatesRegister() {

	movementStateVector.resize(uint32_t(MovementState::SIZE));

	movementStateVector[uint32_t(MovementState::Crouch)] = MovementStateData(25.f, 25.f);
	movementStateVector[uint32_t(MovementState::Walk)] = MovementStateData(50.f, 35.f);
	movementStateVector[uint32_t(MovementState::Run)] = MovementStateData(80.f, 70.f);
}
MovementStates::MovementStateData MovementStates::MovementStateHolder::movementStateGet(MovementState movementState) {
	return movementStateVector[uint32_t(movementState)];
}
