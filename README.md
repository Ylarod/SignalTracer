# SignalTracer

一个追踪信号的内核模块，支持内核5.10+

## Usage / 使用方法

```
# ls /sys/module/SignalTracer/parameters/
dump_kernel_stack  match_from_uid  match_sig  match_target_uid
```

## Start stacktrace / 开启调用栈打印

`echo Y > /sys/module/SignalTracer/parameters/dump_kernel_stack`

## Stop stacktrace / 关闭调用栈打印

`echo Y > /sys/module/SignalTracer/parameters/dump_kernel_stack`

## Stacktrace config / 调用栈打印配置

**-1 means no restrictions**

```
# only print stacktrace when SIGKILL / 只打印 SIGKILL
echo 9 > /sys/module/SignalTracer/parameters/match_sig
# only print stacktrace when root kill someone / 只在 root 进程杀死其他进程的时候打印
echo 0 > /sys/module/SignalTracer/parameters/match_from_uid
# only print stacktrace when shell is killed / 只在 shell 进程被杀死的时候打印
echo 2000 > /sys/module/SignalTracer/parameters/match_target_uid

# With 3 restrictions above, only print when root kill -9 shell
# 有了上面的三个限制，只在 root 进程杀死 shell 进程的时候打印调用栈
```