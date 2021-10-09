#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "timer_linux.h"

typedef struct {
	int a;
	int b;
} timer_callback_arg_t;

static timer_callback_arg_t timer_callback_arg1 = {
	.a = 1,
	.b = 2,
};

static timer_callback_arg_t timer_callback_arg2 = {
	.a = 3,
	.b = 4,
};

void timer1Callback(union sigval arg) {
	timer_callback_arg_t *timer_value_tmp = (timer_callback_arg_t *)arg.sival_ptr;
    printf("Timer1 callback. arg: a = %d, b = %d\n", timer_value_tmp->a, timer_value_tmp->b);

	struct timeval timev;

	gettimeofday(&timev, 0);
	printf("Current time = %d.%d\n", (uint32_t)timev.tv_sec, (uint32_t)timev.tv_usec);
	printf("\n");
}

void timer2Callback(union sigval arg) {
	timer_callback_arg_t *timer_value_tmp = (timer_callback_arg_t *)arg.sival_ptr;
	printf("Timer2 callback. arg: a = %d, b = %d\n", timer_value_tmp->a, timer_value_tmp->b);

	struct timeval timev;

	gettimeofday(&timev, 0);
	printf("Current time = %d.%d\n", (uint32_t)timev.tv_sec, (uint32_t)timev.tv_usec);
	printf("\n");
}

int main(void) {
	custom_timer_t timer1 = createTimer(&timer1Callback, TIMER_PERIODIC, &timer_callback_arg1, 1000);
	custom_timer_t timer2 = createTimer(&timer2Callback, TIMER_PERIODIC, &timer_callback_arg2, 555);

	printf("Timer1 start.\n\n");
	startTimer(&timer1);
	printf("Timer2 start.\n\n");
	startTimer(&timer2);

	printf("Timer1 intervals: sec = %u, nsec = %lu\n", (uint32_t)timer1.timer_period.it_interval.tv_sec, (unsigned long)timer1.timer_period.it_interval.tv_nsec);
	printf("Timer2 intervals: sec = %u, nsec = %lu\n\n", (uint32_t)timer2.timer_period.it_interval.tv_sec, (unsigned long)timer2.timer_period.it_interval.tv_nsec);

	usleep(1000 * 1000 * 2);

	printf("Timer1 running %d\n\n", isTimerRunning(&timer1));
	printf("Timer2 running %d\n\n", isTimerRunning(&timer2));

	usleep(1000 * 1000 * 5);

	printf("Timer2 stop.\n\n");
	stopTimer(&timer2);
	printf("Timer2 running %d\n\n", isTimerRunning(&timer2));

	usleep(1000 * 1000 * 5);

	printf("Timer2 start.\n\n");
	startTimer(&timer2);
	printf("Timer2 running %d\n\n", isTimerRunning(&timer2));

	while(1) {
		usleep(50 * 1000);
	}

    return 0;
}
