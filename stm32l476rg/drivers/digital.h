/**
 * @file digital.h
 *
 * @brief Configuration & control of digital inputs & outputs.
 *
 */

#ifndef DIGITAL_H
#define DIGITAL_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"
#include "pin.h"

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of digital pin states.
 */
typedef enum
{
    DIGITAL_STATE_LOW = 0x0U,
    DIGITAL_STATE_HIGH = 0x1U
} Digital_StateEnum;

/**
 * @brief Digital output structure.
 */
typedef struct
{
    Pin_PortPinEnum PortPin;
    Pin_OutputTypeEnum OutputType;
    Pin_SpeedEnum Speed;
    Digital_StateEnum InitVal;
} Digital_OutputType;

/**
 * @brief Digital input structure.
 */
typedef struct
{
    Pin_PortPinEnum PortPin;
    Pin_ResistorEnum Resistor;
} Digital_InputType;

/* ------------------------- Public function declarations -------------------------- */

/**
 * @brief Initialize the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_OutputInit(const Digital_OutputType* Output);

/**
 * @brief Initialize the given digital input.
 * @param Input Pointer to digital input structure.
 */
void Digital_InputInit(const Digital_InputType* Input);

/**
 * @brief Set the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_Set(const Digital_OutputType* Output);

/**
 * @brief Clear the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_Clear(const Digital_OutputType* Output);

/**
 * @brief Toggle the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_Toggle(const Digital_OutputType* Output);

/**
 * @brief Read the given digital input.
 * @param Input Pointer to digital output structure.
 * @return True = high, False = low
 */
Bool Digital_Read(const Digital_InputType* Input);

#endif /* DIGITAL_H */