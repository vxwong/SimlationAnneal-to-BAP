题目是基于 http://172.18.57.223 上的游戏。
游戏规则上去玩一下就知道了，目的是使得 f(x)=100*未停放的小船数+2*小船的等待时间+最后一只船离开的时间。
其中，小船的等待时间 = 你安排小船放的时间-小船的到达时间（Arriival time）
理论上，12关，每一关小船都可以全部放进去。

测试文件共12个，第一行是关卡的berth，time和小船数n。下面n行是小船的到达时间、服务时间、和所需要的berth数。

exe的文件输入格式为“1.txt”，需要注意的是，测试文件需要和cpp文件需要放在同一个文件夹下面（废话= =）。
