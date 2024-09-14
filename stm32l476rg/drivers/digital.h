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
    LOW = 0x0U,
    HIGH = 0x1U
} Digital_StateEnum;

/**
 * @brief Digital output structure.
 */
typedef struct
{
    Pin_PortPinEnum PortPin;
    Pin_OutputTypeEnum OutputType;
    Pin_SpeedEnum Speed;
    Digital_StateEnum State;
    bool InitValHigh;
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
void Digital_OutputInit(Digital_OutputType* Output);


/**
 * @brief Initialize the given digital input.
 * @param Input Pointer to digital input structure.
 */
void Digital_InputInit(const Digital_InputType* Input);


/**
 * @brief Set the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_Set(Digital_OutputType* Output);


/**
 * @brief Clear the given digital output.
 * @param Output Pointer to digital output structure.
 */
void Digital_Clear(Digital_OutputType* Output);


/**
 * @brief Read the given digital input.
 * @param Input Pointer to digital output structure.
 * @return True = high, false = low
 */
bool Digital_Read(const Digital_InputType* Input);

#endif /* DIGITAL_H */