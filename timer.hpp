#ifndef TIMER_H
#define TIMER_H

#include <GLFW/glfw3.h>

class Timer {

	public:
		Timer();

	public:
		bool started;
		double start_time;
		
		double elapsed();
		void start();
		void stop();
		bool ticking();
};

#endif