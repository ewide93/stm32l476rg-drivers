///
/// @file sched.c
///
/// @brief Source file for superloop priority scheduler module.
///

// -- Include directives --------------------------------------------------------------
#include "scheduler.h"
#include "critical_section.h"
#include "range.h"
#include "systick.h"

// -- Private variables --------------------------------------------------------------
static Sched_TaskType TaskList[NOF_TASKS_MAX] = { 0 };
static volatile U32 RunBits = 0U;
static U32 PeriodicTaskMask = 0U;

// -- Private function definitions ----------------------------------------------------

void Sched_ScheduleAllPeriodicTasks(void)
{
    CRITICAL_SECTION_ENTER;
    RunBits |= PeriodicTaskMask;
    CRITICAL_SECTION_EXIT;
}

// -- Public function definitions -----------------------------------------------------

__NO_RETURN void Sched_Start(void)
{
    static Sched_TaskType Task = NULL;

    SysTick_RegisterCallback(Sched_ScheduleAllPeriodicTasks);
    while (True)
    {
        if (RunBits != 0U)
        {
            CRITICAL_SECTION_ENTER;
            U8 HighestActivePriority = TASK_PRIO_HIGHEST - __CLZ(RunBits);
            if (HighestActivePriority <= TASK_PRIO_HIGHEST)
            {
                Task = TaskList[HighestActivePriority];
                RunBits &= ~(1UL << HighestActivePriority);
            }
            CRITICAL_SECTION_EXIT;

            if (Task != NULL) { Task(); }
        }
    }
}

Bool Sched_RegisterTask(Sched_TaskType Task, U8 Priority)
{
    Bool RetVal = False;
    if (Range_U8(Priority, TASK_PRIO_LOWEST, TASK_PRIO_HIGHEST))
    {
        TaskList[Priority] = Task;
        RetVal = True;
    }

    return RetVal;
}

Bool Sched_RegisterPeriodicTask(Sched_TaskType Task, U8 Priority)
{
    Bool RetVal = Sched_RegisterTask(Task, Priority);
    if (RetVal) { PeriodicTaskMask |= (1U << Priority); }
    return RetVal;
}

void Sched_Run(U8 Priority)
{
    if (Range_U8(Priority, TASK_PRIO_LOWEST, TASK_PRIO_HIGHEST))
    {
        CRITICAL_SECTION_ENTER;
        RunBits |= (1U << Priority);
        CRITICAL_SECTION_EXIT;
    }
}
