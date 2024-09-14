/**
 * @file critical_section.h
 *
 * @brief Macro definitions for entering/exiting critical sections.
 */

#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

/* ------------------------------- Include directives ------------------------------ */
#include "cmsis_gcc.h"

/* -------------------------------- Macro definitions ------------------------------ */
#define CRITICAL_SECTION_ENTER      (__disable_irq())
#define CRITICAL_SECTION_EXIT       (__enable_irq())

#endif /* CRITICAL_SECTION_H */
