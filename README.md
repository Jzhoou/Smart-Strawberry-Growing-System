# 智慧草莓种植系统

## 项目说明

本仓库记录一个智慧草莓种植团队项目。系统围绕种植环境数据采集、现场交互和设备控制展开；本人在团队中仅负责下位机电控部分，包括传感器与执行器接入、控制逻辑、数据通信和串口人机界面联调。仓库内容不代表本人独立完成了整套系统。

## 下位机功能

- 通过机智云（Gizwits）相关程序完成联网数据交互与控制指令处理。
- 使用 DHT11 采集空气温度和湿度，并采集光照、CO₂、土壤 NPK（氮、磷、钾）及 pH 数据。
- 对补光、通风和浇水设备提供手动控制与自动控制逻辑。
- 将自动控制所需阈值保存在 EEPROM 中，使设置能够在设备重新上电后继续使用。
- 通过串口与 HMI 显示屏通信，更新环境数据、接收界面操作并同步控制状态。

## 仓库结构

- `main/`：下位机主程序，包含数据采集、设备控制、机智云通信和串口 HMI 交互代码。
- `screen/`：显示端工程及界面素材。其中 `.HMI` 文件是显示工程，`.zi` 文件是配套字库资产，图片文件用于界面设计。这些文件属于项目资产，应与源码一同保留。

## 使用提示

硬件接口、传感器型号与串口分配以源码中的实际定义为准。部署前请结合自己的接线、显示工程和机智云配置进行核对；本仓库不对测量精度、控制效果或产量提升作量化承诺。

## English Summary

This repository documents a team-built smart strawberry growing system. My contribution was limited to the lower-level embedded electrical-control work: sensor and actuator integration, control logic, data communication, and serial HMI integration. The firmware connects to Gizwits, collects DHT11 temperature/humidity plus light, CO₂, soil NPK, and pH data, and supports manual and automatic control of grow lights, ventilation, and watering. Control thresholds are stored in EEPROM. Files with the `.HMI` and `.zi` extensions are display-project and font assets and are intentionally kept in version control.
