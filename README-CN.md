 ![Dark_Logo](./docs/images/Dark_Logo.png)

# Dark Game Engine

[English](./README.md) | 简体中文

Dark主要是基于Windows的交互式应用程序和渲染引擎。目前还没有实现太多。

## 入门

建议使用Visual Studio 2017或2019，Dark在其他开发环境上尚未经过正式测试，而我们专注于Windows构建。

首先使用git clone --recursive `https://github.com/Acmen-Team/Dark` 克隆存储库。

如果存储库以前是非递归克隆的，请使用git submodule update --init克隆必要的子模块。

## 目标
Dark的计划有两个方面：创建功能强大的3D引擎，下一代游戏引擎发展的探索。

## 计划：
- 快速的2D渲染（UI，粒子，子画面等）
- 高保真基于物理的3D渲染（稍后将进行扩展，首先是2D）
- 支持Mac，Linux，Android和iOS
    - 本机渲染API支持（OpenGL，DirectX，Vulkan，Metal）
- 功能齐全的查看器和编辑器应用程序
- 支持完全脚本化的交互和行为
- 集成的第三方2D和3D物理引擎
- 程序化地形和世界生成
- 人工智能
- 音响系统

## 愿景 ：

我们希望使用Dark游戏引擎制作游戏，使用Dark开发进行游戏所需的适当工具。包含以下工作流程：

- 使用Dark编辑器设计游戏场景。
- 在Dark中测试游戏，包括保存/加载创建的游戏的功能。
- 在沙盒中加载并玩游戏。

我们希望每个人都能在所有台式机平台（Windows，Mac和Linux）上玩游戏。在进行初期的迭代后，将尝试“使用Dark创建游戏”，以查看引擎的运行期望。