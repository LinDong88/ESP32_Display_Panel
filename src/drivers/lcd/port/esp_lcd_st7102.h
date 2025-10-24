/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>

#include "hal/lcd_types.h"
#include "esp_lcd_panel_vendor.h"

#if SOC_MIPI_DSI_SUPPORTED
#include "esp_lcd_mipi_dsi.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_LCD_ST7102_VER_MAJOR    (1)
#define ESP_LCD_ST7102_VER_MINOR    (0)
#define ESP_LCD_ST7102_VER_PATCH    (0)

/**
 * @brief Create LCD panel for model ST7102
 *
 * @note  This function creates a MIPI-DSI LCD panel for ST7102 display controller.
 * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for initialization sequence code.
 *
 * @param[in]  io LCD panel IO handle
 * @param[in]  panel_dev_config General panel device configuration (`vendor_config` and `mipi_dsi_config` are necessary)
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *      - ESP_ERR_INVALID_ARG   if parameter is invalid
 *      - ESP_OK                on success
 *      - Otherwise             on fail
 */
esp_err_t esp_lcd_new_panel_st7102(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);

#if SOC_MIPI_DSI_SUPPORTED
/**
 * @brief Create LCD panel for model ST7102 with MIPI-DSI interface
 *
 * @param[in]  io LCD panel IO handle
 * @param[in]  panel_dev_config General panel device configuration
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *      - ESP_ERR_INVALID_ARG   if parameter is invalid
 *      - ESP_OK                on success
 *      - Otherwise             on fail
 */
