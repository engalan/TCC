#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define UPDATE_WATER_PARAMS_DELAY
typedef struct 
{
    int ph;
    int temp;
    int turbidity;
} water_params_t;

QueueHandle_t* waterParamsgetQueueHandler();
void waterMonitorService(void *pvParameters);