#include "Timer.h"

void Timer::update(float deltaTime)
{
	time_ = std::min(time_ + deltaTime, limitTime_);
}

void Timer::reset() { time_ = 0; }

bool Timer::isTimeout() const {
	return time_ >= limitTime_;
}
