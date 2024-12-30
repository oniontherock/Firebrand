#include "WorldClock.hpp"



void WorldClock::update(Millisecond time) {

	Millisecond timeSinceUpdate;
	if (updateTimeLast == 0) {
		timeSinceUpdate = 0;
	}
	else {
		timeSinceUpdate = time - updateTimeLast;
	}

	timeSinceWorldStart += timeSinceUpdate;
	timeSinceDayStart += timeSinceUpdate;

	if (timeSinceDayStart >= dayLength) {
		dayCurrent += 1;
		timeSinceDayStart = 0;
	}

	updateTimeLast = time;
}


Millisecond WorldClock::timeSinceWorldStartGetReal() {
	return timeSinceWorldStart;
}
double WorldClock::hoursSinceWorldStartGet() {
	return (timeSinceWorldStart * dayTimeRealRatio) / (60 * 1000);
}

Millisecond WorldClock::timeSinceDayStartGetReal() {
	return timeSinceDayStart;
}
double WorldClock::timeSinceDayStartGet() {
	return (timeSinceDayStart * dayTimeRealRatio) / (60 * 1000);
}

DayCount WorldClock::dayCurrentGet() {
	return dayCurrent;
}
