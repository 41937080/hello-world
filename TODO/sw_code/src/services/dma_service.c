/*
main()
|
Dma_Main_service_init() : create task DmaMainService();
|
vTaskDmaMainService() : step1 : dma interrupt init, Dma_Main_INT_init();
                        step2 : dma register init,  Dma_Main_CFG_init();
                        step3 : for(;;), do what you like, e.g. print;
 
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <platform.h>
#include <xil_printf.h>
#include <xil_io.h>
#include <xscugic.h>

/* */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>


#include <common/app_common.h>


#define DMA_IRQ_STATUS_IDLE    0x0
#define DMA_IRQ_ID 121

extern XScuGic InterruptController;

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
  .width    = 0xa0000070,
  .height   = 0xa0000074,
  .size     = 0xa0000008,
  .ctrl     = 0xa000001c,
  .status   = 0xa0000020,
  .frame_cnt= 0xa0000024,
  .int_status= 0xa0000018,
  .int_enable= 0xa0000030,
  .data_enable= 0xa0000034,
};

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
return_st_t dma_main_init()
{
  dma_main_instance.irqBinarySemaphore = xSemaphoreCreateBinary();
  
  XScuGic_Connect(&InterruptController, DMA_IRQ_ID,
                                            (Xil_ExceptionHandler) dma_irqhandler,
                                            (void *)&dma_main_instance );
  XScuGic_Enable(&InterruptController, DMA_IRQ_ID);
  
  return return_st_success ;
}

return_st_t dma_write_setup(dma_instance_t* inst, void* dstaddr, uint32_t width, uint32_t height, uint32_t size)
{
  Xil_Out32(inst->addr0, dstaddr);
  Xil_Out32(inst->addr1, dstaddr);
  Xil_Out32(inst->size,  size);
  Xil_Out32(inst->size,  0x20001);
  Xil_Out32(0xa0000064,  0x10ffff);
  dma_enable(inst, 1);
  return return_st_success ;
}

return_st_t dma_wait_for_done(dma_instance_t* inst, uint32_t timeout_ms)
{
  return_st_t ret = return_st_success;
  //wait for done
  if(0 == xSemaphoreTake(inst->irqBinarySemaphore, timeout_ms))
  {
    //print error
    ret = return_st_error;
  }
  return ret;
  
}

void vTaskDmaService()
{
  dma_main_init();
  dma_write_setup(&dma_main_instance, 0x20800000, 1000,1000,0xd000000);
  for(;;)
  {
    if(xSemaphoreTake(dma_main_instance.irqBinarySemaphore, 1))
    {
      print("DMA ......");  
    }
    vTaskDelay(1);
  }
}


