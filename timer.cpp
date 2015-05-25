
#include "timer.hpp"

Timer::Timer(){ this->started = false; }

bool Timer::ticking(){ return this->started; }
void Timer::stop(){ this->started = false; }
void Timer::start(){ started = true; glfwSetTime(0.0f); }
double Timer::elapsed(){ return glfwGetTime(); }
