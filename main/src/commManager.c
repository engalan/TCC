#include "commManager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "protocol_examples_common.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>

#include "interface_html.h"
#include "adcSimulator.h"

#include <stdlib.h>
#include <time.h>


#define QUEUE_RECEIVE_TIMEOUT       5000

static const char *TAG = "example";

static esp_err_t test_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    httpd_resp_sendstr(req, interface_html); // sobre a interface_html
    return ESP_OK;
}

static esp_err_t putValues_post_handler(httpd_req_t *req)
{
    water_params_t values;
    char json_response[100];
    xQueueReceive(*waterParamsgetQueueHandler(), (void *)&values, pdMS_TO_TICKS(QUEUE_RECEIVE_TIMEOUT));

    sprintf(json_response, "{\"ph\":%d, \"temperature\":%d, \"turbidity\":%d}",values.ph, values.temp, values.turbidity);
    ESP_LOGW(TAG, "json_response: %s", json_response);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, json_response); // sobre a interface_html
    return ESP_OK;
}

static const httpd_uri_t teste = {   
    .uri       = "/teste",
    .method    = HTTP_GET,
    .handler   = test_get_handler,
};

static const httpd_uri_t put_data = {    
    .uri       = "/data",
    .method    = HTTP_GET,
    .handler   = putValues_post_handler,
};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/hello", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        return ESP_OK;
    } else if (strcmp("/tentando", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");

        httpd_register_uri_handler(server, &teste);
        httpd_register_uri_handler(server, &put_data);

        #if CONFIG_EXAMPLE_BASIC_AUTH
        httpd_register_basic_auth(server);
        #endif
        return server;
    }
    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

void initHttpService()
{
    static httpd_handle_t server = NULL;
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
    #ifdef CONFIG_EXAMPLE_CONNECT_WIFI
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
    #endif // CONFIG_EXAMPLE_CONNECT_WIFI
    server = start_webserver();
}

/*
void app_main(void)
{   
    initWifi();
}
*/