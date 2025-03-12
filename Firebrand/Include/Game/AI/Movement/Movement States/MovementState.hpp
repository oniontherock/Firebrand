#ifndef __MOVEMENT_STATE_H__
#define __MOVEMENT_STATE_H__


namespace MovementStates {
	enum class MovementState {
		Crouch,
		Walk,
		Run,
		SIZE, // size of the MovementState enum
	};
}

#endif