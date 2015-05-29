#ifndef TIMER_H
#define TIMER_H

#include <GLFW/glfw3.h>

class Timer {

	public:
		Timer();

	public:
		bool started;
		double start_time, stop_time=0;
		
		double elapsed();
		void start();
		void stop();
		bool ticking();
};

#endif