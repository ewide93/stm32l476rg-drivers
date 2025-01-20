/**
 * @brief Provide prototypes for SysTick, PendSV and SVC handlers from FreeRTOS.
 */

#ifndef PORT_H
#define PORT_H

/*
 * Exception handlers.
 */
void xPortPendSVHandler( void ) __attribute__( ( naked ) );
void xPortSysTickHandler( void );
void vPortSVCHandler( void ) __attribute__( ( naked ) );

#endif /* PORT_H */