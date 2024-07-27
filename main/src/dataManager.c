#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "dataManager.h"

QueueHandle_t* temperatureSensorQueueHandler()
{
    static QueueHandle_t temperatureSensorQueue=NULL;
    if (!temperatureSensorQueue)
        // returns queue memory address
        temperatureSensorQueue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(water_params_t) );
    return &temperatureSensorQueue;
}