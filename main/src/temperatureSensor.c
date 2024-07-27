#include "ds18b20.h"
#include "esp_log.h"
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"


#define TEMP_BUS 26
#define NUM_OF_SENSORS 1
#define TEMPERATURE_SERVICE_DELAY 3000

static const char *TAG = "TemperatureSensor";
DeviceAddress tempSensors[NUM_OF_SENSORS];

void getTempAddresses(DeviceAddress *tempSensorAddresses) {
	unsigned int numberFound = 0;
	reset_search();
	// search for 2 addresses on the oneWire protocol
	while (search(tempSensorAddresses[numberFound],true)) {
		numberFound++;
		if (numberFound == NUM_OF_SENSORS) break;
	}
    ESP_LOGW(TAG,"Temp Sensor ok");
	// if 2 addresses aren't found then flash the LED rapidly
	while (numberFound != NUM_OF_SENSORS) {
        ESP_LOGW(TAG,"Temp Sensor not found");
		numberFound = 0;
		// search in the loop for the temp sensors as they may hook them up
		reset_search();
		while (search(tempSensorAddresses[numberFound],true)) {
			numberFound++;
			if (numberFound == NUM_OF_SENSORS) break;
		}
	}
	return;
}

void initTempSensor()
{
    ds18b20_init(TEMP_BUS);
    getTempAddresses(tempSensors);
    ds18b20_setResolution(tempSensors,NUM_OF_SENSORS,10);
}

float getTemp()
{
    static float temp = 0;
    ds18b20_requestTemperatures();
    temp = ds18b20_getTempC((DeviceAddress *)tempSensors[0]);
    ESP_LOGD(TAG, "Temperature: %.2f",temp);
    return temp;
}

void temperatureSensorService(void *pvParameters)
{
    while (1)
    {
        getTemp();
        vTaskDelay(pdMS_TO_TICKS(TEMPERATURE_SERVICE_DELAY));
    }
    
}