///
/// @file sched.h
///
/// @brief Header file for superloop priority scheduler module.
///

#ifndef SCHED_H
#define SCHED_H

// -- Include directives --------------------------------------------------------------
#include "cmsis_compiler.h"
#include "typedef.h"

// -- Macro definitions ---------------------------------------------------------------
#define NOF_TASKS_MAX       (32U)
#define TASK_PRIO_HIGHEST   (NOF_TASKS_MAX - 1U)
#define TASK_PRIO_LOWEST    (0U)

// -- Structures & enumerations -------------------------------------------------------

typedef void (*Sched_TaskType)(void);

// -- Public function declarations ----------------------------------------------------

/// @brief Scheduler start function.
/// @note Enters an endless loop and does not return.
__NO_RETURN void Sched_Start(void);

/// @brief Register a task to be managed by the scheduler.
/// @param Task Pointer to task function.
/// @param Priority Task priority level.
/// @return Boolean indicating if the task was registered or not.
Bool Sched_RegisterTask(Sched_TaskType Task, U8 Priority);

/// @brief Register a task to be scheduled to run at the system tick frequency.
/// @param Task Pointer to task function.
/// @param Priority Task priority level.
/// @return Boolean indicating if the task was registered or not.
Bool Sched_RegisterPeriodicTask(Sched_TaskType Task, U8 Priority);

/// @brief Signal that the task with the given priority level is ready to run.
/// @param Priority Task priority level.
void Sched_Run(U8 Priority);

#endif // SCHED_H
