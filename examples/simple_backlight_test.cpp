/*
 * 简单的背光测试代码
 * 直接控制GPIO26点亮背光，不依赖板卡框架
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "backlight_test";

// 背光控制配置 - 根据您的板卡配置
#define BACKLIGHT_GPIO    26
#define BACKLIGHT_ON_LEVEL 1  // 高电平点亮背光

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== 背光测试开始 ===");
    
    // 配置GPIO26为输出模式
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << BACKLIGHT_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    esp_err_t ret = gpio_config(&io_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GPIO配置失败: %s", esp_err_to_name(ret));
        return;
    }
    
    ESP_LOGI(TAG, "GPIO%d配置成功", BACKLIGHT_GPIO);
    
    // 点亮背光
    ESP_LOGI(TAG, "点亮背光...");
    gpio_set_level((gpio_num_t)BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "背光已点亮！GPIO%d = %d", BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
    
    // 等待5秒
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // 关闭背光
    ESP_LOGI(TAG, "关闭背光...");
    gpio_set_level((gpio_num_t)BACKLIGHT_GPIO, !BACKLIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "背光已关闭！GPIO%d = %d", BACKLIGHT_GPIO, !BACKLIGHT_ON_LEVEL);
    
    // 等待2秒
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 再次点亮背光
    ESP_LOGI(TAG, "再次点亮背光...");
    gpio_set_level((gpio_num_t)BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "背光已点亮！GPIO%d = %d", BACKLIGHT_GPIO, BACKLIGHT_ON_LEVEL);
    
    ESP_LOGI(TAG, "=== 背光测试完成 ===");
    ESP_LOGI(TAG, "如果屏幕背光没有变化，请检查：");
    ESP_LOGI(TAG, "1. GPIO%d是否正确连接到背光控制线", BACKLIGHT_GPIO);
    ESP_LOGI(TAG, "2. 背光激活电平是否正确（当前设置为%d）", BACKLIGHT_ON_LEVEL);
    ESP_LOGI(TAG, "3. 尝试将BACKLIGHT_ON_LEVEL改为%d", !BACKLIGHT_ON_LEVEL);
    
    // 保持背光点亮
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


