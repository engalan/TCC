#include <stdint.h>

#define ANALOG_SENSOR_TASK_DELAY 3000

void analogSensors_initADS1115();
float convertRawToVoltage();
void analogSensors_service(void *pvParameters);

typedef struct 
{   
    int16_t pHSensorResult;
    int16_t turbiditySensorResult;
    
}analogSensors_t;