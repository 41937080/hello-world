/*


*/

#include <semphr.h>


#define DMA_IRQ_STATUS_IDLE    0x0


typedef struct
{
  SemaphoreHandle_t irqBinaryMutex;  //used for communicate between irqhandle and task
  uint32_t          irqStatusLast;   //used for record irq status register
}dma_irq_handler_inst_t;

static dma_irq_handler_inst_t g_inst[1] = 
{
  {0 , DMA_IRQ_STATUS_IDLE}
}

void dma_irqhandler_st(void* userdata)
{
  dma_irq_handler_inst_t* inst = (dma_irq_handler_inst_t*) userdata;
  inst->irqStatusLast = //read irq status register;
  //clear irq
}



static void vTaskDbgService()
{
    while(1)
    {
        if()
    }
}
