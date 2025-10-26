/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel.hpp"

using namespace esp_panel;

/**
 * @brief FIREBEETLE-ESP32-P4-LCD-4.3 板卡使用示例
 * 
 * 此示例展示如何使用BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_4_3板卡配置
 * 该板卡使用TL043WVV02-B1900A屏幕，支持480x800分辨率和2通道MIPI DSI接口
 */
void board_firebeetle_esp32_p4_lcd_4_3_example()
{
    ESP_LOGI("BOARD_4_3", "开始初始化FIREBEETLE-ESP32-P4-LCD-4.3板卡");

    // 1. 创建板卡实例
    auto board = BoardFactory::create<Board>();
    if (!board) {
        ESP_LOGE("BOARD_4_3", "创建板卡失败");
        return;
    }

    // 2. 初始化板卡
    if (!board->init()) {
        ESP_LOGE("BOARD_4_3", "板卡初始化失败");
        return;
    }

    ESP_LOGI("BOARD_4_3", "板卡初始化成功");
    ESP_LOGI("BOARD_4_3", "屏幕分辨率: %dx%d", board->getLcd()->getWidth(), board->getLcd()->getHeight());

    // 3. 获取LCD驱动
    auto lcd = board->getLcd();
    if (lcd) {
        // 配置LCD功能
        lcd->configInvertColor(false);  // 不反转颜色
        lcd->configMirror(false, false); // 不镜像
        lcd->configGap(0, 0);           // 无间隙

        // 打开显示
        lcd->on();
        ESP_LOGI("BOARD_4_3", "LCD显示已打开");
    }

    // 4. 获取背光控制
    auto backlight = board->getBacklight();
    if (backlight) {
        // 设置背光亮度为100%
        backlight->setBrightness(100);
        ESP_LOGI("BOARD_4_3", "背光亮度设置为100%%");
    }

    // 5. 获取触摸驱动（如果支持）
    auto touch = board->getTouch();
    if (touch) {
        ESP_LOGI("BOARD_4_3", "触摸功能已启用");
    } else {
        ESP_LOGI("BOARD_4_3", "触摸功能未启用");
    }

    ESP_LOGI("BOARD_4_3", "FIREBEETLE-ESP32-P4-LCD-4.3板卡初始化完成");
}

/**
 * @brief 使用工厂模式创建板卡的示例
 */
void board_firebeetle_esp32_p4_lcd_4_3_factory_example()
{
    ESP_LOGI("BOARD_4_3", "使用工厂模式创建板卡");

    // 使用工厂模式创建板卡
    auto board = BoardFactory::create<Board>();
    
    if (board && board->init()) {
        ESP_LOGI("BOARD_4_3", "工厂模式创建板卡成功");
        
        // 获取LCD并打开显示
        auto lcd = board->getLcd();
        if (lcd) {
            lcd->on();
            ESP_LOGI("BOARD_4_3", "LCD显示已打开");
        }
        
        // 获取背光并设置亮度
        auto backlight = board->getBacklight();
        if (backlight) {
            backlight->setBrightness(80);  // 设置80%亮度
            ESP_LOGI("BOARD_4_3", "背光亮度设置为80%%");
        }
    } else {
        ESP_LOGE("BOARD_4_3", "工厂模式创建板卡失败");
    }
}

/**
 * @brief 板卡信息显示示例
 */
void board_firebeetle_esp32_p4_lcd_4_3_info_example()
{
    ESP_LOGI("BOARD_4_3", "显示板卡信息");

    auto board = BoardFactory::create<Board>();
    if (!board || !board->init()) {
        ESP_LOGE("BOARD_4_3", "板卡初始化失败");
        return;
    }

    // 显示板卡基本信息
    ESP_LOGI("BOARD_4_3", "=== 板卡信息 ===");
    ESP_LOGI("BOARD_4_3", "板卡名称: %s", board->getName().c_str());
    
    // 显示LCD信息
    auto lcd = board->getLcd();
    if (lcd) {
        ESP_LOGI("BOARD_4_3", "=== LCD信息 ===");
        ESP_LOGI("BOARD_4_3", "分辨率: %dx%d", lcd->getWidth(), lcd->getHeight());
        ESP_LOGI("BOARD_4_3", "颜色位数: %d", lcd->getColorBits());
        ESP_LOGI("BOARD_4_3", "控制器: %s", lcd->getName().c_str());
    }
    
    // 显示背光信息
    auto backlight = board->getBacklight();
    if (backlight) {
        ESP_LOGI("BOARD_4_3", "=== 背光信息 ===");
        ESP_LOGI("BOARD_4_3", "背光类型: %s", backlight->getName().c_str());
        ESP_LOGI("BOARD_4_3", "当前亮度: %d%%", backlight->getBrightness());
    }
    
    // 显示触摸信息
    auto touch = board->getTouch();
    if (touch) {
        ESP_LOGI("BOARD_4_3", "=== 触摸信息 ===");
        ESP_LOGI("BOARD_4_3", "触摸控制器: %s", touch->getName().c_str());
    } else {
        ESP_LOGI("BOARD_4_3", "=== 触摸信息 ===");
        ESP_LOGI("BOARD_4_3", "触摸功能: 未启用");
    }
}

/**
 * @brief 主函数示例
 */
extern "C" void app_main()
{
    ESP_LOGI("BOARD_4_3", "FIREBEETLE-ESP32-P4-LCD-4.3 示例程序开始");

    // 运行基本示例
    board_firebeetle_esp32_p4_lcd_4_3_example();
    
    // 等待一段时间
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 运行工厂模式示例
    board_firebeetle_esp32_p4_lcd_4_3_factory_example();
    
    // 等待一段时间
    vTaskDelay(pdMS_TO_TICKS(2000));
    
    // 显示板卡信息
    board_firebeetle_esp32_p4_lcd_4_3_info_example();

    ESP_LOGI("BOARD_4_3", "示例程序完成");
}


