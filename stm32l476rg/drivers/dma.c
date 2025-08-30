/**
 * @file dma.c
 *
 * @brief Implementation of driver for DMA peripheral.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "dma.h"

/* ------------------------ Private preprocessor directives ------------------------ */

#define NOF_DMA_INSTANCES       (2U)
#define NOF_CHANNELS_PER_DMA    (7U)

/* --------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Definition of DMA handle type, kept hidden from users.
 */
typedef struct Dma_OpaqueHandleType
{
    DMA_TypeDef* Registers;         /* Peripheral control & status registers. */
    Dma_InstanceEnum Instance;      /* Peripheral instance number. */
    Dma_ChannelEnum Channel;        /* DMA channel number. */
    Bool InUse;
} Dma_OpaqueHandleType;


/* ------------------------------- Private variables ------------------------------- */

static Dma_OpaqueHandleType DmaHandles[NOF_DMA_INSTANCES][NOF_CHANNELS_PER_DMA] = { 0 };
static Bool ModuleInitialized = False;

/* ------------------------- Private function declarations ------------------------- */

/**
 * @brief Map the given DMA instance & channel number pair to
 *        the corresponding peripheral register set.
 */
static inline DMA_TypeDef* Dma_GetRegisterPtr(Dma_InstanceEnum Instance);

/* -------------------------- Private function definitions ------------------------- */

static inline DMA_TypeDef* Dma_GetRegisterPtr(Dma_InstanceEnum Instance)
{
    if (Instance == DMA_INSTANCE_1) { return DMA1; }
    else { return DMA2; }
}

/* -------------------------- Public function definitions -------------------------- */

void Dma_Init(void)
{
    if (!ModuleInitialized)
    {
        for (U8 Instance = 0; Instance < NOF_DMA_INSTANCES; Instance++)
        {
            for (U8 Channel = 0; Channel < NOF_CHANNELS_PER_DMA; Channel++)
            {
                DmaHandles[Instance][Channel].Registers = Dma_GetRegisterPtr(Instance);
                DmaHandles[Instance][Channel].Instance = (Dma_InstanceEnum)Instance;
                DmaHandles[Instance][Channel].Channel = (Dma_ChannelEnum)Channel;
                DmaHandles[Instance][Channel].InUse = False;
            }
        }
        ModuleInitialized = True;
    }
}

Dma_HandleType Dma_GetHandle(Dma_InstanceEnum Instance, Dma_ChannelEnum Channel)
{
    if ( (Instance >= DMA_INSTANCE_ENUM_LIMIT) || (Channel >= DMA_CHANNEL_ENUM_LIMIT) || (!ModuleInitialized) )
    {
        return (Dma_HandleType)NULL;
    }
    else { return &DmaHandles[Instance][Channel]; }
}

Bool Dma_ChannelIsAvailable(Dma_HandleType Handle)
{
    return !Handle->InUse;
}
