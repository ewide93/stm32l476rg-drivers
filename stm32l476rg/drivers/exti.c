/**
 * @file exti.c
 *
 * @brief External interrupt module.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "exti.h"

/* -------------------------- Private function declarations ------------------------ */
void DummyCallback(void);
static void SetEdgeTrigger(Exti_EdgeTriggerEnum EdgeTrigger, Exti_GpioExtiEnum Exti);
static S8 ExtiToCrShift(Exti_GpioExtiEnum Exti);
static IRQn_Type ExtiToIrq(Exti_GpioExtiEnum Exti);

/* ------------------------------- Private variables ------------------------------- */
static CallbackFunction ExtiGpioCallbacks[NOF_GPIO_EXTI] =
{
    DummyCallback, DummyCallback, DummyCallback, DummyCallback,
    DummyCallback, DummyCallback, DummyCallback, DummyCallback,
    DummyCallback, DummyCallback, DummyCallback, DummyCallback,
    DummyCallback, DummyCallback, DummyCallback, DummyCallback
};

/* -------------------------- Private function definitions ------------------------- */

void DummyCallback(void) { }

/**
 * @brief Configure the edge trigger settings for the given EXTI.
 * @param EdgeTrigger Setting for edge triggering.
 * @param Exti External interrupt number.
 */
static void SetEdgeTrigger(Exti_EdgeTriggerEnum EdgeTrigger, Exti_GpioExtiEnum Exti)
{
    U32 RisingEdgeRegister = EXTI->RTSR1;
    U32 FallingEdgeRegister = EXTI->FTSR1;
    const U32 ExtiBitmask = (1 << (U8)Exti);

    RisingEdgeRegister &= ~ExtiBitmask;
    FallingEdgeRegister &= ~ExtiBitmask;

    if (EdgeTrigger == EXTI_TRIGGER_RISING_EDGE || EdgeTrigger == EXTI_TRIGGER_ANY_EDGE)
    {
        RisingEdgeRegister |= ExtiBitmask;
    }
    if (EdgeTrigger == EXTI_TRIGGER_FALLING_EDGE || EdgeTrigger == EXTI_TRIGGER_ANY_EDGE)
    {
        FallingEdgeRegister |= ExtiBitmask;
    }

    EXTI->RTSR1 = RisingEdgeRegister;
    EXTI->FTSR1 = FallingEdgeRegister;
}

/**
 * @brief Unmask the interrupt enable bit for the given EXTI.
 */
static inline void EnableExti(Exti_GpioExtiEnum Exti)
{
    EXTI->IMR1 |= (1 << (U8)Exti);
}

/**
 * @brief Maskask the interrupt enable bit for the given EXTI.
 */
static inline void DisableExti(Exti_GpioExtiEnum Exti)
{
    EXTI->IMR1 &= ~(1 << (U8)Exti);
}

/**
 * @brief Extract start-bit position shift that corresponds to
 *        the given EXTI. Used for configuration of SYSCFG_EXTICR.
 */
static S8 ExtiToCrShift(Exti_GpioExtiEnum Exti)
{
    S8 Shift = -1;
    switch (Exti)
    {
        case EXTI0: case EXTI4: case EXTI8:  case EXTI12: { Shift = 0U; break;  }
        case EXTI1: case EXTI5: case EXTI9:  case EXTI13: { Shift = 4U; break;  }
        case EXTI2: case EXTI6: case EXTI10: case EXTI14: { Shift = 8U; break;  }
        case EXTI3: case EXTI7: case EXTI11: case EXTI15: { Shift = 12U; break; }
        default: { break; }
    }
    return Shift;
}

static IRQn_Type ExtiToIrq(Exti_GpioExtiEnum Exti)
{
    IRQn_Type Irq = UsageFault_IRQn;
    switch (Exti)
    {
        case EXTI0: { Irq = EXTI0_IRQn; break; }
        case EXTI1: { Irq = EXTI1_IRQn; break; }
        case EXTI2: { Irq = EXTI2_IRQn; break; }
        case EXTI3: { Irq = EXTI3_IRQn; break; }
        case EXTI4: { Irq = EXTI4_IRQn; break; }
        case EXTI5:
        case EXTI6:
        case EXTI7:
        case EXTI8:
        case EXTI9: { Irq = EXTI9_5_IRQn; break; }
        case EXTI10:
        case EXTI11:
        case EXTI12:
        case EXTI13:
        case EXTI14:
        case EXTI15: { Irq = EXTI15_10_IRQn; break; }
        default: { break; }
    }
    return Irq;
}

/**
 * @brief Extract the external interrupt configuration register
 *        index based on the given port & pin enumeration.
 */
