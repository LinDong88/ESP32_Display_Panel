/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../esp_panel_lcd_conf_internal.h"
#if ESP_PANEL_DRIVERS_LCD_ENABLE_ST7102

#include "soc/soc_caps.h"

#if SOC_MIPI_DSI_SUPPORTED
#include <stdlib.h>
#include <sys/cdefs.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_st7102.h"

#include "utils/esp_panel_utils_log.h"
#include "esp_utils_helpers.h"
#include "esp_panel_lcd_vendor_types.h"

static const char *TAG = "st7102_mipi";

static esp_err_t panel_st7102_del(esp_lcd_panel_t *panel);
static esp_err_t panel_st7102_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_st7102_init(esp_lcd_panel_t *panel);
static esp_err_t panel_st7102_invert_color(esp_lcd_panel_t *panel, bool invert_color_data);
static esp_err_t panel_st7102_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y);
static esp_err_t panel_st7102_disp_on_off(esp_lcd_panel_t *panel, bool off);
static esp_err_t panel_st7102_sleep(esp_lcd_panel_t *panel, bool sleep);

typedef struct {
    esp_lcd_panel_io_handle_t io;
    int reset_gpio_num;
    uint8_t madctl_val; // save current value of LCD_CMD_MADCTL register
    uint8_t colmod_val; // save current value of LCD_CMD_COLMOD register
    const esp_panel_lcd_vendor_init_cmd_t *init_cmds;
    uint16_t init_cmds_size;
    struct {
        unsigned int reset_level: 1;
    } flags;
    // To save the original functions of MIPI DPI panel
    esp_err_t (*del)(esp_lcd_panel_t *panel);
    esp_err_t (*init)(esp_lcd_panel_t *panel);
} st7102_panel_t;

esp_err_t esp_lcd_new_panel_st7102_mipi(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel)
{
    ESP_RETURN_ON_FALSE(io && panel_dev_config && ret_panel, ESP_ERR_INVALID_ARG, TAG, "invalid argument");
    esp_panel_lcd_vendor_config_t *vendor_config = (esp_panel_lcd_vendor_config_t *)panel_dev_config->vendor_config;
    ESP_RETURN_ON_FALSE(vendor_config && vendor_config->mipi_config.dpi_config && vendor_config->mipi_config.dsi_bus, ESP_ERR_INVALID_ARG, TAG,
                        "invalid vendor config");

    esp_err_t ret = ESP_OK;
    st7102_panel_t *st7102 = (st7102_panel_t *)calloc(1, sizeof(st7102_panel_t));
    ESP_RETURN_ON_FALSE(st7102, ESP_ERR_NO_MEM, TAG, "no mem for st7102 panel");

    if (panel_dev_config->reset_gpio_num >= 0) {
        gpio_config_t io_conf = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num,
        };
        ESP_GOTO_ON_ERROR(gpio_config(&io_conf), err, TAG, "configure GPIO for RST line failed");
    }

    switch (panel_dev_config->color_space) {
    case LCD_RGB_ELEMENT_ORDER_RGB:
        st7102->madctl_val = 0;
        break;
    case LCD_RGB_ELEMENT_ORDER_BGR:
        st7102->madctl_val |= LCD_CMD_BGR_BIT;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported rgb element order");
        break;
    }

    switch (panel_dev_config->bits_per_pixel) {
    case 16: // RGB565
        st7102->colmod_val = 0x55;
        break;
    case 18: // RGB666
        st7102->colmod_val = 0x66;
        break;
    case 24: // RGB888
        st7102->colmod_val = 0x77;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported pixel width");
        break;
    }

    st7102->io = io;
    st7102->reset_gpio_num = panel_dev_config->reset_gpio_num;
    st7102->flags.reset_level = panel_dev_config->flags.reset_active_high;
    st7102->init_cmds = vendor_config->init_cmds;
    st7102->init_cmds_size = vendor_config->init_cmds_size;

    // Create MIPI DPI panel first
    esp_lcd_panel_handle_t dpi_panel = NULL;
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_dpi(vendor_config->mipi_config.dsi_bus, vendor_config->mipi_config.dpi_config, &dpi_panel), err, TAG, "create dpi panel failed");

    // Save original functions
    st7102->del = dpi_panel->del;
    st7102->init = dpi_panel->init;

    // Override functions
    dpi_panel->del = panel_st7102_del;
    dpi_panel->reset = panel_st7102_reset;
    dpi_panel->init = panel_st7102_init;
    dpi_panel->invert_color = panel_st7102_invert_color;
    dpi_panel->mirror = panel_st7102_mirror;
    dpi_panel->disp_on_off = panel_st7102_disp_on_off;
    dpi_panel->disp_sleep = panel_st7102_sleep;

    // Store st7102 panel data
    dpi_panel->user_data = st7102;

    *ret_panel = dpi_panel;
    ESP_LOGI(TAG, "create st7102 mipi panel @%p", dpi_panel);
    return ESP_OK;

