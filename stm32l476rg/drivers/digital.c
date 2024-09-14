/**
 * @file digital.c
 *
 * @brief Configuration & control of digital inputs & outputs
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "digital.h"

/* -------------------------- Public function definitions -------------------------- */

void Digital_OutputInit(Digital_OutputType* Output)
{
    Pin_SetMode(Output->PortPin, PIN_MODE_OUTPUT);
    Pin_SetOutputType(Output->PortPin, Output->OutputType);
    Pin_SetSpeed(Output->PortPin, Output->Speed);
    if (Output->InitValHigh) { Digital_Set(Output); }
    else { Output->State = LOW; }
}


void Digital_InputInit(const Digital_InputType* Input)
{
    Pin_SetMode(Input->PortPin, PIN_MODE_INPUT);
    Pin_SetResistor(Input->PortPin, Input->Resistor);
}


void Digital_Set(Digital_OutputType* Output)
{
    Pin_SetOutputData(Output->PortPin);
    Output->State = HIGH;
}


void Digital_Clear(Digital_OutputType* Output)
{
    Pin_ClearOutputData(Output->PortPin);
    Output->State = LOW;
}


Bool Digital_Read(const Digital_InputType* Input)
{
    return Pin_ReadInputData(Input->PortPin);
}