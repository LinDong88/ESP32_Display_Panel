/*
 * 背光测试代码
 * 直接控制GPIO26点亮背光
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "backlight_test";

// 背光控制配置
#define BACKLIGHT_GPIO    26
#define BACKLIGHT_ON_LEVEL 1  // 高电平点亮背光

void backlight_test_task(void *pvParameters)
{
    // 配置GPIO
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << BACKLIGHT_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    ESP_ERROR_CHECK(gpio_config(&io_config));
    ESP_LOGI(TAG, "GPIO%d configured for backlight control", BACKLIGHT_GPIO);
    
    while (1) {
        // 点亮背光
        ESP_LOGI(TAG, "Turning ON backlight (GPIO%d = %d)", BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
        gpio_set_level((gpio_num_t)BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
        vTaskDelay(pdMS_TO_TICKS(3000)); // 等待3秒
        
        // 关闭背光
        ESP_LOGI(TAG, "Turning OFF backlight (GPIO%d = %d)", BACKLIGHT_GPIO, !BACKLIGHT_ON_LEVEL);
        gpio_set_level((gpio_num_t)BACKLIGHT_GPIO, !BACKLIGHT_ON_LEVEL);
        vTaskDelay(pdMS_TO_TICKS(3000)); // 等待3秒
    }
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting backlight test...");
    
    // 创建背光测试任务
    xTaskCreate(backlight_test_task, "backlight_test", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Backlight test task created. Check your screen!");
}


