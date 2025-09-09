/*
改进方向：[ ] 将底层绘制函数和w_all合并入WouoUI对象，便于多个UI对象的同时使用
         [ ] 将各个参数改为页面百分比，实现多尺寸页面的适配
*/

#ifndef __OLED_UI_H__
#define __OLED_UI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI_conf.h"
#include "WouoUI_oled.h"
#include "WouoUI_page.h"
#include "WouoUI_widget.h"
#include "WouoUI_win.h"
#include "stdbool.h"
#include "stdio.h"

// 定义字符串类型
typedef char *my_String;
/*============================================常量定义=========================================*/
// 输入消息缓冲队列的大小(至少为2)
#define INPUT_MSG_QUNEE_SIZE UI_INPUT_MSG_QUNEE_SIZE
//--------msg_que处理相关函数
#define OLED_MsgQueIsEmpty (p_cur_ui->msg_que_font == p_cur_ui->msg_que_rear)                             // 队列空
#define OLED_MsgQueIsFull ((p_cur_ui->msg_que_rear + 1) % INPUT_MSG_QUNEE_SIZE == p_cur_ui->msg_que_font) // 队列满

//------类别下标声明。用于UI参数数组中做索引
enum _ani_kind // 动画速度类别(数组中的下标)
{
    IND_ANI = 0x00, // 指示器动画速度
    TILE_ANI,       // 磁贴动画速度
    LIST_ANI,       // 列表动画速度
    WIN_ANI,        // 弹窗动画速度
    TAG_ANI,        // 标签动画速度
    DIGI_ANI,       // 数字动画滚动速度
    FADE_ANI,       // 页面渐变退出速度
    AIN_ALL_NUM,    // 动画速度参数的数目，用于数组初始化
};
enum _ufd_kind // 展开方式类别(数组中的下标)
{
    TILE_UFD = 0x00, // 磁贴图标从头展开开关
    LIST_UFD,        // 菜单列表从头展开开关
    UFD_ALL_NUM,     // 展开方式类别数目
};
enum _loop_kind // 循环模式类别(数组中的下标)
{
    TILE_LOOP = 0x00, // 磁贴图标循环模式开关
    LIST_LOOP,        // 菜单列表循环模式开关
    LOOP_ALL_NUM,     // 循环模式类别数目
};

typedef struct UiPara {
    uint8_t page_fadeout : 1;         // 页面退出渐变效果开关
    uint8_t win_blur : 1;             // 弹窗背景虚化开关
    uint8_t digital_ripple : 1;       // digital页面多位数值递增时是否使用ripple模式还是同时运动递增
    uint16_t ani_param[AIN_ALL_NUM];  // 动画参数数组
    uint8_t ufd_param[UFD_ALL_NUM];   // 展开参数数组
    uint8_t loop_param[LOOP_ALL_NUM]; // 循环参数数组
} UiPara;                             // UI参数集合类型
extern UiPara g_default_ui_para;      // 共外部使用的全局UI参数变量

//---------输入消息枚举类型
typedef enum {
    msg_none = 0x00, // none表示没有操作
    msg_add,         // 加，用于数值调整
    msg_sub,         // 减，用于数值调整
    msg_up,          // 上，或者last消息，表上一个
    msg_down,        // 下，或者next消息，表下一个
    msg_click,       // 点击消息，表确认，确认某一选项，回调用一次回调
    msg_return,      // 返回消息，表示返回，从一个页面退出
    msg_home,        // home消息，表回主界面(尚未设计，目前还没有设计对应的功能，默认以page_id为0的页面为主页面)
} InputMsg;          // 输入消息类型，UI设计只供输入5种消息

// 指示器
typedef struct Indicator {
    float x, x_tgt; // 选中框x(以屏幕原点为0)
    float y, y_tgt; // 选中框y
    float w, w_tgt;
    float h, h_tgt;
} Indicator;

typedef struct ScrollBar {
    bool display;   // 是否显示滚动条
    float y, y_tgt; // 滚动条的y值
} ScrollBar;

typedef struct PageFadeOut {
    uint8_t state;
    uint16_t timer;
    PageAddr in_page; // 记录fade out后要进入的页面地址
} PageFadeOut;

// WouoUI类型，整个UI类型
typedef struct WouoUI {
    uint8_t init_finish_flag;                 // 页面是否正在初始化完成的标志位，即是否完成过度页面的初始化
    PageAddr home_page;                       // 主页面的地址
    PageAddr current_page;                    // 当前页面的地址
    UIState state;                            // ui状态变量
    UiPara *upara;                            // ui参数集合
    InputMsg msg_queue[INPUT_MSG_QUNEE_SIZE]; // 消息队列
    uint8_t msg_que_font;                     // 消息队列的头指针
    uint8_t msg_que_rear;                     // 消息队列的尾指针
    Indicator indicator;                      // 指示器
    ScrollBar scrollBar;                      // 滚动条
    PageFadeOut pageFadeOut;                  // 页面渐变退出

    struct TitlePageVar tp_var; // TitlePage共用变量集合
    struct ListPageVar lp_var;  // List页面的共用变量集合(包括Radio和Wave的列表的变量也使用这个共用变量)

    struct DigitalPageVar dp_var; // digital页面的共用变量集合
    struct WaveTag wt_var;        // 波形显示区域的共用变量集合

    SlideValWin slideValWin; // 滑动数值弹窗插件
    ConfWin confWin;         // 确认弹窗插件
    InfoWin infoWin;         // 确认弹窗插件
    SpinWin spinWin;         // 微调数值弹窗插件

    PageBlur pageBlur; // 弹窗背景页面模糊
} WouoUI;

//============================================全局变量的外界声明================================
extern WouoUI *p_cur_ui;
extern window w_all;

/*============================================接口函数=========================================*/
void OLED_WinFSM(Win *w, PageAddr bg, Option *select_item, uint16_t time);

char *ui_itoa(uint32_t num);
char *ui_itoa_str(uint32_t num, char *str);
char *ui_ftoa(float num);

void OLED_MsgQueSend(InputMsg msg);
InputMsg OLED_MsgQueRead(void);

void OLED_SelectDefaultUI(void);
void OLED_SetCurrentUI(WouoUI *ui);
void OLED_NewUI(WouoUI *ui, UiPara *ui_para);

void OLED_UILayerInProc(uint16_t time);
void OLED_UILayerOutProc(uint16_t time);
void OLED_UIPageProc(uint16_t time);
void OLED_UIIndicatorProc(uint8_t time);
void OLED_UIScrollBarProc(uint8_t time);
void OLED_UIProc(uint8_t time);

void OLED_UIJumpToPage(PageAddr self_page, PageAddr terminate_page);
void OLED_UIChangeCurrentPage(PageAddr terminate_page);
Page *OLED_GetCurrentPage(void);

void OLED_Animation(float *a, float *a_tgt, uint16_t aniTime, uint16_t time);

#ifdef __cplusplus
}
#endif

#endif