esp_err_t esp_lcd_new_panel_st7102_mipi(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Default Configuration Macros for MIPI-DSI Interface //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief MIPI-DSI bus configuration structure for ST7102 (4-lane)
 */
#define ST7102_PANEL_BUS_DSI_4CH_CONFIG()                \
    {                                                    \
        .bus_id = 0,                                     \
        .num_data_lanes = 4,                             \
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,     \
        .lane_bit_rate_mbps = 800,                       \
    }

/**
 * @brief MIPI-DSI bus configuration structure for ST7102 (2-lane)
 */
#define ST7102_PANEL_BUS_DSI_2CH_CONFIG()                \
    {                                                    \
        .bus_id = 0,                                     \
        .num_data_lanes = 2,                             \
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,     \
        .lane_bit_rate_mbps = 500,                       \
    }

/**
 * @brief MIPI-DBI panel IO configuration structure for ST7102
 *
 */
#define ST7102_PANEL_IO_DBI_CONFIG()  \
    {                                 \
        .virtual_channel = 0,         \
        .lcd_cmd_bits = 8,            \
        .lcd_param_bits = 8,          \
    }

/**
 * @brief MIPI DPI configuration structure for ST7102
 *
 * @note  refresh_rate = (dpi_clock_freq_mhz * 1000000) / (h_res + hsync_pulse_width + hsync_back_porch + hsync_front_porch)
 *                                                      / (v_res + vsync_pulse_width + vsync_back_porch + vsync_front_porch)
 *
 * @param[in] px_format Pixel format of the panel
 *
 */
#define ST7102_1280_720_PANEL_60HZ_DPI_CONFIG(px_format)  \
    {                                                    \
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,     \
        .dpi_clock_freq_mhz = 74,                        \
        .virtual_channel = 0,                            \
        .pixel_format = px_format,                       \
        .num_fbs = 1,                                    \
        .video_timing = {                                \
            .h_size = 1280,                              \
            .v_size = 720,                               \
            .hsync_back_porch = 110,                      \
            .hsync_pulse_width = 40,                     \
            .hsync_front_porch = 220,                    \
            .vsync_back_porch = 5,                       \
            .vsync_pulse_width = 5,                      \
            .vsync_front_porch = 20,                     \
        },                                               \
        .flags.use_dma2d = true,                         \
    }

/**
 * @brief MIPI DPI configuration structure for ST7102 (alternative resolution)
 *
 * @param[in] px_format Pixel format of the panel
 *
 */
#define ST7102_1920_1080_PANEL_60HZ_DPI_CONFIG(px_format)  \
    {                                                    \
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,     \
        .dpi_clock_freq_mhz = 148,                       \
        .virtual_channel = 0,                            \
        .pixel_format = px_format,                       \
        .num_fbs = 1,                                    \
        .video_timing = {                                \
            .h_size = 1920,                              \
            .v_size = 1080,                              \
            .hsync_back_porch = 148,                      \
            .hsync_pulse_width = 44,                     \
            .hsync_front_porch = 88,                     \
            .vsync_back_porch = 36,                      \
            .vsync_pulse_width = 5,                      \
            .vsync_front_porch = 4,                      \
        },                                               \
        .flags.use_dma2d = true,                         \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// TL043WVV02-B1900A Specific Configuration ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief TL043WVV02-B1900A MIPI DPI configuration structure
 * 
 * @note Based on manufacturer's 2-lane configuration
 * @note Resolution: 480x272 (typical for 4.3" display)
 * @note Timing parameters from TL043WVV02-B1900A 2Lane Code V1.0.txt
 *
 * @param[in] px_format Pixel format of the panel
 *
 */
#define TL043WVV02_B1900A_480_272_PANEL_60HZ_DPI_CONFIG(px_format)  \
    {                                                    \
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,     \
        .dpi_clock_freq_mhz = 9,                         \
        .virtual_channel = 0,                            \
        .pixel_format = px_format,                       \
        .num_fbs = 1,                                    \
        .video_timing = {                                \
            .h_size = 480,                               \
            .v_size = 272,                               \
            .hsync_back_porch = 40,                      \
            .hsync_pulse_width = 2,                      \
            .hsync_front_porch = 40,                     \
            .vsync_back_porch = 10,                      \
            .vsync_pulse_width = 2,                      \
            .vsync_front_porch = 180,                    \
        },                                               \
        .flags.use_dma2d = true,                         \
    }

/**
 * @brief TL043WVV02-B1900A initialization commands
 * 
 * @note These commands are extracted from TL043WVV02-B1900A 2Lane Code V1.0.txt
 * @note Commands include power settings, gamma correction, and GIP configuration
 */
#define TL043WVV02_B1900A_INIT_CMDS() \
    { \
        {0x29, {0x99, 0x71, 0x02, 0xa2}, 4, 0}, \
        {0x29, {0x99, 0x71, 0x02, 0xa3}, 4, 0}, \
        {0x29, {0x99, 0x71, 0x02, 0xa4}, 4, 0}, \
        {0x39, {0xA4, 0x31}, 2, 0}, \
        {0x39, {0xB0, 0x22, 0x57, 0x1E, 0x61, 0x2F, 0x57, 0x61}, 8, 0}, \
        {0x39, {0xB7, 0x64, 0x64}, 3, 0}, \
        {0x39, {0xBF, 0xB4, 0xB4}, 3, 0}, \
        {0x29, {0xC8, 0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, 0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, 0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, 0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 40, 0}, \
        {0x29, {0xC9, 0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, 0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, 0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, 0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 40, 0}, \
        {0x39, {0xD7, 0x10, 0x0C, 0x36, 0x19, 0x90, 0x90}, 7, 0}, \
        {0x39, {0xA3, 0x51, 0x03, 0x80, 0xCF, 0x44, 0x00, 0x00, 0x00, 0x00, 0x04, 0x78, 0x78, 0x00, 0x1A, 0x00, 0x45, 0x05, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x02, 0x20, 0x52, 0x00, 0x05, 0x00, 0x00, 0xFF}, 35, 0}, \
        {0x39, {0xA6, 0x02, 0x00, 0x24, 0x55, 0x35, 0x00, 0x38, 0x00, 0x78, 0x78, 0x00, 0x24, 0x55, 0x36, 0x00, 0x37, 0x00, 0x78, 0x78, 0x02, 0xAC, 0x51, 0x3A, 0x00, 0x00, 0x00, 0x78, 0x78, 0x03, 0xAC, 0x21, 0x00, 0x04, 0x00, 0x00, 0x78, 0x78, 0x3e, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 47, 0}, \
        {0x39, {0xA7, 0x19, 0x19, 0x00, 0x64, 0x40, 0x07, 0x16, 0x40, 0x00, 0x04, 0x03, 0x78, 0x78, 0x00, 0x64, 0x40, 0x25, 0x34, 0x00, 0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x64, 0x40, 0x4B, 0x5A, 0x00, 0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x24, 0x40, 0x69, 0x78, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x00, 0x44}, 51, 0}, \
        {0x39, {0xAC, 0x08, 0x0A, 0x11, 0x00, 0x13, 0x03, 0x1B, 0x18, 0x06, 0x1A, 0x19, 0x1B, 0x1B, 0x1B, 0x18, 0x1B, 0x09, 0x0B, 0x10, 0x02, 0x12, 0x01, 0x1B, 0x18, 0x06, 0x1A, 0x19, 0x1B, 0x1B, 0x1B, 0x18, 0x1B, 0xFF, 0x67, 0xFF, 0x67, 0x00}, 40, 0}, \
        {0x39, {0xAD, 0xCC, 0x40, 0x46, 0x11, 0x04, 0x78, 0x78}, 8, 0}, \
        {0x39, {0xE8, 0x30, 0x07, 0x00, 0x94, 0x94, 0x9C, 0x00, 0xE2, 0x04, 0x00, 0x00, 0x00, 0x00, 0xEF}, 15, 0}, \
        {0x39, {0xE7, 0x8B, 0x3C, 0x00, 0x0C, 0xF0, 0x5D, 0x00, 0x5D, 0x00, 0x5D, 0x00, 0x5D, 0x00, 0xFF, 0x00, 0x08, 0x7B, 0x00, 0x00, 0xC8, 0x6A, 0x5A, 0x08, 0x1A, 0x3C, 0x00, 0x81, 0x01, 0xCC, 0x01, 0x7F, 0xF0, 0x22}, 35, 0}, \
        {0x05, {0x11}, 1, 20}, \
        {0x05, {0x29}, 1, 20}, \
        {0x39, {0x35, 0x00}, 2, 0}, \
    }

/**
 * @brief TL043WVV02-B1900A initialization commands size
 */
#define TL043WVV02_B1900A_INIT_CMDS_SIZE (sizeof(TL043WVV02_B1900A_INIT_CMDS()) / sizeof(TL043WVV02_B1900A_INIT_CMDS()[0]))

#ifdef __cplusplus
}
#endif
