================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了采用CAN2.0协议、扩展帧、外部回环的轮询方式与PCAN-View的通信功能，MCU
首先自动发送8byte数据0x1~0x8，MCU接收到数据后，自动将接收到数据通过串口打出。

Function descriptions:
This example demonstrates the communication function with PCAN-View using the
CAN2.0 protocol, extended frames, and external loopback polling method. The MCU
first automatically sends 8-byte data 0x1~0x8, and after receiving the data, the
MCU automatically prints the received data through the serial port.
================================================================================
测试环境：
测试用板：PY32F072_STK
MDK版本： 5.28
IAR版本： 9.20
GCC 版本：GNU Arm Embedded Toolchain 10.3-2021.10

Test environment:
Test board: PY32F072_STK
MDK Version: 5.28
IAR Version: 9.20
GCC Version: GNU Arm Embedded Toolchain 10.3-2021.10
================================================================================
使用步骤：
1. 编译并下载程序；
2. 通过USB转TTL模块连接PC与STK板,STK板与USB转TTL模块的连线方式如下；
STK板        USB转TTL模块
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND       -->  GND
3. 将CAN_RX（PB8）与CAN_TX（PB9）短接；
4. 复位STK板，观察串口是否打印8byte数据0x1~0x8，如果串口打印，表明外部回环模式
测试成功。

Example execution steps:
1.Compile and download the program;
2.Connect the PC to the STK board through the USB to TTL module, and the
connection method between the STK board and the USB to TTL module is as follows:
STK board   USB to TTL module
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND       -->  GND
3.Connect CAN_RX (PB8) and CAN_TX (PB9) short circuit;
4.Reset the STK board and observe whether the serial port prints 8byte data
0x1~0x8. If the serial port prints, it indicates that the external loopback
mode test is successful.
================================================================================
注意事项：

Notes:

================================================================================