static inline U8 PortPinToExtiCrIndex(Pin_PortPinEnum PortPin)
{
    return (U8)(Pin_GetPin(PortPin) >> 2U);
}


/* -------------------------- Public function definitions -------------------------- */

void Exti_GpioInit(Pin_PortPinEnum PortPin, CallbackFunction Callback, Exti_EdgeTriggerEnum EdgeTrigger)
{
    const Exti_GpioExtiEnum Exti = (Exti_GpioExtiEnum)Pin_GetPin(PortPin);
    const U8 CrIndex = PortPinToExtiCrIndex(PortPin);
    const S8 CrShift = ExtiToCrShift(Exti);
    const IRQn_Type Irq = ExtiToIrq(Exti);

    if (CrShift >= 0 && Irq != UsageFault_IRQn)
    {
        ExtiGpioCallbacks[Exti] = Callback;
        SYSCFG->EXTICR[CrIndex] |= ((((PortPin & 0xF0U) >> 4U) - 0xAU) << CrShift);
        SetEdgeTrigger(EdgeTrigger, Exti);
        EnableExti(Exti);
        NVIC_SetPriority(Irq, 8U);
        NVIC_EnableIRQ(Irq);
    }
}

/* --------------------------- Interrupt service routines -------------------------- */

/**
 * @brief Interrupt handler for EXTI0
 */
void EXTI0_IRQHandler(void)
{
    ExtiGpioCallbacks[EXTI0]();
    EXTI->PR1 |= EXTI_PR1_PIF0;
    __DSB();
}

/**
 * @brief Interrupt handler for EXTI1
 */
void EXTI1_IRQHandler(void)
{
    ExtiGpioCallbacks[EXTI1]();
    EXTI->PR1 |= EXTI_PR1_PIF0;
    __DSB();
}

/**
 * @brief Interrupt handler for EXTI2
 */
void EXTI2_IRQHandler(void)
{
    ExtiGpioCallbacks[EXTI2]();
    EXTI->PR1 |= EXTI_PR1_PIF0;
    __DSB();
}

/**
 * @brief Interrupt handler for EXTI3
 */
void EXTI3_IRQHandler(void)
{
    ExtiGpioCallbacks[EXTI3]();
    EXTI->PR1 |= EXTI_PR1_PIF0;
    __DSB();
}

/**
 * @brief Interrupt handler for EXTI4
 */
void EXTI4_IRQHandler(void)
{
    ExtiGpioCallbacks[EXTI4]();
    EXTI->PR1 |= EXTI_PR1_PIF4;
    __DSB();
}

/**
 * @brief Interrupt handler for EXTI 5, 6, 7, 8 & 9.
 */
void EXTI9_5_IRQHandler(void)
{
    const U32 PendingRegister = EXTI->PR1;

    if (PendingRegister & EXTI_PR1_PIF5)
    {
        ExtiGpioCallbacks[EXTI5]();
        EXTI->PR1 |= EXTI_PR1_PIF5;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF6)
    {
        ExtiGpioCallbacks[EXTI6]();
        EXTI->PR1 |= EXTI_PR1_PIF6;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF7)
    {
        ExtiGpioCallbacks[EXTI7]();
        EXTI->PR1 |= EXTI_PR1_PIF7;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF8)
    {
        ExtiGpioCallbacks[EXTI8]();
        EXTI->PR1 |= EXTI_PR1_PIF9;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF9)
    {
        ExtiGpioCallbacks[EXTI9]();
        EXTI->PR1 |= EXTI_PR1_PIF9;
        __DSB();
    }
}

/**
 * @brief Interrupt handler for EXTI 10, 11, 12, 13, 14 & 15.
 */
void EXTI15_10_IRQHandler(void)
{
    const U32 PendingRegister = EXTI->PR1;

    if (PendingRegister & EXTI_PR1_PIF10)
    {
        ExtiGpioCallbacks[EXTI10]();
        EXTI->PR1 |= EXTI_PR1_PIF10;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF11)
    {
        ExtiGpioCallbacks[EXTI11]();
        EXTI->PR1 |= EXTI_PR1_PIF11;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF12)
    {
        ExtiGpioCallbacks[EXTI12]();
        EXTI->PR1 |= EXTI_PR1_PIF12;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF13)
    {
        ExtiGpioCallbacks[EXTI13]();
        EXTI->PR1 |= EXTI_PR1_PIF13;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF14)
    {
        ExtiGpioCallbacks[EXTI14]();
        EXTI->PR1 |= EXTI_PR1_PIF14;
        __DSB();
    }

    if (PendingRegister & EXTI_PR1_PIF15)
    {
        ExtiGpioCallbacks[EXTI15]();
        EXTI->PR1 |= EXTI_PR1_PIF15;
        __DSB();
    }
}