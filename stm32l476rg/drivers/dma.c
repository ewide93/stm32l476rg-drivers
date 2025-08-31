/**
 * @file dma.c
 *
 * @brief Implementation of driver for DMA peripheral.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "dma.h"
#include "clock_control.h"

/* ------------------------ Private preprocessor directives ------------------------ */

#define NOF_DMA_INSTANCES       (2U)
#define NOF_CHANNELS_PER_DMA    (7U)

/* --------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Definition of DMA handle type, kept hidden from users.
 */
typedef struct Dma_OpaqueHandleType
{
    DMA_TypeDef* InstanceRegs;         /* Instance control & status registers. */
    DMA_Channel_TypeDef* ChannelRegs;  /* Channel control & status registers. */
    Dma_InstanceEnum Instance;         /* Peripheral instance number. */
    Dma_ChannelEnum Channel;           /* DMA channel number. */
    Bool InUse;                        /* Usage status. */
} Dma_OpaqueHandleType;


/* ------------------------------- Private variables ------------------------------- */

static Dma_OpaqueHandleType DmaHandles[NOF_DMA_INSTANCES][NOF_CHANNELS_PER_DMA] = { 0 };
static Bool ModuleInitialized = False;

/* ------------------------- Private function declarations ------------------------- */

/**
 * @brief Map the given DMA instance to the corresponding
 *        DMA instance register set.
 * @param Instance DMA instance number.
 * @return DMA instance status & control register set.
 */
static inline DMA_TypeDef* Dma_GetInstanceRegisters(Dma_InstanceEnum Instance);

/**
 * @brief Map the given DMA instance & channel pair to the corresponding
 *        DMA channel register set.
 * @param Instance DMA instance number.
 * @param Channel DMA channel number.
 * @return DMA channel status & control register set.
 */
static DMA_Channel_TypeDef* Dma_GetChannelRegisters(Dma_InstanceEnum Instance, Dma_ChannelEnum Channel);

/**
 * @brief Set the arbitration priority level for the given DMA channel.
 * @param Handle DMA peripheral handle.
 * @param Priority Arbitration priority level.
 */
static inline void Dma_SetChannelPriority(Dma_HandleType Handle, Dma_ChannelPriorityEnum Priority);

/**
 * @brief Set the DMA memory transfer size.
 * @param Handle DMA peripheral handle.
 * @param Size Transfer size.
 */
static inline void Dma_SetMemoryTransferSize(Dma_HandleType Handle, Dma_TransferSizeEnum Size);

/**
 * @brief Set the DMA peripheral transfer size.
 * @param Handle DMA peripheral handle.
 * @param Size Transfer size.
 */
static inline void Dma_SetPeripheralTransferSize(Dma_HandleType Handle, Dma_TransferSizeEnum Size);

/**
 * @brief Set DMA transfer direction.
 * @param Handle DMA peripheral handle.
 * @param Direction DMA transfer direction.
 */
static inline void Dma_SetTransferDirection(Dma_HandleType Handle, Dma_TransferDirectionEnum Direction);

/**
 * @brief Enable memory-to-memory transfer mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_MemToMemModeEnable(Dma_HandleType Handle);

/**
 * @brief Disable memory-to-memory transfer mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_MemToMemModeDisable(Dma_HandleType Handle);

/**
 * @brief Enable memory increment mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_MemoryIncrementModeEnable(Dma_HandleType Handle);

/**
 * @brief Disable memory increment mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_MemoryIncrementModeDisable(Dma_HandleType Handle);

/**
 * @brief Enable peripheral increment mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_PeripheralIncrementModeEnable(Dma_HandleType Handle);

/**
 * @brief Disable peripheral increment mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_PeripheralIncrementModeDisable(Dma_HandleType Handle);

/**
 * @brief Enable circular mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_CircularModeEnable(Dma_HandleType Handle);

/**
 * @brief Disable circular mode.
 * @param Handle DMA peripheral handle.
 */
static inline void Dma_CircularModeDisable(Dma_HandleType Handle);

/**
 * @brief Configure the peripheral & memory addresses.
 * @param Handle DMA peripheral handle.
 * @param PeripheralAddr Peripheral address.
 * @param MemoryAddr Memory address.
 */
static inline void Dma_SetAddresses(Dma_HandleType Handle, void* PeripheralAddr, void* MemoryAddr);


/* -------------------------- Private function definitions ------------------------- */

