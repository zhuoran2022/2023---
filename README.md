# 2023结对编程-数独游戏开发日志
- 0620 创建github仓库，调研作业要求中的框架、环境
- 0624 pyt（第一次push）: 创建数独需要的初始环境，定义工具结点类、玩家类
    - 目前程序完成环境初始化
- 0626 pyt（第二次push）：为数独游戏添加功能1：生成不重复的数独终局至文件
    - 目前程序可以生成不重复的数独终局，并写入文件
- 0626 wzr（第一次push）: 为数独游戏添加-c和-s的控制台参数并实现其解析、执行
    - 目前程序可以读入待求解数独棋盘，空格用"$"代替
    - 目前程序可以输入需要的数独终盘数量，并将指定个终局输出到若干个txt文件中
    - 目前程序可以接收需要解的数独棋盘文件路径，从中读取一个或若干个无分割陈列的9×9数独棋盘，生成解答，并输出到sudoku.txt中，输出文件棋盘之间由“===”分割
    - 以上输入、输出文件位置都在.exe的同名文件夹下
- 0626 wzr（第二次push）: 添加了-n、-m、-r参数
    - 目前程序可以指定需要的游戏数量，程序会将指定数目的数独问题打印在控制台上，并调用求解函数给出它们的解
    - 在指定游戏数量的同时，可以指定生成游戏的难度，分为1-3三个等级，分别对应30、40、50个挖空
    - 也可以越过难度等级直接指定挖空的数量，范围指定命令强于难度等级指定命令。但还没有实现输入挖空的数量范围功能。
- 0626 wzr（第三次push）:修正了-r参数，部分实现了-u参数的处理
    - 目前程序可以正常地用-r参数输入诸如20~55的数量范围，可以分割字符串读出20和55，并生成[22, 55]范围内的随机数作为挖空数量
    - u参数目前可以读取为bool值并影响数独游戏生成，目前框架是每次挖空时判断一下当前数独棋盘解的个数，如果多于1就重新挖这个空。但判断解的个数是否多于1的函数还没有实现，目前恒返回true（代表解唯一）。
