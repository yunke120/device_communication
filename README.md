## 编译与运行

### 依赖环境

- Qt 5.12+ 或 Qt 6.x（推荐 Qt 6，需包含 Qt Widgets、Qt SerialPort、Qt Network 模块）
- C++17 或更高
- Windows/Linux/MacOS（跨平台）

### 编译步骤

1. 安装 Qt 并配置好 Qt Creator 或命令行环境。
2. 打开 `device_communication.pro` 工程文件，使用 Qt Creator 编译，或在命令行下执行：
   ```sh
   qmake
   make   # 或 mingw32-make / nmake / jom，视你的编译器而定
   ```
3. 运行生成的可执行文件（在 `bin/debug/` 或 `bin/release/` 目录下）。

### Windows 快捷编译

双击 `build.bat` 可自动完成编译。

## 使用说明

1. 启动程序后，在左侧拖拽区选择设备类型（如“8808A（万用表）”），拖拽到设备列表区。
2. 点击“配置”按钮，设置通信参数（如串口号、波特率、IP、端口等）。
3. 配置完成后，可点击“连接”或“断开”按钮进行设备通信管理。
4. 支持设备的动态添加、删除和多设备并行管理。

## 扩展性说明

- **新增设备**：继承 `DeviceBase`，实现设备特有的通信和数据处理逻辑，并在 `DeviceFactory` 注册。
- **新增通信方式**：实现 `ICommInterface` 接口，并在设备配置和工厂中支持新类型。
- **UI 扩展**：可通过 Qt Designer 修改 `.ui` 文件，或直接扩展 `src/ui/` 相关代码。
