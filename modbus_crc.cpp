#include "modbus_crc.h"

quint16 ModbusCRC16(const QByteArray &senddata) {
  int len = senddata.size();
  quint16 crc = 0xFFFF;           // 预置16位crc寄存器，初值全部为1
  quint8 temp;                    // 定义中间变量
  for (int i = 0; i < len; i++) { // 循环计算每个数据
    temp = senddata.at(i);
    crc ^= temp;
    for (int j = 0; j < 8; j++) {
      if ((crc & 0x0001) != 0) { // 判断右移出的是不是1
        crc >>= 1;
        crc ^= 0xA001; // 与多项式进行异或
      } else {         // 如果不是1，则直接移出
        crc >>= 1;
      }
    }
  }
  return crc;
}