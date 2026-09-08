# 智慧草莓种植系统

## 项目说明

本仓库记录一个智慧草莓种植团队项目。系统围绕种植环境数据采集、现场交互和设备控制展开；本人在团队中仅负责下位机电控部分，包括传感器与执行器接入、控制逻辑、数据通信和串口人机界面联调。仓库内容不代表本人独立完成了整套系统。

## ⚠️ 当前状态与已知问题

当前代码快照存在明确的 GPIO 复用冲突：`KEY1` 与 `water` 都使用 GPIO 6，`KEY2` 与 `Heater` 都使用 GPIO 7。`setup()` 中先将 `water` 和 `Heater` 配置为输出，随后又通过 `KEY1` 和 `KEY2` 将同一组引脚配置为 `INPUT_PULLUP`，因此程序完成初始化后的最终配置是上拉输入。当前快照无法保证浇水和加热控制可靠工作。

修复时必须依据实物接线重新分配引脚并完成实机验证。仓库没有足够信息支持推断替代引脚，现阶段请勿猜测或直接套用新的引脚编号。

## 下位机功能

- 通过机智云（Gizwits）相关程序完成联网数据交互与控制指令处理。
- 使用 DHT11 采集空气温度和湿度，并采集光照、CO₂、土壤 NPK（氮、磷、钾）及 pH 数据。
- 对补光、通风和浇水设备提供手动控制与自动控制逻辑。
- 将自动控制所需阈值保存在 EEPROM 中，使设置能够在设备重新上电后继续使用。
- 通过串口与 HMI 显示屏通信，更新环境数据、接收界面操作并同步控制状态。

## 仓库结构

- `main/`：下位机主程序，包含数据采集、设备控制、机智云通信和串口 HMI 交互代码。
- `screen/`：显示端工程及界面素材。其中 `.HMI` 文件是显示工程，`.zi` 文件是配套字库资产，图片文件用于界面设计。这些文件属于项目资产，应与源码一同保留。

## Arduino IDE 入口与依赖

在 Arduino IDE 中打开 [`main/main.ino`](main/main.ino)。同一 `main/` 目录下的其他 `.ino` 文件属于同一个 sketch，编译时会一并参与构建，不需要逐个单独打开或编译。

依据 `main.ino` 的 `#include`，代码使用以下依赖：`Gizwits`、`Wire`、`SoftwareSerial`、`EEPROM`、`MsTimer2` 和 `dht11`。其中部分库由 Arduino 开发环境提供，其他库需要按实际开发环境另行安装；仓库未锁定具体库版本。

## 使用提示

硬件接口、传感器型号与串口分配以源码中的实际定义为准。部署前请结合自己的接线、显示工程和机智云配置进行核对；本仓库不对测量精度、控制效果或产量提升作量化承诺。

## English Summary

This repository documents a team-built smart strawberry growing system. My contribution was limited to the lower-level embedded electrical-control work: sensor and actuator integration, control logic, data communication, and serial HMI integration. The firmware connects to Gizwits, collects DHT11 temperature/humidity plus light, CO₂, soil NPK, and pH data, and supports manual and automatic control of grow lights, ventilation, and watering. Control thresholds are stored in EEPROM. Files with the `.HMI` and `.zi` extensions are display-project and font assets and are intentionally kept in version control.

**Known issue:** the current snapshot assigns both `KEY1` and `water` to GPIO 6, and both `KEY2` and `Heater` to GPIO 7. Because `setup()` configures the key pins as `INPUT_PULLUP` after configuring the actuator pins as outputs, the final pin mode is input with pull-up; reliable watering and heating operation cannot be guaranteed. Reassign these pins only after checking the real wiring, then verify the change on hardware. Do not guess replacement pins from this repository.

For Arduino IDE, open [`main/main.ino`](main/main.ino); all `.ino` files in that directory are compiled as one sketch. Its direct includes identify the dependencies as `Gizwits`, `Wire`, `SoftwareSerial`, `EEPROM`, `MsTimer2`, and `dht11`. Exact library versions are not pinned here.
