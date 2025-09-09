#ifndef __WOUOUI_WIDGET__
#define __WOUOUI_WIDGET__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI_conf.h"
#include "WouoUI_page.h"
#include "WouoUI_oled.h"

// 定义字符串类型
typedef char *my_String;

// 弹窗内小控件类型
typedef struct Widget {
    uint8_t widget_W; // 控件宽
    uint8_t widget_H; // 控件高
    uint8_t l;        // = (DISP_W - widget_W) / 2;
    uint8_t r;        // = (DISP_W + widget_W) / 2;
    uint8_t u;        // = (DISP_H - widget_H) / 2;
    uint8_t d;        // = (DISP_H + widget_H) / 2;
    my_String text;      // 字符串
} Widget;

#ifdef __cplusplus
}
#endif

#endif
