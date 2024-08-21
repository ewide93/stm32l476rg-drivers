/**
 * @file pin.c
 *
 * @brief GPIO configuration.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "pin.h"

/* ------------------------------ Private definitions ------------------------------ */
#define EXTRACT_PORT(PortPin) (((PortPin) & 0xF0U) >> 4U)

/* -------------------------- Private function prototypes -------------------------- */

/**
 * @brief Configure the pin mode of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetMode(const Pin_PinType* Pin);

/**
 * @brief Configure the output type of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetOutputType(const Pin_PinType* Pin);

/**
 * @brief Configure the output speed of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetSpeed(const Pin_PinType* Pin);

/**
 * @brief Configure the pull-up/down circuitry of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetResistor(const Pin_PinType* Pin);

/**
 * @brief Configure the alternate function of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetAltFunc(const Pin_PinType* Pin);

/**
 * @brief Configure the analog settings of the given pin.
 * @param Pin Pointer to pin structure.
 */
static inline void Pin_SetAnalog(const Pin_PinType* Pin);

/* --------------------------- Module private variables ---------------------------- */

/**
 * @brief GPIO pin configuration table. LQFP64 package STM32L476RG.
 */
static const Pin_PinType PinCfgTable[] =
{
    /* Pin 1: VBAT */
    /* Pin 2: PC13 (User button) */
    {
        .Port = GPIOC,
        .Pin = 13,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 3: PC14 (Crystal oscillator input, 32.768kHz) */
    /* Pin 4: PC15 (Crystal oscillator output, 32.768kHz) */
    /* Pin 5: PH0 (External high speed oscillator input, unpopulated) */
    /* Pin 6: PH1 (External high speed oscillator output, unpopulated) */
    /* Pin 7: NRST (MCU reset connected to button) */
    /* Pin 8: PC0 */
    {
        .Port = GPIOC,
        .Pin = 0,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 9: PC1 */
    {
        .Port = GPIOC,
        .Pin = 1,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 10: PC2 */
    {
        .Port = GPIOC,
        .Pin = 2,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 11: PC3 */
    {
        .Port = GPIOC,
        .Pin = 3,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 12: Vssa/Vref- */
    /* Pin 13: Vdda/Vref+ */
    /* Pin 14: PA0 */
    {
        .Port = GPIOA,
        .Pin = 0,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 15: PA1 */
    {
        .Port = GPIOA,
        .Pin = 1,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 16: PA2 (UART Tx to ST-Link) */
    /* Pin 17: PA3 (UART Rx from ST-Link) */
    /* Pin 18: Vss */
    /* Pin 19: Vdd */
    /* Pin 20: PA4 */
    {
        .Port = GPIOA,
        .Pin = 4,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM,
        }
    },
    /* Pin 21: PA5 (Connected to green LED) */
    {
        .Port = GPIOA,
        .Pin = 5,
        .Config =
        {
            .Mode = PIN_MODE_OUTPUT,
            .OutputType = PIN_OUT_TYPE_PUSH_PULL,
            .Speed = PIN_SPEED_LOW,
            .Resistor = PIN_RES_NONE,
            .AltFunc = PIN_AF0,
            .Analog = PIN_ANALOG_NOT_CONNECTED
        }
    },
    /* Pin 22: PA6 */
    {
        .Port = GPIOA,
        .Pin = 6,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 23: PA7 */
    {
        .Port = GPIOA,
        .Pin = 7,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 24: PC4 */
    {
        .Port = GPIOC,
        .Pin = 4,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 25: PC5 */
    {
        .Port = GPIOC,
        .Pin = 5,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 26: PB0 */
    {
        .Port = GPIOB,
        .Pin = 0,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 27: PB1 */
    {
        .Port = GPIOB,
        .Pin = 1,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 28: PB2 */
    {
        .Port = GPIOB,
        .Pin = 2,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 29: PB10 */
    {
        .Port = GPIOB,
        .Pin = 10,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 30: PB11 */
    {
        .Port = GPIOB,
        .Pin = 11,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 31: Vss */
    /* Pin 32: Vdd */
    /* Pin 33: PB12 */
    {
        .Port = GPIOB,
        .Pin = 12,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 34: PB13 */
    {
        .Port = GPIOB,
        .Pin = 13,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 35: PB14 */
    {
        .Port = GPIOB,
        .Pin = 14,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 36: PB15 */
    {
        .Port = GPIOB,
        .Pin = 15,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 37: PC6 */
    {
        .Port = GPIOC,
        .Pin = 6,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 38: PC7 */
    {
        .Port = GPIOC,
        .Pin = 7,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 39: PC8 */
    {
        .Port = GPIOC,
        .Pin = 8,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 40: PC9 */
    {
        .Port = GPIOC,
        .Pin = 9,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 41: PA8 */
    {
        .Port = GPIOA,
        .Pin = 8,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 42: PA9 */
    {
        .Port = GPIOA,
        .Pin = 9,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 43: PA10 */
    {
        .Port = GPIOA,
        .Pin = 10,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 44: PA11 */
    {
        .Port = GPIOA,
        .Pin = 11,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 45: PA12 */
    {
        .Port = GPIOA,
        .Pin = 12,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 46: JTMS-SWDIO */
    /* Pin 47: Vss */
    /* Pin 48: Vdd USB */
    /* Pin 49: JTCK-SWCLK */
    /* Pin 50: PA15 */
    {
        .Port = GPIOA,
        .Pin = 15,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 51: PC10 */
    {
        .Port = GPIOC,
        .Pin = 10,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 52: PC11 */
    {
        .Port = GPIOC,
        .Pin = 11,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 53: PC12 */
    {
        .Port = GPIOC,
        .Pin = 12,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 54: PD2 */
    {
        .Port = GPIOD,
        .Pin = 2,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 55: JTDO-TRACESWO */
    /* Pin 56: PB4 */
    {
        .Port = GPIOB,
        .Pin = 4,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 57: PB5 */
    {
        .Port = GPIOB,
        .Pin = 5,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 58: PB6 */
    {
        .Port = GPIOB,
        .Pin = 6,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 59: PB7 */
    {
        .Port = GPIOB,
        .Pin = 7,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 60: BOOT0 */
    /* Pin 61: PB8 */
    {
        .Port = GPIOB,
        .Pin = 8,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    },
    /* Pin 62: PB9 */
    {
        .Port = GPIOB,
        .Pin = 9,
        .Config =
        {
            .Mode = PIN_MODE_ENUM_LIM,
            .OutputType = PIN_OUT_TYPE_ENUM_LIM,
            .Speed = PIN_SPEED_ENUM_LIM,
            .Resistor = PIN_RES_ENUM_LIM,
            .AltFunc = PIN_AF_ENUM_LIM,
            .Analog = PIN_ANALOG_ENUM_LIM
        }
    }
    /* Pin 63: Vss */
    /* Pin 64: Vdd */
};

static const U8 NofPins = sizeof(PinCfgTable) / sizeof(PinCfgTable[0]);

/* -------------------------- Private function definitions ------------------------- */

static inline void Pin_SetMode(const Pin_PinType* Pin)
{
    if (Pin->Config.Mode >= PIN_MODE_ENUM_LIM) { return; }
    const U8 Shift = (U8)(Pin->Pin << 1);
    Pin->Port->MODER &= ~(0x03UL << Shift);
    Pin->Port->MODER |= ((U32)(Pin->Config.Mode) << Shift);
}

static inline void Pin_SetOutputType(const Pin_PinType* Pin)
{
    if (Pin->Config.OutputType >= PIN_OUT_TYPE_ENUM_LIM) { return; }
    Pin->Port->OTYPER &= ~(0x01UL << Pin->Pin);
    Pin->Port->OTYPER |= ((U32)(Pin->Config.OutputType) << Pin->Pin);
}

static inline void Pin_SetSpeed(const Pin_PinType* Pin)
{
    if (Pin->Config.Speed >= PIN_SPEED_ENUM_LIM) { return; }
    const U8 Shift = (U8)(Pin->Pin < 1);
    Pin->Port->OSPEEDR &= ~(0x03UL << Shift);
    Pin->Port->OSPEEDR |= ((U32)(Pin->Config.Speed) << Shift);
}

static inline void Pin_SetResistor(const Pin_PinType* Pin)
{
    if (Pin->Config.Resistor >= PIN_RES_ENUM_LIM) { return; }
    const U8 Shift = (U8)(Pin->Pin << 1);
    Pin->Port->PUPDR &= ~(0x03UL << Shift);
    Pin->Port->PUPDR |= ((U32)(Pin->Config.Resistor) << Shift);
}

static inline void Pin_SetAltFunc(const Pin_PinType* Pin)
{
    if (Pin->Config.AltFunc >= PIN_AF_ENUM_LIM) { return; }
    const U8 Shift = (Pin->Pin < 8) ? (U8)(Pin->Pin << 2) : (U8)((Pin->Pin - 8) << 2);
    const U8 AFRIndex = (Pin->Pin < 8) ? 0U : 1U;
    Pin->Port->AFR[AFRIndex] &= ~(0x0FUL << Shift);
    Pin->Port->AFR[AFRIndex] |= ((U32)(Pin->Config.AltFunc) << Shift);
}

static inline void Pin_SetAnalog(const Pin_PinType* Pin)
{
    if (Pin->Config.Analog >= PIN_ANALOG_ENUM_LIM) { return; }
    Pin->Port->ASCR &= ~(0x01UL << Pin->Pin);
    Pin->Port->ASCR |= ((U32)(Pin->Config.Analog) << Pin->Pin);
}

GPIO_TypeDef* Pin_GetPort(Pin_PortPinEnum PortPin)
{
    switch (EXTRACT_PORT(PortPin))
    {
        case 0xA: { return GPIOA; }
        case 0xB: { return GPIOB; }
        case 0xC: { return GPIOC; }
        case 0xD: { return GPIOD; }
        default:  { return NULL; }
    }
    return NULL;
}

/* -------------------------- Public function definitions -------------------------- */
void Pin_Init(void)
{
    for (U8 i = 0; i < NofPins; i++)
    {
        Pin_SetMode(&PinCfgTable[i]);
        Pin_SetOutputType(&PinCfgTable[i]);
        Pin_SetSpeed(&PinCfgTable[i]);
        Pin_SetResistor(&PinCfgTable[i]);
        Pin_SetAltFunc(&PinCfgTable[i]);
        Pin_SetAnalog(&PinCfgTable[i]);
    }
}