static inline DMA_TypeDef* Dma_GetInstanceRegisters(Dma_InstanceEnum Instance)
{
    if (Instance == DMA_INSTANCE_1) { return DMA1; }
    else { return DMA2; }
}

static DMA_Channel_TypeDef* Dma_GetChannelRegisters(Dma_InstanceEnum Instance, Dma_ChannelEnum Channel)
{
    DMA_Channel_TypeDef* RetVal = (DMA_Channel_TypeDef*)NULL;

    switch (Channel)
    {
        case DMA_CHANNEL_1:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel1 : DMA2_Channel1;
            break;
        }
        case DMA_CHANNEL_2:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel2 : DMA2_Channel2;
            break;
        }
        case DMA_CHANNEL_3:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel3 : DMA2_Channel3;
            break;
        }
        case DMA_CHANNEL_4:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel4 : DMA2_Channel4;
            break;
        }
        case DMA_CHANNEL_5:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel5 : DMA2_Channel5;
            break;
        }
        case DMA_CHANNEL_6:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel6 : DMA2_Channel6;
            break;
        }
        case DMA_CHANNEL_7:
        {
            RetVal = (Instance == DMA_INSTANCE_1) ? DMA1_Channel7 : DMA2_Channel7;
            break;
        }
        default:
        {
            /* This should not happen... */
            break;
        }
    }

    return RetVal;
}

static inline void Dma_SetChannelPriority(Dma_HandleType Handle, Dma_ChannelPriorityEnum Priority)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_PL;
    Handle->ChannelRegs->CCR |= ((U32)Priority << DMA_CCR_PL_Pos);
}

static inline void Dma_SetMemoryTransferSize(Dma_HandleType Handle, Dma_TransferSizeEnum Size)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_MSIZE;
    Handle->ChannelRegs->CCR |= ((U32)Size << DMA_CCR_MSIZE_Pos);
}

static inline void Dma_SetPeripheralTransferSize(Dma_HandleType Handle, Dma_TransferSizeEnum Size)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_PSIZE;
    Handle->ChannelRegs->CCR |= ((U32)Size << DMA_CCR_PSIZE_Pos);
}

static inline void Dma_SetTransferDirection(Dma_HandleType Handle, Dma_TransferDirectionEnum Direction)
{
    if (Direction == DMA_TRANSFER_DIR_READ_FROM_PERIPHERAL) { Handle->ChannelRegs->CCR &= ~DMA_CCR_DIR; }
    else { Handle->ChannelRegs->CCR |= DMA_CCR_DIR; }
}

static inline void Dma_MemToMemModeEnable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR |= DMA_CCR_MEM2MEM;
}

static inline void Dma_MemToMemModeDisable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_MEM2MEM;
}

static inline void Dma_MemoryIncrementModeEnable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR |= DMA_CCR_MINC;
}

static inline void Dma_MemoryIncrementModeDisable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_MINC;
}

static inline void Dma_PeripheralIncrementModeEnable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR |= DMA_CCR_PINC;
}

static inline void Dma_PeripheralIncrementModeDisable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_PINC;
}

static inline void Dma_CircularModeEnable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR |= DMA_CCR_CIRC;
}

static inline void Dma_CircularModeDisable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_CIRC;
}

static inline void Dma_SetAddresses(Dma_HandleType Handle, void* PeripheralAddr, void* MemoryAddr)
{
    Handle->ChannelRegs->CPAR = (U32)PeripheralAddr;
    Handle->ChannelRegs->CMAR = (U32)MemoryAddr;
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
                DmaHandles[Instance][Channel].InstanceRegs = Dma_GetInstanceRegisters(Instance);
                DmaHandles[Instance][Channel].ChannelRegs = Dma_GetChannelRegisters(Instance, Channel);
                DmaHandles[Instance][Channel].Instance = (Dma_InstanceEnum)Instance;
                DmaHandles[Instance][Channel].Channel = (Dma_ChannelEnum)Channel;
                DmaHandles[Instance][Channel].InUse = False;
            }
        }
        ClkCtrl_PeripheralClockEnable(PCLK_DMA1);
        ClkCtrl_PeripheralClockEnable(PCLK_DMA2);
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

U16 Dma_GetTransferCnt(Dma_HandleType Handle)
{
    return (U16)(Handle->ChannelRegs->CNDTR & 0x0000FFFFUL);
}

void Dma_ChannelEnable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR |= DMA_CCR_EN;
}

void Dma_ChannelDisable(Dma_HandleType Handle)
{
    Handle->ChannelRegs->CCR &= ~DMA_CCR_EN;
}
