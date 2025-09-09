#ifndef __WOUOUI_WIN__
#define __WOUOUI_WIN__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI_conf.h"
#include "WouoUI_oled.h"
#include "WouoUI_page.h"
#include "WouoUI_widget.h"

// 定义字符串类型
typedef char *my_String;

// 弹窗参数
#define WIN_FONT Font_7_12 // 磁贴大标题字体

// 滑动数值弹窗参数
#define SLI_VAL_WIN_FONT Font_6_8        // 弹窗字体
#define SLI_VAL_WIN_W 120                // 弹窗宽度
#define SLI_VAL_WIN_H 32                 // 弹窗高度
#define SLI_VAL_WIN_R 2                  // 弹窗倒角
#define SLI_VAL_WIN_Y_TGT -SLI_VAL_WIN_H // 弹窗竖直方向退场终止位置
#define SLI_VAL_WIN_BAR_W 66             // 进度条宽度
#define SLI_VAL_WIN_BAR_H 7              // 进度条高度
#define SLI_VAL_WIN_BAR_R 1              // 进度条倒角
#define SLI_VAL_WIN_BAR_Y_OFS 20         // 弹窗进度条相对于弹窗的Y偏移量

// 信息弹窗参数
#define INFO_WIN_FONT Font_6_8     // 弹窗字体
#define INFO_WIN_W 100             // 弹窗宽度
#define INFO_WIN_H 58              // 弹窗高度
#define INFO_WIN_Y_TGT -INFO_WIN_H // 弹窗竖直方向退场终止位置
#define INFO_WIN_R 2               // 弹窗倒角
#define INFO_WIN_STR_Y_OFS 17      // 信息相对于弹窗的Y偏移量

// 确认弹窗参数
#define CON_WIN_FONT Font_6_8    // 弹窗字体
#define CON_WIN_W 100            // 弹窗宽度
#define CON_WIN_H 58             // 弹窗高度
#define CON_WIN_Y_TGT -CON_WIN_H // 弹窗竖直方向退场终止位置
#define CON_WIN_R 2              // 弹窗倒角
#define CON_WIN_BTN_H 12         // 按键高度
#define CON_WIN_BTN_R 2          // 按键倒角
#define CON_WIN_STR_Y_OFS 17     // 信息相对于弹窗的Y偏移量

// 微调数值弹窗参数
#define SPIN_WIN_FONT Font_6_8      // 弹窗字体
#define SPIN_WIN_NUM_FONT Font_7_12 // 数字字体
#define SPIN_WIN_NUM_W 70           // 数字总宽度
#define SPIN_WIN_W 100              // 弹窗宽度
#define SPIN_WIN_H 44               // 弹窗高度
#define SPIN_WIN_Y_TGT -SPIN_WIN_H  // 弹窗竖直方向退场终止位置

// 定义字符串类型
typedef char *my_String;

// 弹窗类型
typedef struct Win {
    uint8_t state : 2;                             // 弹窗的状态 不再弹窗内0，参数初始化1,弹窗动画轮询2
    uint8_t l;                                     // = (DISP_W - WIN_W) / 2;
    uint8_t r;                                     // = (DISP_W + WIN_W) / 2;
    uint8_t u;                                     // = (DISP_H - WIN_H) / 2;
    uint8_t d;                                     // = (DISP_H + WIN_H) / 2;
    float y;                                       // 弹窗y值
    float y_tgt;                                   // 弹窗y植目标值
    void (*init)(PageAddr bg, Option *sel_item);   // 初始化方法(函数指针，参数为背景页面和选中项)
    void (*show)(Option *sel_item, uint16_t time); // 展示方法(函数指针，参数为选中项)
    void (*react)(PageAddr bg, Option *sel_item);  // 响应方法(函数指针，参数为背景页面和选中项)
} Win;                                             // 数值(确认)弹窗类型
// 有一个全局数值弹窗变量slideValWin在ui结构体中，用于每次弹窗使用(对于数值弹窗来说,不论是上下、还是确认，都会调用一次回调，方便数值实时更新)
// 有一个全局确认弹窗变量confWin在ui结构体中，用于每次确认弹窗使用（对于确认弹窗来说，只有按下确认键，才会触发回调,确认的意义所在）

typedef struct SlideValWin {
    Win win;
    char min_val_str[16];
    char max_val_str[16];
    uint8_t min_val_w;
    uint8_t slider_l, slider_r, slider_u, slider_d;
} SlideValWin;

typedef struct InfoWin {
    Win win;
    uint8_t title_w;
} InfoWin;

typedef struct ConfWin {
    Win win;
    uint8_t title_w;
} ConfWin;

typedef struct SpinWin {
    Win win;
    uint8_t title_w;
    uint8_t range_str_w;
    char range_str[32];
} SpinWin;

void OLED_WinFSM(Win *w, PageAddr bg, Option *sel_item, uint16_t time);
//------------弹窗背景虚化
void OLED_WinBlurAnimInit(void);
void OLED_WinBlurAnim(uint16_t time);
//------------数值弹窗相关函数
void OLED_SlideValWinInit(PageAddr bg, Option *sel_item);
void OLED_SlideValWinShow(Option *sel_item, uint16_t time);
void OLED_SlideValWinReact(PageAddr bg, Option *sel_item);
//------------信息弹窗相关函数
void OLED_InfoWinInit(PageAddr bg, Option *sel_item);
void OLED_InfoWinShow(Option *sel_item, uint16_t time);
void OLED_InfoWinReact(PageAddr bg, Option *sel_item);
//------------确认弹窗相关函数
void OLED_ConWinInit(PageAddr bg, Option *sel_item);
void OLED_ConWinShow(Option *sel_item, uint16_t time);
void OLED_ConWinReact(PageAddr bg, Option *sel_item);
//------------微调数值弹窗相关函数
void OLED_SpinWinInit(PageAddr bg, Option *sel_item);
void OLED_SpinWinShow(Option *sel_item, uint16_t time);
void OLED_SpinWinReact(PageAddr bg, Option *sel_item);

#ifdef __cplusplus
}
#endif

#endif
