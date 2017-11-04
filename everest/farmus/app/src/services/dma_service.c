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
#include <common/app_common.h>


#define DMA_IRQ_STATUS_IDLE    0x0

/* Type define*/
typedef enum
{
   dma_ret_ok,
   dma_ret_error,
   dma_ret_busy
} dma_ret_t;

typedef struct
{
  //register list
  uint32_t addr0;
  uint32_t addr1;
  uint32_t width;    //0x70
  uint32_t height;   //0x74
  uint32_t size;  
  uint32_t ctrl;
  uint32_t status;   //fifo status
  uint32_t frame_cnt;
  uint32_t int_status;
  uint32_t int_enable;
  uint32_t data_enable;
  //
  SemaphoreHandle_t irqBinarySemaphore;  //used for communicate between irqhandle and task
  uint32_t          irqStatusLast;   //used for record irq status register
} dma_instance_t;

/* variable define */
static dma_instance_t dma_main_instance = 
{
  .addr0    = 0xa0000000,
  .addr1    = 0xa0000004,
  //.width    = 0xa0000070,
  //.height   = 0xa0000074;
  .size     = 0xa0000008,
  .ctrl     = 0xa000001c,
  .status   = 0xa0000020,
  .frame_cnt= 0xa0000024,
  .int_status= 0xa0000018,
  .int_enable= 0xa0000030,
  .data_enable= 0xa0000034
}

/* Local function */
static void dma_enable(dma_instance_t* inst, bool enable)
{
  if(enable)
  {
    Xil_Out32(inst->int_enable, 1); 
    Xil_Out32(inst->data_enable, 1); 
  }
  else
  {
    Xil_Out32(inst->int_enable, 0); 
    Xil_Out32(inst->data_enable, 0); 
  }
}

static void dma_abort(dma_instance_t* inst)
{
  dma_enable(inst, 0);
}

void dma_irqhandler(void* userdata)
{
   dma_instance_t* inst = (dma_instance_t*) userdata;
   inst->irqStatusLast = Xil_In32(inst->int_status);
   xSemaphoreGiveFromISR(inst->irqBinarySemaphore,NULL);
  //TODO
  //BaseType_t xHiPrioTaskWoken = 0;
  //xSemaphoreGiveFromISR(inst->irqBinaryMutex,&xHiPrioTaskWoken);
  //portYIELD_FROM_ISR(xHiPrioTaskWoken);
}

static void dma_print_status(dma_instance_t* inst)
{
  
}


/* Public funciton */
dma_ret_t dma_init()
{
  if()
}







static dma_irq_handler_inst_t dma_main_inst = 
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
