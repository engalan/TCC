
#include "commManager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "adcSimulator.h"
#include "temperatureSensor.h"
#include "analogSensors.h"

void app_main(void)
{   
    TaskHandle_t waterMonitorTaskHandle            = NULL;
    TaskHandle_t temperatureSensorTaskHandle       = NULL;
    TaskHandle_t analogSensorsTaskHandle       = NULL;

    //initHttpService();
    initTempSensor();
    //xTaskCreate(&waterMonitorService,"water_monitor_service",2048,NULL,1,&waterMonitorTaskHandle);
    xTaskCreate(&temperatureSensorService,"water_temperature_monitor_service",2048,NULL,1,&temperatureSensorTaskHandle);
    xTaskCreate(&analogSensors_service,"water_quality_monitor_service",2048,NULL,1,&analogSensorsTaskHandle);
    
}


