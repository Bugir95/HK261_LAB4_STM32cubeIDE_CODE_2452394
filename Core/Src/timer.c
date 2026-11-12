#include "main.h"
#include "sched.h"
#include "timer.h"

#define TIMER_CYCLE 10U

static volatile int timer0_counter = 0;
static volatile int timer1_counter = 0;
static volatile int timer0_flag = 1;
static volatile int timer1_flag = 1;

void setTimer0(int duration)
{
    timer0_counter = (duration > 0) ? (duration / (int)TIMER_CYCLE) : 0;
    timer0_flag = (timer0_counter == 0);
}
int getTimer0_flag(void) { return timer0_flag; }

void setTimer1(int duration)
{
    timer1_counter = (duration > 0) ? (duration / (int)TIMER_CYCLE) : 0;
    timer1_flag = (timer1_counter == 0);
}
int getTimer1_flag(void) { return timer1_flag; }

void timer_run(void)
{
    if (timer0_counter > 0 && --timer0_counter == 0) timer0_flag = 1;
    if (timer1_counter > 0 && --timer1_counter == 0) timer1_flag = 1;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        timer_run();
        SCH_Update();
    }
}
