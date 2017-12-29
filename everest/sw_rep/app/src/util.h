

#ifndef UTIL_H
#define UTIL_H

/* ========== Include ========== */

/* Standard libraries */
#include "stdint.h"
#include "stdbool.h"


/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>

/* Xilinx includes */



/* ========== Task defines ========== */

#define SETUP_NAME          "Setup"
#define SETUP_STACK_SIZE    (4096)
#define SETUP_TASK_PRIORITY ( configMAX_PRIORITIES - 1 )

#define TRACE_NAME          "Trace"
#define TRACE_STACK_SIZE    (2048)
#define TRACE_TASK_PRIORITY ( configMAX_PRIORITIES - 4 )



#endif /* Header guard */


