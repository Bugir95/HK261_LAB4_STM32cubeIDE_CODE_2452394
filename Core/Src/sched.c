#include "sched.h"
#include "main.h"

task_struct taskArr[SCH_MAX_TASKS];

int ERROR_CODE = 0;
int SCH_TickCount = 0;

void SCH_Init(void)
{
    int i;

    __disable_irq();
    SCH_TickCount = 0;
    ERROR_CODE = 0;

    for (i = 0; i < SCH_MAX_TASKS; ++i)
    {
        taskArr[i].funcPtr = 0;
        taskArr[i].delay = 0;
        taskArr[i].period = 0;
        taskArr[i].run_me = 0;
        taskArr[i].taskId = NO_TASK_ID;
    }

    __enable_irq();
}

int SCH_Add_Task(void (*pFunction)(void), int DELAY, int PERIOD)
{
    int i;
    if (pFunction == 0)
    {
        ERROR_CODE = ERROR_SCH_TOO_MANY_TASKS;
        return NO_TASK_ID;
    }

    __disable_irq();
    for (i = 0; i < SCH_MAX_TASKS; ++i)
    {
        if (taskArr[i].funcPtr == 0)
        {
            taskArr[i].funcPtr = pFunction;
            taskArr[i].delay = DELAY;
            taskArr[i].period = PERIOD;
            taskArr[i].run_me = 0;
            taskArr[i].taskId = i + 1;

            __enable_irq();
            return taskArr[i].taskId;
        }
    }
    __enable_irq();
    ERROR_CODE = ERROR_SCH_TOO_MANY_TASKS;
    return NO_TASK_ID;
}

void SCH_Update(void)
{
    int i;
    ++SCH_TickCount;
    for (i = 0; i < SCH_MAX_TASKS; ++i)
    {
        if (taskArr[i].funcPtr != 0)
        {
            if (taskArr[i].delay > 0)
            {
                --taskArr[i].delay;
            }
            if (taskArr[i].delay == 0)
            {
                if (taskArr[i].run_me < 255)
                {
                    ++taskArr[i].run_me;
                }
                if (taskArr[i].period > 0)
                {
                    taskArr[i].delay = taskArr[i].period;
                }
            }
        }
    }
}

void SCH_Dispatch_Tasks(void)
{
    int i;
    for (i = 0; i < SCH_MAX_TASKS; ++i)
    {
        if ((taskArr[i].funcPtr != 0) &&
            (taskArr[i].run_me > 0))
        {
            void (*task)(void) = taskArr[i].funcPtr;
            --taskArr[i].run_me;
            task();
            if (taskArr[i].period == 0)
            {
                (void)SCH_Delete_Task(taskArr[i].taskId);
            }
        }
    }
}

int SCH_Delete_Task(int taskID)
{
    int index;

    if ((taskID == NO_TASK_ID) ||
        (taskID > SCH_MAX_TASKS))
    {
        ERROR_CODE = ERROR_SCH_CANNOT_DELETE_TASK;
        return RETURN_CODE_ERROR_DELETE;
    }

    index = taskID - 1;

    __disable_irq();

    if ((taskArr[index].funcPtr == 0) ||
        (taskArr[index].taskId != taskID))
    {
        __enable_irq();

        ERROR_CODE = ERROR_SCH_CANNOT_DELETE_TASK;
        return RETURN_CODE_ERROR_DELETE;
    }

    taskArr[index].funcPtr = 0;
    taskArr[index].delay = 0;
    taskArr[index].period = 0;
    taskArr[index].run_me = 0;
    taskArr[index].taskId = NO_TASK_ID;

    __enable_irq();
    return RETURN_CODE_SUCCESS_DELETE;
}

int SCH_Get_Time(void)
{
    int ticks;
    __disable_irq();
    ticks = SCH_TickCount;
    __enable_irq();

    return ticks * 10;
}
