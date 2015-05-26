
#include "timer.hpp"

Timer::Timer(){ this->started = false; }

bool Timer::ticking(){ return this->started; }
void Timer::stop(){ this->started = false; }
void Timer::start(){ started = true; start_time = glfwGetTime(); }
double Timer::elapsed(){ return glfwGetTime() - start_time; }
