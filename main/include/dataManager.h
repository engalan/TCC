#include "freertos/queue.h"

#define MAX_QUEUE_SIZE 1
#define QUEUE_SEND_TIMEOUT          50

typedef struct 
{
    int ph;
    int temp;
    int turbidity;
    
} water_params_t;

QueueHandle_t* temperatureSensorQueueHandler();
