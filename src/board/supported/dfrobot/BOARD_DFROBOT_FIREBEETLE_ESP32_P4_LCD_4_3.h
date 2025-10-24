/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file   BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_4_3.h
 * @brief  Configuration file for DFRobot FIREBEETLE-ESP32-P4-LCD-4.3 with TL043WVV02-B1900A
 * @author DFRobot
 * @link   https://www.dfrobot.com/
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Board name
 */
#define ESP_PANEL_BOARD_NAME                "DFRobot:FIREBEETLE_ESP32_P4_LCD_4_3"

/**
 * @brief Panel resolution configuration in pixels
 */
#define ESP_PANEL_BOARD_WIDTH               (480)  // Panel width (horizontal, in pixels)
#define ESP_PANEL_BOARD_HEIGHT              (800)  // Panel height (vertical, in pixels)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief LCD panel configuration flag (0/1)
 *
 * Set to `1` to enable LCD panel support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_LCD             (1)

#if ESP_PANEL_BOARD_USE_LCD
/**
 * @brief LCD controller selection
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER      SIMPLE

/**
 * @brief LCD bus type selection
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_MIPI_DSI)

/**
 * @brief LCD bus parameters configuration
 *
 * Configure parameters based on the selected bus type. Parameters for other bus types will be ignored.
 * For detailed parameter explanations, see:
 * https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32s3/api-reference/peripherals/lcd/index.html
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI

    /**
     * @brief MIPI DSI bus
     */
    /* For host */
    #define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_NUM           (2)     // ESP32-P4 supports 1 or 2 lanes, TL043WVV02-B1900A uses 2 lanes
    #define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_RATE_MBPS     (1000)   // Single lane bit rate for TL043WVV02-B1900A
                                                                    // ESP32-P4 supports max 1500Mbps
    /* For refresh panel (DPI) */
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_CLK_MHZ            (28)     // Clock frequency for 480x800@60Hz
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_PIXEL_BITS         (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                                    // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HPW                (2)     // Horizontal pulse width
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HBP                (40)    // Horizontal back porch
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HFP                (40)    // Horizontal front porch
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VPW                (2)     // Vertical pulse width
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VBP                (10)    // Vertical back porch
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VFP                (180)   // Vertical front porch
    /* For DSI power PHY */
    #define ESP_PANEL_BOARD_LCD_MIPI_PHY_LDO_ID             (3)     // -1 if not used.

#endif // ESP_PANEL_BOARD_LCD_BUS_TYPE

/**
 * @brief LCD vendor initialization commands
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * The initialization sequence can be specified in two formats:
 * 1. Raw format:
 *    {command, (uint8_t []){data0, data1, ...}, data_size, delay_ms}
 * 2. Helper macros:
 *    - ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, {data0, data1, ...})
 *    - ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() {               \
        {0x11, (uint8_t []){0x00}, 0, 120},                   \
        {0x99, (uint8_t []){0x71, 0x02, 0xa2}, 3, 0},        \
        {0x99, (uint8_t []){0x71, 0x02, 0xa3}, 3, 0},        \
        {0x99, (uint8_t []){0x71, 0x02, 0xa4}, 3, 0},        \
        {0xA4, (uint8_t []){0x31}, 1, 0},                     \
        {0xB0, (uint8_t []){0x22, 0x57, 0x1E, 0x61, 0x2F, 0x57, 0x61}, 7, 0}, \
        {0xB7, (uint8_t []){0x64, 0x64}, 2, 0},               \
        {0xBF, (uint8_t []){0xB4, 0xB4}, 2, 0},               \
        {0xC8, (uint8_t []){0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, \
                            0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, \
                            0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, \
                            0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 37, 0}, \
        {0xC9, (uint8_t []){0x00, 0x00, 0x0F, 0x1C, 0x34, 0x00, 0x60, 0x03, 0xA0, \
                            0x06, 0x10, 0xFE, 0x06, 0x74, 0x03, 0x21, 0xC4, 0x00, 0x08, \
                            0x00, 0x22, 0x46, 0x0F, 0x8F, 0x0A, 0x32, 0xF2, 0x0C, 0x42, \
                            0x0C, 0xF3, 0x80, 0x00, 0xAB, 0xC0, 0x03, 0xC4}, 37, 0}, \
        {0xD7, (uint8_t []){0x10, 0x0C, 0x36, 0x19, 0x90, 0x90}, 6, 0}, \
        {0xA3, (uint8_t []){0x51, 0x03, 0x80, 0xCF, 0x44, 0x00, 0x00, 0x00, 0x00, \
                            0x04, 0x78, 0x78, 0x00, 0x1A, 0x00, 0x45, 0x05, 0x00, 0x00, \
                            0x00, 0x00, 0x46, 0x00, 0x00, 0x02, 0x20, 0x52, 0x00, 0x05, \
                            0x00, 0x00, 0xFF}, 32, 0}, \
        {0xA6, (uint8_t []){0x02, 0x00, 0x24, 0x55, 0x35, 0x00, 0x38, 0x00, 0x78, \
                            0x78, 0x00, 0x24, 0x55, 0x36, 0x00, 0x37, 0x00, 0x78, 0x78, \
                            0x02, 0xAC, 0x51, 0x3A, 0x00, 0x00, 0x00, 0x78, 0x78, 0x03, \
                            0xAC, 0x21, 0x00, 0x04, 0x00, 0x00, 0x78, 0x78, 0x3e, 0x00, \
                            0x06, 0x00, 0x00, 0x00, 0x00}, 43, 0}, \
        {0xA7, (uint8_t []){0x19, 0x19, 0x00, 0x64, 0x40, 0x07, 0x16, 0x40, 0x00, \
                            0x04, 0x03, 0x78, 0x78, 0x00, 0x64, 0x40, 0x25, 0x34, 0x00, \
                            0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x64, 0x40, 0x4B, 0x5A, \
                            0x00, 0x00, 0x02, 0x01, 0x78, 0x78, 0x00, 0x24, 0x40, 0x69, \
                            0x78, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x00, 0x44}, 48, 0}, \
        {0xAC, (uint8_t []){0x08, 0x0A, 0x11, 0x00, 0x13, 0x03, 0x1B, 0x18, 0x06, \
                            0x1A, 0x19, 0x1B, 0x1B, 0x1B, 0x18, 0x1B, 0x09, 0x0B, 0x10, \
                            0x02, 0x12, 0x01, 0x1B, 0x18, 0x06, 0x1A, 0x19, 0x1B, 0x1B, \
                            0x1B, 0x18, 0x1B, 0xFF, 0x67, 0xFF, 0x67, 0x00}, 37, 0}, \
        {0xAD, (uint8_t []){0xCC, 0x40, 0x46, 0x11, 0x04, 0x78, 0x78}, 7, 0}, \
        {0xE8, (uint8_t []){0x30, 0x07, 0x00, 0x94, 0x94, 0x9C, 0x00, 0xE2, 0x04, \
                           0x00, 0x00, 0x00, 0x00, 0xEF}, 14, 0}, \
        {0xE7, (uint8_t []){0x8B, 0x3C, 0x00 ,0x0C ,0xF0 ,0x5D, 0x00, 0x5D, 0x00, \
                            0x5D, 0x00, 0x5D, 0x00, 0xFF, 0x00, 0x08, 0x7B, 0x00, 0x00, \
                            0xC8, 0x6A, 0x5A, 0x08, 0x1A, 0x3C, 0x00, 0x81, 0x01, 0xCC, \
                            0x01, 0x7F, 0xF0, 0x22}, 33, 0}, \
        {0x11, (uint8_t []){0x00}, 0, 120},                     \
        {0x29, (uint8_t []){0x00}, 0, 20},                       \
        {0x35, (uint8_t []){0x00}, 1, 0},                       \
    }                                                           \


/**
 * @brief LCD color configuration
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                        // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)     // 0: RGB, 1: BGR
#define ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT    (0)     // 0/1

/**
 * @brief LCD transformation configuration
 */
#define ESP_PANEL_BOARD_LCD_SWAP_XY             (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_GAP_X               (0)     // [0, ESP_PANEL_BOARD_WIDTH]
#define ESP_PANEL_BOARD_LCD_GAP_Y               (0)     // [0, ESP_PANEL_BOARD_HEIGHT]

/**
 * @brief LCD reset pin configuration
 */
#define ESP_PANEL_BOARD_LCD_RST_IO              (10)     // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_LCD_RST_LEVEL           (1)     // Reset active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_USE_LCD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Touch panel configuration flag (0/1)
 *
 * Set to `1` to enable touch panel support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_TOUCH               (1)     // Disable touch for now

#if ESP_PANEL_BOARD_USE_TOUCH
/**
 * @brief Touch controller selection
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        ST7123

/**
 * @brief Touch bus type selection
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE          (ESP_PANEL_BUS_TYPE_I2C)

#if (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C) || \
    (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI)
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 *
 * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
 * So it is not necessary to set the macro to `1`. For other drivers (like `Wire`), please set the macro to `1`
 * ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST        (0)     // 0/1. Typically set to 0
#endif

/**
 * @brief Touch bus parameters configuration
 */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    /**
     * @brief I2C bus
     */
    /* For general */
    #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID           (0)     // Typically set to 0
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (100 * 1000)
                                                                // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (0)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (0)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (8)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (7)
#endif
    /* For panel */
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS           (0)     // Typically set to 0 to use the default address.

#endif // ESP_PANEL_BOARD_TOUCH_BUS_TYPE

/**
 * @brief Touch panel transformation flags
 */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY           (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X          (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y          (0)     // 0/1

/**
 * @brief Touch panel control pins
 */
#define ESP_PANEL_BOARD_TOUCH_RST_IO            (-1)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL         (0)     // Reset active level, 0: low, 1: high
#define ESP_PANEL_BOARD_TOUCH_INT_IO            (-1)    // Interrupt pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL         (0)     // Interrupt active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_USE_TOUCH

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Backlight configuration flag (0/1)
 *
 * Set to `1` to enable backlight support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (1)

#if ESP_PANEL_BOARD_USE_BACKLIGHT
/**
 * @brief Backlight control type selection
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO)

#if (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_EXPANDER) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

    /**
     * @brief Backlight control pin configuration
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_IO        (11)    // Output GPIO pin number
    #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL  (1)     // Active level, 0: low, 1: high

#elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM

    /**
     * @brief Custom backlight function
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION(percent, user_data)  \
        do { \
            (void)percent; \
            (void)user_data; \
        } while (0)

#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

/**
 * @brief Backlight idle state configuration (0/1)
 *
 * Set to 1 if want to turn off the backlight after initializing. Otherwise, the backlight will be on.
 */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (0)

#endif // ESP_PANEL_BOARD_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief IO expander configuration flag (0/1)
 *
 * Set to `1` to enable IO expander support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_EXPANDER            (0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please utilize the following macros to execute any additional code if required /////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions. These version numbers are used to check compatibility between this
 * configuration file and the library. Rules for version numbers:
 * 1. Major version mismatch: Configurations are incompatible, must use library version
 * 2. Minor version mismatch: May be missing new configurations, recommended to update
 * 3. Patch version mismatch: No impact on functionality
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-ON*
