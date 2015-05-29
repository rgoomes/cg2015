
#include "timer.hpp"

Timer::Timer(){ this->started = false; this->start_time = 0;}

bool Timer::ticking(){ return this->started; }
void Timer::stop(){ this->started = false; stop_time = glfwGetTime(); }
void Timer::start(){ started = true; start_time = start_time + (glfwGetTime() - stop_time); }
double Timer::elapsed(){ return glfwGetTime() - start_time; }
