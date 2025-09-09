#include <Arduino.h>
#include <SPI.h>
#include <ST73XX_UI.h>
#include <ST7305_2p9_BW_DisplayDriver.h>
#include <U8g2_for_ST73XX.h>
#include <Wire.h>
#include <AHTxx.h>
#include "WouoUI/WouoUI.h"
#include "WouoUI/WouoUI_user.h"
#include <MillisTaskManager.h>
#include "Button2.h"

MillisTaskManager task;

#define DC_PIN   7
#define RES_PIN  6
#define CS_PIN   10
#define SCLK_PIN 2
#define SDIN_PIN 3

ST7305_2p9_BW_DisplayDriver display(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN, SPI);
U8G2_FOR_ST73XX u8g2_for_st73xx;

float ahtValue;                               //to store T/RH result
AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR); //sensor address, sensor type

#define FORE_COLOR LIGHTBLUE // 前景颜色
#define BACK_COLOR WHITE     // 背景颜色

#define pixelSeize 1

uint16_t fps = 0;

#define BUTTON_PIN0 9
Button2 button0;


void simulateSendBuff(uint8_t buff[OLED_HEIGHT / 8][OLED_WIDTH]) {
    display.clearDisplay();
    for (uint8_t i = 0; i < OLED_HEIGHT / 8; i++)
        for (uint8_t j = 0; j < OLED_WIDTH; j++)
            for (uint8_t n = 0; n < 8; n++) {
                if (buff[i][j] & 0x01 << n) {
                    uint16_t x = j * pixelSeize, y = (i * 8 + n) * pixelSeize;
                    // display.drawFilledRectangle(x, y, x + pixelSeize - 1, y + pixelSeize - 1, ST7305_COLOR_BLACK);
                    // display.drawFilledRectangle(y, x, y + pixelSeize - 1, x + pixelSeize - 1, ST7305_COLOR_BLACK);
                    display.drawFilledRectangle(y, 383 - (x + pixelSeize - 1),y + pixelSeize - 1, 383 - x,  ST7305_COLOR_BLACK);
                }
            }
    display.display();
    fps++;
}

void TimerEvent1() {
    OLED_UIProc(20);
}

void TimerEvent2() {
    OLED_RefreshBuff();
}

void handler(Button2& btn) {
    Serial.printf("button %d: ", btn.getID());
    switch (btn.getType()) {
        case single_click:
        OLED_MsgQueSend(msg_up);
        OLED_MsgQueSend(msg_sub);
            break;
        case double_click:
        OLED_MsgQueSend(msg_click);
            Serial.print("double ");
            break;
        case triple_click:
        OLED_MsgQueSend(msg_down);
        OLED_MsgQueSend(msg_add);
            Serial.print("triple ");
            break;
        case long_click:
        OLED_MsgQueSend(msg_return);
            Serial.print("long");
            break;
    }
    Serial.print("click");
    Serial.print(" (");
    Serial.print(btn.getNumberOfClicks());    
    Serial.println(")");
}

/*----------------------------------------------------------------
void TimerEvent3() {
    if (GetAsyncKeyState(VK_ESCAPE))
        OLED_MsgQueSend(msg_return);

    if (GetAsyncKeyState(VK_F1)) {
        OLED_MsgQueSend(msg_up);
        OLED_MsgQueSend(msg_sub);
    }

    if (GetAsyncKeyState(VK_F2))
        OLED_MsgQueSend(msg_click);

    if (GetAsyncKeyState(VK_F3)) {
        OLED_MsgQueSend(msg_down);
        OLED_MsgQueSend(msg_add);
    }

    if (GetAsyncKeyState(VK_F4))
        OLED_MsgQueSend(msg_sub);

    if (GetAsyncKeyState(VK_F5)) {
        OLED_MsgQueSend(msg_add);
    }
}
----------------------------------------------------------------*/

void TimerEvent4() {
    printf("fps:%d\n", fps);
    fps = 0;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Hello Arduino! (V8.0.X)");

    aht20.begin(5, 4, 400000);

    display.initialize();
    // display.Low_Power_Mode();
    display.High_Power_Mode();
    display.display_on(true);
    display.display_Inversion(false);

    u8g2_for_st73xx.begin(display);                 // connect u8g2 procedures to Adafruit GFX

    display.clearDisplay();
    display.display();

    // 增加task任务
    task.Register(TimerEvent1, 20);
    task.Register(TimerEvent2, 25);
    // task.Register(TimerEvent3, 200);
    task.Register(TimerEvent4, 1000);

    // WouoUI
    TestUI_Init();

    pinMode(BUTTON_PIN0, INPUT_PULLUP); /* make the LED pin an output */

    button0.setLongClickTime(1000);
    button0.begin(BUTTON_PIN0);
    button0.setClickHandler(handler);
    button0.setLongClickDetectedHandler(handler);  // this will only be called upon detection
    button0.setDoubleClickHandler(handler);
    button0.setTripleClickHandler(handler);
}

void loop() {
    task.Running(millis());
    button0.loop();
}
