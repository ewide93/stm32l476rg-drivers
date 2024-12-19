/**
 * @file digital.c
 *
 * @brief Configuration & control of digital inputs & outputs
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "digital.h"

/* -------------------------- Public function definitions -------------------------- */

void Digital_OutputInit(const Digital_OutputType* Output)
{
    Pin_SetMode(Output->PortPin, PIN_MODE_OUTPUT);
    Pin_SetOutputType(Output->PortPin, Output->OutputType);
    Pin_SetSpeed(Output->PortPin, Output->Speed);
    if (Output->InitVal == HIGH) { Digital_Set(Output); }
    else { Digital_Clear(Output); }
}

void Digital_InputInit(const Digital_InputType* Input)
{
    Pin_SetMode(Input->PortPin, PIN_MODE_INPUT);
    Pin_SetResistor(Input->PortPin, Input->Resistor);
}

void Digital_Set(const Digital_OutputType* Output)
{
    Pin_SetOutputData(Output->PortPin);
}

void Digital_Clear(const Digital_OutputType* Output)
{
    Pin_ClearOutputData(Output->PortPin);
}

void Digital_Toggle(const Digital_OutputType* Output)
{
    Pin_ToggleOutputData(Output->PortPin);
}

Bool Digital_Read(const Digital_InputType* Input)
{
    return Pin_ReadInputData(Input->PortPin);
}
