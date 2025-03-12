#ifndef __MOVEMENT_STATE_DATA_H__
#define __MOVEMENT_STATE_DATA_H__

#include "MovementState.hpp"
#include <vector>

namespace Movement {

	typedef float MovementStateValue;

	struct MovementStateData {
		MovementStateValue speed;
		MovementStateValue volume;
	};

	class MovementStateHolder {

		static std::vector<MovementStateData> movementStateVector;

		static void movementStatesRegister();

		static MovementStateData movementStateGet(MovementState movementState);
	};
}


#endif