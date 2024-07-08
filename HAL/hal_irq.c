/*
 * hal_irq.c
 *
 *  Created on: Jul 8, 2024
 *      Author: i.dymov
 */


/*
 * hal_irq.c
 *
 *  Created on: Jul 6, 2024
 *      Author: i.dymov
 */
#include "hal_irq.h"

#if MCU == CH32V2
void PFIC_IRQ_ENABLE_PG1(IRQn_Type irg, u8 prior, u8 subprior)
{

    if (prior == 1 )
    {
        NVIC_SetPriority(irg, (1 << 7) | (subprior << 4));
    }
    else
    {
         NVIC_SetPriority(irg, (0 << 7) | (subprior << 4));
    }
    NVIC_EnableIRQ(irg);
}
#endif
