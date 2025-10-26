# TL043WVV02-B1900A 屏幕集成指南

## 概述

本指南说明如何将厂商提供的TL043WVV02-B1900A屏幕配置集成到ESP32 Display Panel的ST7102驱动中。

## 屏幕规格

- **型号**: TL043WVV02-B1900A
- **分辨率**: 480x272
- **接口**: 2通道MIPI DSI
- **尺寸**: 4.3英寸
- **颜色格式**: 支持RGB565, RGB666, RGB888

## 配置参数解析

### 时序参数

从厂商配置文件 `TL043WVV02-B1900A 2Lane Code V1.0.txt` 中提取的时序参数：

```
Hsync: 2    (水平同步脉冲宽度)
HBP: 40     (水平后沿)
HFP: 40     (水平前沿)
Vsync: 2    (垂直同步脉冲宽度)
VBP: 10     (垂直后沿)
VFP: 180    (垂直前沿)
```

### 时钟频率计算

对于480x272@60Hz的显示：
- 总像素时钟 = (480 + 40 + 2 + 40) × (272 + 10 + 2 + 180) × 60
- 总像素时钟 = 562 × 464 × 60 ≈ 15.6 MHz
- 考虑到MIPI DSI的编码效率，实际时钟频率约为9MHz

## 集成步骤

### 1. 启用ST7102驱动

在menuconfig中启用ST7102驱动：

```
Component config → ESP Panel → LCD → Enable used drivers in factory → Use ST7102
```

### 2. 使用预定义配置

```cpp
#include "esp_panel_lcd_st7102.hpp"
#include "esp_lcd_st7102.h"

// 配置2通道MIPI DSI总线
BusFactory::Config bus_config = {
    .bus_type = ESP_PANEL_BUS_TYPE_MIPI_DSI,
    .mipi_dsi_config = {
        .dsi_bus = ST7102_PANEL_BUS_DSI_2CH_CONFIG(),  // 2通道配置
        .dpi_config = TL043WVV02_B1900A_480_272_PANEL_60HZ_DPI_CONFIG(MIPI_DSI_PIXEL_FORMAT_RGB888),
        .panel_io_config = ST7102_PANEL_IO_DBI_CONFIG(),
    },
};

// 配置LCD设备
LCD::Config lcd_config = {
    .width = 480,
    .height = 272,
    .color_bits = 24,  // RGB888
    .rst_io = 4,       // 复位GPIO
    .vendor_config = {
        .flags = {
            .use_mipi_interface = true,
            .reset_level = false,
        },
        .init_cmds = TL043WVV02_B1900A_INIT_CMDS(),
        .init_cmds_size = TL043WVV02_B1900A_INIT_CMDS_SIZE,
    },
};
```

### 3. 创建和初始化LCD

```cpp
// 创建LCD驱动实例
LCD_ST7102 lcd(bus_config, lcd_config);

// 初始化
if (!lcd.init()) {
    ESP_LOGE("TL043WVV02", "LCD初始化失败");
    return;
}

// 打开显示
lcd.on();
```

## 初始化命令详解

厂商提供的初始化命令包含以下部分：

### 1. 基本配置命令
```cpp
{0x29, {0x99, 0x71, 0x02, 0xa2}, 4, 0},
{0x29, {0x99, 0x71, 0x02, 0xa3}, 4, 0},
{0x29, {0x99, 0x71, 0x02, 0xa4}, 4, 0},
```

### 2. 电源配置
```cpp
{0x39, {0xA4, 0x31}, 2, 0},  // 电源控制
{0x39, {0xB0, 0x22, 0x57, 0x1E, 0x61, 0x2F, 0x57, 0x61}, 8, 0},  // VGH/VGL (14V)
{0x39, {0xB7, 0x64, 0x64}, 3, 0},  // Source (5V)
{0x39, {0xBF, 0xB4, 0xB4}, 3, 0},  // VCOM (-2V)
```

### 3. 伽马校正
```cpp
{0x29, {0xC8, ...}, 40, 0},  // 正伽马
{0x29, {0xC9, ...}, 40, 0},  // 负伽马
```

### 4. GIP配置
```cpp
{0x39, {0xD7, 0x10, 0x0C, 0x36, 0x19, 0x90, 0x90}, 7, 0},
{0x39, {0xA3, ...}, 35, 0},
{0x39, {0xA6, ...}, 47, 0},
{0x39, {0xA7, ...}, 51, 0},
{0x39, {0xAC, ...}, 40, 0},
{0x39, {0xAD, ...}, 8, 0},
{0x39, {0xE8, ...}, 15, 0},
{0x39, {0xE7, ...}, 35, 0},
```

### 5. 显示控制
```cpp
{0x05, {0x11}, 1, 20},  // SLEEP OUT (延迟20ms)
{0x05, {0x29}, 1, 20},  // DISP ON (延迟20ms)
{0x39, {0x35, 0x00}, 2, 0},  // Tearing Effect
```

## 硬件连接

### MIPI DSI连接（2通道）

```
ESP32 MIPI DSI    TL043WVV02-B1900A
├─ DSI_CLK_P  ──→ CLK+
├─ DSI_CLK_N  ──→ CLK-
├─ DSI_D0_P   ──→ D0+
├─ DSI_D0_N   ──→ D0-
├─ DSI_D1_P   ──→ D1+
└─ DSI_D1_N   ──→ D1-
```

### 控制信号

- **复位信号**: 连接到任意GPIO引脚（示例中使用GPIO4）
- **背光控制**: 可选，连接到PWM或GPIO

## 自定义配置

如果您需要修改某些参数，可以创建自定义的初始化命令：

```cpp
const esp_panel_lcd_vendor_init_cmd_t custom_init_cmds[] = {
    // 基本配置
    {0x29, {0x99, 0x71, 0x02, 0xa2}, 4, 0},
    
    // 电源配置（可以调整电压参数）
    {0x39, {0xA4, 0x31}, 2, 0},
    {0x39, {0xB0, 0x22, 0x57, 0x1E, 0x61, 0x2F, 0x57, 0x61}, 8, 0},
    
    // 伽马校正（可以调整显示效果）
    {0x29, {0xC8, /* 自定义伽马值 */}, 40, 0},
    
    // 显示控制
    {0x05, {0x11}, 1, 20},
    {0x05, {0x29}, 1, 20},
    {0x39, {0x35, 0x00}, 2, 0},
};
```

## 故障排除

### 常见问题

1. **显示无信号**
   - 检查MIPI DSI连接是否正确
   - 确认使用2通道配置
   - 检查时钟频率设置

2. **显示异常**
   - 检查时序参数是否正确
   - 确认颜色格式配置
   - 检查初始化命令顺序

3. **显示效果不佳**
   - 调整伽马校正参数
   - 检查电源电压设置
   - 调整GIP配置参数

### 调试信息

启用调试日志：

```cpp
esp_log_level_set("st7102", ESP_LOG_DEBUG);
esp_log_level_set("st7102_mipi", ESP_LOG_DEBUG);
```

## 示例代码

完整的使用示例请参考：
- `examples/tl043wvv02_b1900a_example.cpp` - 基本使用示例
- `examples/tl043wvv02_b1900a_example.cpp` - 工厂模式示例
- `examples/tl043wvv02_b1900a_example.cpp` - 自定义配置示例

## 技术支持

如有问题，请参考：
- ESP Panel文档
- ST7102数据手册
- TL043WVV02-B1900A规格书
- ESP32 MIPI DSI技术参考手册


