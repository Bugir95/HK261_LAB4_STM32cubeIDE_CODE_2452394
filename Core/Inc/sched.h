#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0

#define ERROR_SCH_TOO_MANY_TASKS       1
#define ERROR_SCH_CANNOT_DELETE_TASK   2

#define RETURN_CODE_ERROR_DELETE       0
#define RETURN_CODE_SUCCESS_DELETE     1

typedef struct
{
    void (*funcPtr)(void);
    int delay;       /* ticks remaining before next execution */
    int period;      /* 0 = one-shot; nonzero = periodic */
    int run_me;      /* task is ready to dispatch */
    int taskId;      /* task ID returned by SCH_Add_Task */
} task_struct;

extern task_struct taskArr[SCH_MAX_TASKS];

extern int ERROR_CODE;
extern int SCH_TickCount;

void SCH_Init(void);

int SCH_Add_Task(void (*pFunction)(void), int DELAY, int PERIOD);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

int SCH_Delete_Task(int taskID);

int SCH_Get_Time(void);

void SCH_Sleep(void);

void SCH_Report_Status(void);

#endif
