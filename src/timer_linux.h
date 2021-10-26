#ifndef TIMER_LINUX_H
#define TIMER_LINUX_H

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
  TIMER_ONCE = 0,
  TIMER_PERIODIC = 1
} timer_type_t;


typedef struct {
	timer_t timer_id;
	struct itimerspec timer_period;
	timer_type_t type;
} custom_timer_t;

typedef void (*timerCallback)(union sigval arg);

custom_timer_t createTimer(timerCallback callback, timer_type_t type, void *callback_arg, uint32_t msec);
int deleteTimer(custom_timer_t *timer);
int startTimer(custom_timer_t *timer);
int stopTimer(custom_timer_t *timer);
void restartTimer(custom_timer_t *timer);
bool isTimerRunning(custom_timer_t *timer);

#endif //TIMER_LINUX_H
