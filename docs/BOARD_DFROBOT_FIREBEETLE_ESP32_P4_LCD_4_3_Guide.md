# BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_4_3 使用指南

## 概述

BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_4_3 是专为TL043WVV02-B1900A屏幕设计的板卡配置，支持480x800分辨率和2通道MIPI DSI接口。

## 板卡规格

- **板卡名称**: FIREBEETLE-ESP32-P4-LCD-4.3
- **屏幕型号**: TL043WVV02-B1900A
- **分辨率**: 480x800
- **屏幕尺寸**: 4.3英寸
- **接口类型**: 2通道MIPI DSI
- **颜色格式**: RGB888 (支持RGB565, RGB666, RGB888)
- **控制器**: ST7102

## 硬件连接

### MIPI DSI连接（2通道）

```
ESP32-P4 MIPI DSI    TL043WVV02-B1900A
├─ DSI_CLK_P  ──→ CLK+
├─ DSI_CLK_N  ──→ CLK-
├─ DSI_D0_P   ──→ D0+
├─ DSI_D0_N   ──→ D0-
├─ DSI_D1_P   ──→ D1+
└─ DSI_D1_N   ──→ D1-
```

### 控制信号

- **复位信号**: GPIO4 (低电平有效)
- **背光控制**: GPIO26 (高电平有效)

## 配置方法

### 方法1: 使用menuconfig配置

1. 运行 `idf.py menuconfig`
2. 导航到 `Component config → ESP Panel → Board → Supported Board`
3. 选择 `FIREBEETLE-ESP32-P4-LCD-4.3`
4. 保存配置并编译

### 方法2: 直接修改配置文件

在 `esp_panel_board_supported_conf.h` 文件中：

```c
#define BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_4_3
// 注释掉其他板卡配置
// #define BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_5
// #define BOARD_DFROBOT_FIREBEETLE_ESP32_P4_LCD_8_8
```

## 使用方法

### 基本使用

```cpp
#include "esp_panel.hpp"

using namespace esp_panel;

void app_main()
{
    // 创建板卡实例
    auto board = BoardFactory::create<Board>();
    if (!board) {
        ESP_LOGE("BOARD_4_3", "创建板卡失败");
        return;
    }

    // 初始化板卡
    if (!board->init()) {
        ESP_LOGE("BOARD_4_3", "板卡初始化失败");
        return;
    }

    // 获取LCD驱动
    auto lcd = board->getLcd();
    if (lcd) {
        // 配置LCD功能
        lcd->configInvertColor(false);
        lcd->configMirror(false, false);
        lcd->configGap(0, 0);
        
        // 打开显示
        lcd->on();
    }

    // 获取背光控制
    auto backlight = board->getBacklight();
    if (backlight) {
        backlight->setBrightness(100);  // 设置100%亮度
    }
}
```

### 高级功能

```cpp
// 获取板卡信息
ESP_LOGI("BOARD_4_3", "板卡名称: %s", board->getName().c_str());
ESP_LOGI("BOARD_4_3", "屏幕分辨率: %dx%d", lcd->getWidth(), lcd->getHeight());

// LCD控制
lcd->configInvertColor(true);   // 反转颜色
lcd->configMirror(true, false); // 水平镜像
lcd->off();                     // 关闭显示
lcd->on();                      // 打开显示

// 背光控制
backlight->setBrightness(50);   // 设置50%亮度
backlight->off();               // 关闭背光
backlight->on();                // 打开背光
```

## 配置参数详解

### LCD配置

```c
// 分辨率
#define ESP_PANEL_BOARD_WIDTH               (480)
#define ESP_PANEL_BOARD_HEIGHT              (800)

// MIPI DSI配置
#define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_NUM           (2)     // 2通道
#define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_RATE_MBPS     (500)   // 500Mbps
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_CLK_MHZ            (9)     // 9MHz时钟

// 时序参数
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_HPW                (2)     // 水平脉冲宽度
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_HBP                (40)    // 水平后沿
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_HFP                (40)    // 水平前沿
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_VPW                (2)     // 垂直脉冲宽度
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_VBP                (10)    // 垂直后沿
#define ESP_PANEL_BOARD_LCD_MIPI_DPI_VFP                (180)   // 垂直前沿

// 颜色配置
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB888)
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)     // RGB顺序
#define ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT    (0)     // 不反转
```

### 背光配置

```c
// 背光控制
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (1)
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO)
#define ESP_PANEL_BOARD_BACKLIGHT_IO            (26)    // GPIO26
#define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL      (1)     // 高电平有效
```

## 初始化命令

板卡配置包含了完整的TL043WVV02-B1900A初始化命令序列：

- 基本配置命令
- 电源配置（VGH/VGL, Source, VCOM）
- 伽马校正
- GIP配置
- 显示控制命令

这些命令基于厂商提供的 `TL043WVV02-B1900A 2Lane Code V1.0.txt` 文件。

## 示例程序

完整的使用示例请参考：
- `examples/board_firebeetle_esp32_p4_lcd_4_3_example.cpp`

示例包含：
- 基本板卡初始化
- 工厂模式使用
- 板卡信息显示
- LCD和背光控制

## 故障排除

### 常见问题

1. **显示无信号**
   - 检查MIPI DSI连接
   - 确认使用2通道配置
   - 检查时钟频率设置

2. **显示异常**
   - 检查时序参数
   - 确认颜色格式配置
   - 检查初始化命令

3. **背光不亮**
   - 检查GPIO26连接
   - 确认背光控制逻辑

### 调试信息

启用调试日志：

```cpp
esp_log_level_set("st7102", ESP_LOG_DEBUG);
esp_log_level_set("st7102_mipi", ESP_LOG_DEBUG);
esp_log_level_set("board", ESP_LOG_DEBUG);
```

## 与内置Demo的兼容性

配置完成后，您可以直接使用ESP Panel的内置demo：

```bash
# 编译并运行LVGL demo
idf.py build
idf.py flash monitor

# 或者运行其他内置示例
cd examples/platformio/lvgl_v8_port
idf.py build flash monitor
```

## 技术支持

如有问题，请参考：
- ESP Panel文档
- ST7102数据手册
- TL043WVV02-B1900A规格书
- ESP32-P4 MIPI DSI技术参考手册


