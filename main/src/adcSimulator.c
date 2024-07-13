
#include "esp_log.h"

#include "adcSimulator.h"

#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 1
#define QUEUE_SEND_TIMEOUT          50

#define WATER_MONITOR_TASK_DELAY 5000
#define TAG "WATER PARAMS"



// typedef struct 
// {
//     int ph;
//     int temp;
//     int turbidity;
    
// }water_params_t;


QueueHandle_t* waterParamsgetQueueHandler()
{
    static QueueHandle_t waterParamsQueue=NULL;
    if (!waterParamsQueue)
        // returns queue memory address
        waterParamsQueue = xQueueCreate( MAX_QUEUE_SIZE, sizeof(water_params_t) );
    return &waterParamsQueue;
}

int randomValueGenerator()
{
    int numero_aleatorio = rand() % 10; // Gera um número entre 0 e 9
    return numero_aleatorio;
}

void getWaterParamsValue(water_params_t * valuesToUpdate)
{
    valuesToUpdate->ph = randomValueGenerator();
    valuesToUpdate->temp = randomValueGenerator() ;
    valuesToUpdate->turbidity = randomValueGenerator();

    ESP_LOGI(TAG,"PH: %d\tTemp: %d\tTurb: %d\t", valuesToUpdate->ph, valuesToUpdate->temp, valuesToUpdate->turbidity);
}

void waterMonitorService(void *pvParameters) //void *pvParameters pois uma função que é uma task deve receber esse parametro
{
    water_params_t * waterParamsToSend;
    while (1){
        getWaterParamsValue(&waterParamsToSend);
        xQueueSend(*waterParamsgetQueueHandler(), (const void *)&waterParamsToSend, pdMS_TO_TICKS(QUEUE_SEND_TIMEOUT));
        vTaskDelay(pdMS_TO_TICKS(WATER_MONITOR_TASK_DELAY));
    }  
}