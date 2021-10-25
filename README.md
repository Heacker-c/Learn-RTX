# 1. 简述

这是我对 光线追踪 的学习的历程，基于下面的三本书

- Ray Tracing in One Weekend
- Ray Tracing the Next Week
- Ray Tracing the Rest of Your Life
 
> 书籍参考 https://github.com/RayTracing/raytracing.github.io

框架参考之前某大佬的一个框架重新改写的，部分实现可能和书籍略有不同，后续打算完成基于OPENGL实现一版基于computer shader的版本，然后再基于硬件使用vulkan实现一版。

# 2. 使用方法

## 2.1 环境

- Visual Studio 2017+
- Windows10
- Git
- CMake（版本2.8以上）

## 2.2 编译

1. 建立工程

```bash
# 在 cmd 中运行如下命令
mkdir build
cd build
cmake .. -A Win32()
```
的`CMakePredefinedTargets`中，右键`INSTALL`，点击生成

## 2.3 配置

在运行exe前，可以对 `config.out` 进行修改来配置，在调试时在 `config.in` 进行修改来配置

# 3. 交流加群
QQ群号：90813715 暗号：RTX