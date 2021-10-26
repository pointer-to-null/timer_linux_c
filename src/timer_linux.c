#include "timer_linux.h"
#include <stdio.h>
#include <errno.h>

custom_timer_t createTimer(timerCallback callback, timer_type_t type, void *callback_arg, uint32_t msec) {
	custom_timer_t timer;
	struct sigevent timer_signal_event;
	timer_signal_event.sigev_notify = SIGEV_THREAD;
	timer_signal_event.sigev_notify_function = callback;
	timer_signal_event.sigev_value.sival_ptr = callback_arg;
	timer_signal_event.sigev_notify_attributes = NULL;
	timer_create(CLOCK_MONOTONIC, &timer_signal_event, &(timer.timer_id));

	timer.type = type;
	switch(type) {
		case TIMER_ONCE: {
			timer.timer_period.it_value.tv_sec = msec / 1000;
			timer.timer_period.it_value.tv_nsec = (msec % 1000) * 1000 * 1000;
			timer.timer_period.it_interval.tv_sec = 0;
			timer.timer_period.it_interval.tv_nsec = 0;
		} break;
		case TIMER_PERIODIC: {
			timer.timer_period.it_value.tv_sec = msec / 1000;
			timer.timer_period.it_value.tv_nsec = (msec % 1000) * 1000 * 1000;
			timer.timer_period.it_interval.tv_sec = msec / 1000;
			timer.timer_period.it_interval.tv_nsec = (msec % 1000) * 1000 * 1000;
		} break;
		default: {
			timer.timer_period.it_value.tv_sec = 0;
			timer.timer_period.it_value.tv_nsec = 0;
			timer.timer_period.it_interval.tv_sec = 0;
			timer.timer_period.it_interval.tv_nsec = 0;
		} break;
	}
	return timer;
}

int deleteTimer(custom_timer_t *timer) {
	return timer_delete(timer->timer_id);
}

int startTimer(custom_timer_t *timer) {
	return timer_settime(timer->timer_id, 0, &(timer->timer_period), NULL);
}

int stopTimer(custom_timer_t *timer) {
	struct itimerspec timer_value;
	timer_value.it_value.tv_sec = 0;
	timer_value.it_value.tv_nsec = 0;
	timer_value.it_interval.tv_sec = 0;
	timer_value.it_interval.tv_nsec = 0;
	return timer_settime(timer->timer_id, 0, &timer_value, NULL);
}

void restartTimer(custom_timer_t *timer) {
	int status = 0;
	int errno_code = 0;
	if(isTimerRunning(timer)) {
		status = stopTimer(timer);
		if(status != 0) {
			errno_code = errno;
			printf("Error during timer restart! (stopTimer()) errno code = %d\n", errno_code);
		}
	}
	status = startTimer(timer);
	if(status != 0) {
		errno_code = errno;
		printf("Error during timer restart! (startTimer()) errno code = %d\n", errno_code);
	}
}

bool isTimerRunning(custom_timer_t *timer) {
	struct itimerspec timer_value;
	timer_gettime(timer->timer_id, &timer_value);
	return (timer_value.it_value.tv_sec > 0 || timer_value.it_value.tv_nsec > 0);
}
