#ifndef __WORLD_CLOCK_H__
#define __WORLD_CLOCK_H__

#include <cstdint>

// type representing days
typedef uint8_t DayCount;
// type representing millisecond
typedef uint32_t Millisecond;

// has a bunch of data about the time, like the day, hour of the day, amount of real life time in a day, and some other stuff
class WorldClock {

	// amount of real life minutes in a day
	static constexpr double dayMinuteCount = 35.f;
	// the ratio of the day's length compared to the actual length of a day (24 hours)
	static constexpr double dayTimeRealRatio = 24.0 / dayMinuteCount;
	// amount of real life milliseconds in a day
	static constexpr Millisecond dayLength = Millisecond(60.0 * dayMinuteCount * 1000.0);

	// time since the world began
	Millisecond timeSinceWorldStart;
	// time since the day began
	Millisecond timeSinceDayStart;

	// current day
	DayCount dayCurrent = 0;

	// the time since epoch of the last update
	Millisecond updateTimeLast = 0;

public:
	// maximum number of days
	static constexpr DayCount dayCountMax = 7;

	void update(Millisecond time);

	// gets the actual amount of millisecond since the world started
	Millisecond timeSinceWorldStartGetReal();
	// gets the amount of game hours since the world, an hour is (1.0 / dayTimeRealRatio)
	double hoursSinceWorldStartGet();

	// gets the actual amount of millisecond since the day started
	Millisecond timeSinceDayStartGetReal();
	// gets the amount of game "hours" since the day started, an hour is (1.0 / dayTimeRealRatio)
	double timeSinceDayStartGet();

	// gets the current day
	DayCount dayCurrentGet();
};


#endif