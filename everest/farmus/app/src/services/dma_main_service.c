/*
main()
|
Dma_Main_service_init() : create task DmaMainService();
|
vTaskDmaMainService() : step1 : dma interrupt init, Dma_Main_INT_init();
                        step2 : dma register init,  Dma_Main_CFG_init();
                        step3 : for(;;), do what you like, e.g. print;
 
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
  //read irq status to clear irq
  inst->irqStatusLast = Xil_In32(0xa0000018);
  xSemaphoreGiveFromISR(inst->irqBinaryMutex,NULL);
  //TODO
  //BaseType_t xHiPrioTaskWoken = 0;
  //xSemaphoreGiveFromISR(inst->irqBinaryMutex,&xHiPrioTaskWoken);
  //portYIELD_FROM_ISR(xHiPrioTaskWoken);
}



static void vTaskDbgService()
{
    while(1)
    {
        if()
    }
}