err:
    if (st7102) {
        free(st7102);
    }
    return ret;
}

static esp_err_t panel_st7102_del(esp_lcd_panel_t *panel)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    if (st7102->del) {
        ret = st7102->del(panel);
    }

    if (st7102) {
        free(st7102);
    }
    return ret;
}

static esp_err_t panel_st7102_reset(esp_lcd_panel_t *panel)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    if (st7102->reset_gpio_num >= 0) {
        ESP_RETURN_ON_ERROR(gpio_set_level(st7102->reset_gpio_num, st7102->flags.reset_level), TAG, "gpio set level failed");
        vTaskDelay(pdMS_TO_TICKS(10));
        ESP_RETURN_ON_ERROR(gpio_set_level(st7102->reset_gpio_num, !st7102->flags.reset_level), TAG, "gpio set level failed");
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    return ret;
}

static esp_err_t panel_st7102_init(esp_lcd_panel_t *panel)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    // Reset panel first
    ESP_RETURN_ON_ERROR(panel_st7102_reset(panel), TAG, "panel reset failed");

    // Send vendor specific initialization commands
    if (st7102->init_cmds && st7102->init_cmds_size > 0) {
        for (int i = 0; i < st7102->init_cmds_size; i++) {
            ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, st7102->init_cmds[i].cmd, st7102->init_cmds[i].data, st7102->init_cmds[i].data_bytes), TAG, "send command failed");
            if (st7102->init_cmds[i].delay_ms > 0) {
                vTaskDelay(pdMS_TO_TICKS(st7102->init_cmds[i].delay_ms));
            }
        }
    }

    // Set color mode
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_COLMOD, &st7102->colmod_val, 1), TAG, "send colmod failed");

    // Set memory access control
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_MADCTL, &st7102->madctl_val, 1), TAG, "send madctl failed");

    // Initialize MIPI DPI panel
    if (st7102->init) {
        ret = st7102->init(panel);
    }

    return ret;
}

static esp_err_t panel_st7102_invert_color(esp_lcd_panel_t *panel, bool invert_color_data)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    uint8_t data = 0;
    esp_err_t ret = ESP_OK;

    if (invert_color_data) {
        data = 0x01;
    }

    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_INVON, &data, 1), TAG, "send invon failed");

    return ret;
}

static esp_err_t panel_st7102_mirror(esp_lcd_panel_t *panel, bool mirror_x, bool mirror_y)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    if (mirror_x) {
        st7102->madctl_val |= LCD_CMD_MX_BIT;
    } else {
        st7102->madctl_val &= ~LCD_CMD_MX_BIT;
    }

    if (mirror_y) {
        st7102->madctl_val |= LCD_CMD_MY_BIT;
    } else {
        st7102->madctl_val &= ~LCD_CMD_MY_BIT;
    }

    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_MADCTL, &st7102->madctl_val, 1), TAG, "send madctl failed");

    return ret;
}

static esp_err_t panel_st7102_disp_on_off(esp_lcd_panel_t *panel, bool off)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    if (off) {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_DISPOFF, NULL, 0), TAG, "send dispoff failed");
    } else {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_DISPON, NULL, 0), TAG, "send dispoff failed");
    }

    return ret;
}

static esp_err_t panel_st7102_sleep(esp_lcd_panel_t *panel, bool sleep)
{
    st7102_panel_t *st7102 = (st7102_panel_t *)panel->user_data;
    esp_err_t ret = ESP_OK;

    if (sleep) {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_SLPIN, NULL, 0), TAG, "send slpin failed");
    } else {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(st7102->io, LCD_CMD_SLPOUT, NULL, 0), TAG, "send slpout failed");
    }

    return ret;
}

#endif // SOC_MIPI_DSI_SUPPORTED

#endif // ESP_PANEL_DRIVERS_LCD_ENABLE_ST7102
