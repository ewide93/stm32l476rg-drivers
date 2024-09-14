/**
 * @file pin.c
 *
 * @brief Pin configuration module.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "pin.h"

/* -------------------------- Private function definitions ------------------------- */

/**
 * @brief Get the GPIO port based on the given port & pin enumeration.
 * @param PortPin Port & pin enumeration.
 * @returns Pointer to GPIO port structure.
 */
static GPIO_TypeDef* Pin_GetPort(Pin_PortPinEnum PortPin)
{

    switch ((PortPin & 0xF0U) >> 4U)
    {
        case 0xA: { return GPIOA; }
        case 0xB: { return GPIOB; }
        case 0xC: { return GPIOC; }
        case 0xD: { return GPIOD; }
        default:  { return NULL; }
    }
    return NULL;
}


/**
 * @brief Get the pin number based on the given port & pin enumeration.
 * @param PortPin Port & pin enumeration.
 * @return Pin number
 */
static inline U8 Pin_GetPin(Pin_PortPinEnum PortPin)
{
    return (U8)(PortPin & 0x0FU);
}

/* -------------------------- Public function definitions -------------------------- */

void Pin_SetMode(Pin_PortPinEnum PortPin, Pin_ModeEnum Mode)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        const U8 Shift = (U8)(Pin << 1);
        Port->MODER &= ~(0x03U << Shift);
        Port->MODER |= (Mode << Shift);
    }
}


void Pin_SetOutputType(Pin_PortPinEnum PortPin, Pin_OutputTypeEnum OutputType)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        Port->OTYPER &= ~(0x01U << Pin);
        Port->OTYPER |= (OutputType << Pin);
    }
}


void Pin_SetSpeed(Pin_PortPinEnum PortPin, Pin_SpeedEnum Speed)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        const U8 Shift = (U8)(Pin < 1);
        Port->OSPEEDR &= ~(0x03UL << Shift);
        Port->OSPEEDR |= (Speed << Shift);
    }
}


void Pin_SetResistor(Pin_PortPinEnum PortPin, Pin_ResistorEnum Resistor)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        const U8 Shift = (U8)(Pin << 1);
        Port->PUPDR &= ~(0x03UL << Shift);
        Port->PUPDR |= (Resistor << Shift);
    }
}


void Pin_SetAltFunc(Pin_PortPinEnum PortPin, Pin_AlternateFunctionEnum AltFunc)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        const U8 Shift = (Pin < 8) ? (U8)(Pin << 2) : (U8)((Pin - 8) << 2);
        const U8 Index = (Pin < 8) ? 0U : 1U;
        Port->AFR[Index] &= ~(0x0FUL << Shift);
        Port->AFR[Index] |= (AltFunc << Shift);
    }
}


void Pin_SetAnalog(Pin_PortPinEnum PortPin, Pin_AnalogEnum Analog)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        Port->ASCR &= ~(0x01UL << Pin);
        Port->ASCR |= (Analog << Pin);
    }
}


void Pin_SetOutputData(Pin_PortPinEnum PortPin)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        Port->BSRR |= (1 << Pin);
    }
}


void Pin_ClearOutputData(Pin_PortPinEnum PortPin)
{
    GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        Port->BSRR |= (1 << (Pin + 16U));
    }
}


Bool Pin_ReadInputData(Pin_PortPinEnum PortPin)
{
    const GPIO_TypeDef* const Port = Pin_GetPort(PortPin);
    if (Port != NULL)
    {
        const U8 Pin = Pin_GetPin(PortPin);
        return Port->IDR & (1 << Pin);
    }
    return False;
}