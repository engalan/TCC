#include <stdio.h>

#include <esp_log.h>
#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ADS1115.h"
#include "analogSensors.h"

#define SDA_IO (21)                      /*!< gpio number for I2C master data  */
#define SCL_IO (22)                      /*!< gpio number for I2C master clock */

#define FREQ_HZ (100000)                 /*!< I2C master clock frequency */
#define TX_BUF_DISABLE (0)               /*!< I2C master doesn't need buffer */
#define RX_BUF_DISABLE (0)               /*!< I2C master doesn't need buffer */

#define I2C_NUM I2C_NUM_0                /*!< I2C number */
#define I2C_MODE I2C_MODE_MASTER         /*!< I2C mode to act as */
#define I2C_RX_BUF_STATE RX_BUF_DISABLE  /*!< I2C set rx buffer status */
#define I2C_TX_BUF_STATE TX_BUF_DISABLE  /*!< I2C set tx buffer status */
#define I2C_INTR_ALOC_FLAG (0)           /*!< I2C set interrupt allocation flag */
#define ANALOG_SENSOR_TASK_DELAY 3000

static const char *TAG = "Analog Sensors";

/* i2c setup ----------------------------------------- */
// Config profile for espressif I2C lib
i2c_config_t i2c_cfg = {                     
  .mode = I2C_MODE_MASTER, 
  .sda_io_num = SDA_IO,
  .scl_io_num = SCL_IO,
  .sda_pullup_en = GPIO_PULLUP_DISABLE,
  .scl_pullup_en = GPIO_PULLUP_DISABLE,
  .master.clk_speed = FREQ_HZ,
};

/* ADS1115 setup ------------------------------------- */
// Below uses the default values speficied by the datasheet
ads1115_t ads1115_cfg = {
  .reg_cfg =  ADS1115_CFG_LS_COMP_MODE_TRAD | // Comparator is traditional
              ADS1115_CFG_LS_COMP_LAT_NON |   // Comparator is non-latching
              ADS1115_CFG_LS_COMP_POL_LOW |   // Alert is active low
              ADS1115_CFG_LS_COMP_QUE_DIS |   // Compator is disabled
              ADS1115_CFG_LS_DR_1600SPS |     // No. of samples to take
              ADS1115_CFG_MS_MODE_SS |          // Mode is set to single-shot
              ADS1115_CFG_MS_PGA_FSR_4_096V,  // PGA Gain
  .dev_addr = 0x48,
};

void analogSensors_initADS1115()
{
    ESP_LOGE(TAG,"Starting sensors");
    // Setup I2C
    i2c_param_config(I2C_NUM, &i2c_cfg);
    i2c_driver_install(I2C_NUM, I2C_MODE, I2C_RX_BUF_STATE, I2C_TX_BUF_STATE, I2C_INTR_ALOC_FLAG);

    // Setup ADS1115
    ADS1115_initiate(&ads1115_cfg);
}

float convertVoltageToNTU(float voltage)
{
    // -1120,4x²+5742,3x-4352,9 VOLTAGE TO NTU

    float x  = voltage *2; 
    float ntu = (-(1120.4) * ((x)*(x)))+(5742.3 * (x))-(4358.8);
    return ntu;
}

float getTurbiditySensor()
{    
    // Request single ended on pin AIN1  
    ADS1115_request_single_ended_AIN1();
    while(!ADS1115_get_conversion_state()) 
      vTaskDelay(pdMS_TO_TICKS(5));          // wait 5ms before check again
    // analogSensorsResult.pHSensorResult = ADS1115_get_conversion(); 
    float voltage = convertRawToVoltage();
    float ntu = convertVoltageToNTU(voltage);
    ESP_LOGD(TAG,"Voltage AN1: %f", voltage);
    ESP_LOGD(TAG,"NTU AN1: %f", ntu);
    return ntu;

}

/*float getpHSensor ()
{
   // Request single ended on pin AIN2  
    ADS1115_request_single_ended_AIN2();
    while(!ADS1115_get_conversion_state()) 
      vTaskDelay(pdMS_TO_TICKS(5));          // wait 5ms before check again
    analogSensorsResult.turbiditySensorResult = ADS1115_get_conversion();   
    // // ESP_LOGD(TAG,"tubiditySensor Value: %d", analogSensorsResult.turbiditySensorResult);  
    // ESP_LOGD(TAG,"Voltage AN2: %f", voltage);
}
*/

void analogSensors_service(void *pvParameters)
{
 // Buffer for result
    // analogSensors_t analogSensorsResult = {.pHSensorResult=0, .turbiditySensorResult=0};
    analogSensors_initADS1115();
  while(1) 
  {
    getTurbiditySensor();
    vTaskDelay(pdMS_TO_TICKS(ANALOG_SENSOR_TASK_DELAY));
  }
    
    ESP_LOGD(TAG,"Should not reach here!");
    vTaskDelete(NULL);  
}   