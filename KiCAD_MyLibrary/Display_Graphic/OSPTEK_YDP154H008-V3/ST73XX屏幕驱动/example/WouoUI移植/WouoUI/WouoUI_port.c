#include "WouoUI_port.h"

// extern void simulateSendBuff(uint8_t buff[8][128]);
// extern void simulateSendBuff(uint8_t buff[11][192]);
extern void simulateSendBuff(uint8_t buff[15][250]);

// void WouoUI_port_SendBuff(uint8_t buff[8][128]) {
//     simulateSendBuff(buff);
// }

// void WouoUI_port_SendBuff(uint8_t buff[11][192]) {
//     simulateSendBuff(buff);
// }

void WouoUI_port_SendBuff(uint8_t buff[15][250]) {
    simulateSendBuff(buff);
}