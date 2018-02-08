# gomokuer

基于强化学习的一个小小的五子棋AI.

规则: 无禁手. 有禁没做进去...

PS: Python部分代码写得比较偷懒, 见谅.


# Timeline

Day 0: 乱下.

Day 2: 击败作者.

Day 3: QQ游戏大厅五子棋高分区, 执白高胜率.


# 可能需要

Windows 10; NVIDIA显卡; Python 3.6; VS 2017; CUDA 9.1; PyTorch 0.3.0


# 安装步骤

1. Python 3.6

    最好用Anaconda (因为PyTorch并不官方支持Windows, 用conda安装比较方便).

    链接: https://www.anaconda.com/download/

    安装到英文路径, 并且把该路径添加到PATH和PYTHONHOME环境变量里.


2. CUDA 9.1

    链接: https://developer.nvidia.com/cuda-downloads


3. PyTorch 0.3.0

    开始菜单里找到Anaconda, 启动Anaconda Prompt.

    运行: conda install -c peterjc123 pytorch cuda90

    可能要很久, 有科学上网工具会比较快.


# 启动

打开命令行, cd到./py目录下. (历史遗留问题...)

运行: ../gomokuer.exe play -w ../weights/122.pkl -k 48000 -c w

-w: 要加载的权重

-k: 执行几次搜索

-c: AI的颜色, b=黑, w=白


# 从0训练

清空weights文件夹, 最好备份.

打开命令行, cd到./py目录下.

运行: python scratch.py

然后: 看下面↓↓


# 继续训练

打开命令行, cd到./py目录下.

运行: python fullauto.py

默认打开5进程, 内存不够的话可以考虑减少进程数量, 在config.py里设置.