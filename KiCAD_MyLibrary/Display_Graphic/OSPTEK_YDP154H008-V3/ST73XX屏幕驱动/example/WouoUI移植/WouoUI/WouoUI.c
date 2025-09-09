#include "WouoUI.h"
#include "stdio.h"
#include "string.h"

// 静态函数声明(方便全局变量初始化函数指针)
void OLED_WinBlurAnimInit(void);
void OLED_WinBlurAnim(uint16_t time);

//--------UI对象类型
window w_all = {0, 0, OLED_WIDTH, OLED_HEIGHT}; // 全局窗口变量，所有的绘制都在这个窗口内进行

//===================================全局变量==================================
// 全局UI参数集合对象(同时初始化)，这个UI的相关参数都在这个集合中定义
UiPara g_default_ui_para = {
    .page_fadeout = true,   // 页面退出渐变效果开关
    .win_blur = true,       // 弹窗背景虚化开关
    .digital_ripple = true, // digital页面波纹递增动画开关
    .ani_param = {
        [IND_ANI] = 80,   // 指示器动画速度
        [TILE_ANI] = 80,  // 磁贴动画速度
        [LIST_ANI] = 100, // 列表动画速度
        [WIN_ANI] = 80,   // 弹窗动画速度
        [TAG_ANI] = 80,   // 标签动画速度
        [DIGI_ANI] = 80,  // 数字动画滚动速度
        [FADE_ANI] = 30,  // 页面渐变退出速度
    },
    .ufd_param = {
        [TILE_UFD] = false, // 磁贴图标从头展开开关
        [LIST_UFD] = false, // 菜单列表从头展开开关
    },
    .loop_param = {
        [TILE_LOOP] = true, // 磁贴图标循环模式开关
        [LIST_LOOP] = true, // 菜单列表循环模式开关
    },
};

// 默认UI对象（同时进行初始化）
WouoUI default_ui = {
    .init_finish_flag = false, // 初始时需要先开启过度动画
    .home_page = NULL,         // 初始化当前页面和主页面均是NULL
    .current_page = NULL,
    .state = ui_page_in, // 从没页面进入主页面，所以是lay_in
#if PAGE_DIGITAL_ENABLE
    .upara = &g_default_ui_para, // 将默认参数赋值给default_ui
#endif
    .msg_queue = {msg_none},
    .msg_que_font = 0, // 消息队列相关的初始化
    .msg_que_rear = 0,
    .indicator = {
        .x = 0,
        .y = 0,
        .w = OLED_WIDTH,
        .h = OLED_HEIGHT,
    },
    .scrollBar = {
        .display = true,
        .y = 0,
        .y_tgt = 0,
    },
    .pageFadeOut = {
        .state = 0,
        .timer = 0,
    },
    .lp_var = {.radio_click_flag = false},
#if PAGE_DIGITAL_ENABLE
    .dp_var = {
        // 初始化Digital页面共用变量的初始化
        .w_digtal = {
            // 数字Label窗口的坐标设置
            [0] = {.start_x = 16, .w = 96, .h = 24}, // 确定数字窗口的一些参数
            [1] = {.start_x = 0, .w = 128, .h = 16}, // 确定标签窗口的一些参数
        },
        .digital_num_xindex = {12 + 24 + 12 + 24 + 12, 12 + 24 + 12 + 24, 12 + 24 + 12, 12 + 24, 12, 0},
        // 数字框的横坐标数组{12+24+12+24+12, 12+24+12+24, 12+24+12,12+24, 12,0}
        .Gap_shine_time = 0,
        .Uline_shine_time = 0,
        .num_array = {{0}, {0}, {0}, {0}, {0}, {0}},
        .last_or_next_label_index = 0,
        .temp_num_pos = 0,
        .temp_dir = Digital_Direct_None,
        .temp_label_dir = Digital_Direct_None,
    },
#endif
    // 滑动数值弹窗
    .slideValWin = {
        .win = {
            .state = 0, // 默认弹窗是关闭的
            .l = (OLED_WIDTH - SLI_VAL_WIN_W) / 2,
            .r = (OLED_WIDTH + SLI_VAL_WIN_W) / 2,
            .u = (OLED_HEIGHT - SLI_VAL_WIN_H) / 2,
            .d = (OLED_HEIGHT + SLI_VAL_WIN_H) / 2,
            .init = OLED_SlideValWinInit,
            .show = OLED_SlideValWinShow,
            .react = OLED_SlideValWinReact, // 关联相关的处理函数
        },
    },
#if UI_CONWIN_ENABLE
    .confWin = {
        .win = {
            .state = 0, // 默认弹窗是关闭的
            .l = (OLED_WIDTH - CON_WIN_W) / 2,
            .r = (OLED_WIDTH + CON_WIN_W) / 2,
            .u = (OLED_HEIGHT - CON_WIN_H) / 2,
            .d = (OLED_HEIGHT + CON_WIN_H) / 2,
            .init = OLED_ConWinInit,
            .show = OLED_ConWinShow,
            .react = OLED_ConWinReact, // 关联相关的处理函数
        },
    },
#endif
    .infoWin = {
        .win = {
            .state = 0, // 默认弹窗是关闭的
            .l = (OLED_WIDTH - INFO_WIN_W) / 2,
            .r = (OLED_WIDTH + INFO_WIN_W) / 2,
            .u = (OLED_HEIGHT - INFO_WIN_H) / 2,
            .d = (OLED_HEIGHT + INFO_WIN_H) / 2,
            .init = OLED_InfoWinInit,
            .show = OLED_InfoWinShow,
            .react = OLED_InfoWinReact,
        },
    },
    .spinWin = {
        .win = {
            .state = 0, // 默认弹窗是关闭的
            .l = (OLED_WIDTH - SPIN_WIN_W) / 2,
            .r = (OLED_WIDTH + SPIN_WIN_W) / 2,
            .u = (OLED_HEIGHT - SPIN_WIN_H) / 2,
            .d = (OLED_HEIGHT + SPIN_WIN_H) / 2,
            .init = OLED_SpinWinInit,
            .show = OLED_SpinWinShow,
            .react = OLED_SpinWinReact,
        },
    },
};

//--------UI对象类型
WouoUI *p_cur_ui = &default_ui; // 当前操作的ui对象的指针(默认使用defaultui)

/**
 * @brief 将无符号32位整数转换为字符串
 *
 * @param num 要转换的无符号32位整数
 * @return char* 转换后的字符串
 */
char *ui_itoa(uint32_t num) {
    static char str[16] = {0};   // 定义一个静态字符数组，用于存储转换后的字符串
    memset(str, 0, sizeof(str)); // 将字符数组清零
    sprintf(str, "%d", num);     // 将无符号32位整数转换为字符串
    return str;                  // 返回转换后的字符串
}

char *ui_itoa_str(uint32_t num, char *str) {
    memset(str, 0, sizeof(str)); // 将字符数组清零
    sprintf(str, "%d", num);     // 将无符号32位整数转换为字符串
    return str;                  // 返回转换后的字符串
}

/**
 * @brief 将浮点数转换为字符串
 *
 * @param num 要转换的浮点数
 * @return char* 转换后的字符串
 */
char *ui_ftoa(float num) {
    static char str[10] = {0};   // 定义一个静态字符数组，用于存储转换后的字符串
    memset(str, 0, sizeof(str)); // 将字符数组清零
    sprintf(str, "%.2f", num);   // 将浮点数转换为字符串，并保留两位小数
    return str;                  // 返回转换后的字符串
}

/**
 * @brief 向msg队列发送消息
 *
 * @param msg 消息
 */
void OLED_MsgQueSend(InputMsg msg) {
    if (!OLED_MsgQueIsFull) {
        p_cur_ui->msg_queue[p_cur_ui->msg_que_rear] = msg;
        p_cur_ui->msg_que_rear++;
        if (p_cur_ui->msg_que_rear == INPUT_MSG_QUNEE_SIZE)
            p_cur_ui->msg_que_rear = 0;
    }
}

/**
 * @brief 读msg队列消息
 *
 * @return InputMsg 消息
 */
InputMsg OLED_MsgQueRead(void) {
    InputMsg msg = msg_none;
    if (!OLED_MsgQueIsEmpty) {
        msg = p_cur_ui->msg_queue[p_cur_ui->msg_que_font];
        p_cur_ui->msg_que_font++;
        if (p_cur_ui->msg_que_font == INPUT_MSG_QUNEE_SIZE)
            p_cur_ui->msg_que_font = 0;
    }
    return msg;
}

//-------UI相关函数
// 将当前操作的UI对象选择为默认UI对象
void OLED_SelectDefaultUI(void) {
    p_cur_ui = &default_ui;
}
// 设置当前正在操作的UI对象
void OLED_SetCurrentUI(WouoUI *ui) {
    p_cur_ui = ui;
}
// 初始化一个新的UI对象，需要传入这个UI的设置参数
//(这个还需要改进,只是初始化一个带参数的UI对象没有意义，需要带上对应的画点函数才有意义)
void OLED_NewUI(WouoUI *ui, UiPara *ui_para) {
    *ui = default_ui;    // 将默认UI的值复制到新UI
    ui->upara = ui_para; // 将参数设置为新的UI的参数
}

/**
 * @brief 页面进入初始化
 *
 * @param time 轮询时间
 */
void OLED_UILayerInProc(uint16_t time) {
    if (p_cur_ui->current_page != NULL) // 检查该页面地址存在
    {
        Page *p = (Page *)(p_cur_ui->current_page); // 把当前页面转为page读取
        if (p->enter_anim != NULL)
            p->enter_anim(p_cur_ui->current_page, time);
    }
}

/**
 * @brief 页面渐变退出动画处理
 *
 * @param time 轮询时间
 */
void OLED_UILayerOutProc(uint16_t time) {
    bool fadeout_finish = false;
    if (p_cur_ui->upara->page_fadeout) {
        p_cur_ui->pageFadeOut.timer += time;
        if (p_cur_ui->pageFadeOut.timer >= p_cur_ui->upara->ani_param[FADE_ANI] && p_cur_ui->pageFadeOut.state < 3) {
            p_cur_ui->pageFadeOut.state++;
            p_cur_ui->pageFadeOut.timer = 0;
        }
        OLED_SetPointColor(0);
        switch (p_cur_ui->pageFadeOut.state) {
        case 0:
            OLED_AllBlur(BLUR_1_4);
            break;
        case 1:
            OLED_AllBlur(BLUR_2_4);
            break;
        case 2:
            OLED_AllBlur(BLUR_3_4);
            break;
        case 3:
            OLED_AllBlur(BLUR_4_4);
            fadeout_finish = true;
            break;
        default:
            break;
        }
        OLED_SetPointColor(1);
    } else {
        fadeout_finish = true;
    }

    if (fadeout_finish) {
        p_cur_ui->current_page = p_cur_ui->pageFadeOut.in_page; // 将UI页面设置为上一级页面
        p_cur_ui->init_finish_flag = false;                     // 启用过度页面动画
        p_cur_ui->pageFadeOut.state = 0;
        p_cur_ui->pageFadeOut.timer = 0;
        p_cur_ui->state = ui_page_in;
    }
}

void OLED_UIPageProc(uint16_t time) // 页面处理任务
{
    if (p_cur_ui->current_page != NULL) // 该页面地址存在
    {
        Option *op = NULL;
        Page *p = NULL;
        TitlePage *cur_tile = NULL;
        ListPage *cur_list = NULL;
        PageType pt = OLED_CheckPageType(p_cur_ui->current_page);
        switch (pt) {
        case type_title:
            cur_tile = (TitlePage *)(p_cur_ui->current_page);
            op = &(cur_tile->option_array[cur_tile->select_item]);
            break;
        case type_list:
            cur_list = (ListPage *)(p_cur_ui->current_page);
            op = &(cur_list->option_array[cur_list->select_item]);
            break;
        default:
            break;
        }
        p = (Page *)(p_cur_ui->current_page); // 把当前页面转为page读取
        p->show(p_cur_ui->current_page, time);
        if (pt == type_list || pt == type_title) { // valwin和conwin不会同时出现
            if (p_cur_ui->slideValWin.win.state == 0 &&
                p_cur_ui->spinWin.win.state == 0 &&
                p_cur_ui->confWin.win.state == 0 &&
                p_cur_ui->infoWin.win.state == 0) {
                p->react(p_cur_ui->current_page, time);
            }
            OLED_WinFSM(&(p_cur_ui->slideValWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->spinWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->confWin.win), p_cur_ui->current_page, op, time);
            OLED_WinFSM(&(p_cur_ui->infoWin.win), p_cur_ui->current_page, op, time);
        } else
            p->react(p_cur_ui->current_page, time);
    }
}

void OLED_UIIndicatorProc(uint8_t time) {
    // indicator
    OLED_Animation(&(p_cur_ui->indicator.x), &(p_cur_ui->indicator.x_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.y), &(p_cur_ui->indicator.y_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.w), &(p_cur_ui->indicator.w_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);
    OLED_Animation(&(p_cur_ui->indicator.h), &(p_cur_ui->indicator.h_tgt), p_cur_ui->upara->ani_param[IND_ANI], time);

    PageType pageType = OLED_CheckPageType(p_cur_ui->current_page);

    // slideValWin
    if (p_cur_ui->slideValWin.win.state == 2) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, SLI_VAL_WIN_BAR_R);
        OLED_SetPointColor(1); // 实色绘制
    }
    // confWin
    else if (p_cur_ui->confWin.win.state == 2) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, CON_WIN_BTN_R);
        OLED_SetPointColor(1); // 实色绘制
    }
    // infoWin
    else if (p_cur_ui->infoWin.win.state == 2) {
        OLED_WinDrawRBoxEmpty(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                              p_cur_ui->indicator.w, p_cur_ui->indicator.h, INFO_WIN_R);
    }
    // spinWin
    else if (p_cur_ui->spinWin.win.state == 2) {
        OLED_SetPointColor(2); // 反色绘制
        OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                         p_cur_ui->indicator.w, p_cur_ui->indicator.h, 0);
        OLED_SetPointColor(1); // 实色绘制
    } else {
        switch (pageType) {
        case type_title:
            OLED_SetPointColor(2); // 反色绘制
            OLED_WinDrawBoxRightAngle(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                                      p_cur_ui->indicator.w, p_cur_ui->indicator.h, TILE_ICON_IND_R);
            OLED_SetPointColor(1); // 实色绘制
            break;
        case type_list:
            if (p_cur_ui->slideValWin.win.state == 0 &&
                p_cur_ui->confWin.win.state == 0 &&
                p_cur_ui->infoWin.win.state == 0) {
                OLED_SetPointColor(2); // 反色绘制
                OLED_WinDrawRBox(&w_all, p_cur_ui->indicator.x, p_cur_ui->indicator.y,
                                 p_cur_ui->indicator.w, p_cur_ui->indicator.h, LIST_BOX_R);
                OLED_SetPointColor(1); // 实色绘制
            }
            break;
        case type_digital:
            break;
        case type_wave:
            break;
        default:
            break;
        }
    }
}

void OLED_UIScrollBarProc(uint8_t time) {
    // Animation
    OLED_Animation(&(p_cur_ui->scrollBar.y), &(p_cur_ui->scrollBar.y_tgt), p_cur_ui->upara->ani_param[LIST_ANI], time);

    PageType pageType = OLED_CheckPageType(p_cur_ui->current_page);
    switch (pageType) {
    case type_title:
        break;
    case type_list:
        OLED_WinDrawHLine(&w_all, OLED_WIDTH - LIST_BAR_W, OLED_WIDTH, 0);
        OLED_WinDrawHLine(&w_all, OLED_WIDTH - LIST_BAR_W, OLED_WIDTH, OLED_HEIGHT - 1);
        OLED_WinDrawVLine(&w_all, OLED_WIDTH - ((LIST_BAR_W / 2) + 1), 0, OLED_HEIGHT);
        OLED_WinDrawRBox(&w_all, OLED_WIDTH - LIST_BAR_W, 0, LIST_BAR_W, p_cur_ui->scrollBar.y, 0);
        break;
    case type_digital:
        break;
    case type_wave:
        break;
    default:
        break;
    }
}

void OLED_UIProc(uint8_t time) // UI总任务(FSM)
{
    // OLED_RefreshBuff();
    switch (p_cur_ui->state) // ui状态机轮询
    {
    case ui_page_out_in:
        OLED_ClearBuff();           // 清空buff
        OLED_UIScrollBarProc(time); // 绘制进度条
        OLED_UIPageProc(time);      // 绘制页面
        OLED_UILayerOutProc(time);
        break;
    case ui_page_in: // 主要是对页面动画在切换时做一次参数的赋值
        OLED_UILayerInProc(time);
        p_cur_ui->state = ui_page_proc;
        break;
    case ui_page_proc:
        OLED_ClearBuff();           // 清空buff
        OLED_UIScrollBarProc(time); // 绘制进度条
        OLED_UIPageProc(time);      // 绘制页面
        break;
    default:
        break;
    }
    OLED_UIIndicatorProc(time); // 绘制指示器
}

/**
 * @brief 跳转到指定页面(跳转是会将当前页面id输入，用于关联确认上下级页面关系)
 *
 * @param self_page 当前页面对象的地址
 * @param terminate_page 目标页面地址
 * 从一个页面跳转到另一个页面，常用于回调函数中调用，并确定页面的上下级关系
 * 这样，在terminate_page页面收到return消息时，会返回self_page_id所代表的页面
 */
void OLED_UIJumpToPage(PageAddr self_page_addr, PageAddr terminate_page) {
    // 关联上级页面并跳转页面
    if (terminate_page != NULL) {
        Page *p_ter = (Page *)terminate_page;
        p_ter->last_page = self_page_addr;
        // 跳转页面时，如果当前页面有弹窗，将其关闭
        p_cur_ui->slideValWin.win.state = 0;
        p_cur_ui->spinWin.win.state = 0;
        p_cur_ui->confWin.win.state = 0;
        p_cur_ui->infoWin.win.state = 0;
        p_cur_ui->state = ui_page_out_in;               // 开始层级渲染
        p_cur_ui->pageFadeOut.in_page = terminate_page; // 将UI页面设置为上一级页面
    }
}

/**
 * @brief 改变当前页面(不会关联上下级页面关系，只是单纯跳转页面)
 *
 * @param terminate_page The address of the page to change to.
 */
void OLED_UIChangeCurrentPage(PageAddr terminate_page) {
    if (terminate_page != NULL) {
        // 跳转页面时，如果当前页面有弹窗，将其关闭
        p_cur_ui->slideValWin.win.state = 0;
        p_cur_ui->spinWin.win.state = 0;
        p_cur_ui->confWin.win.state = 0;
        p_cur_ui->infoWin.win.state = 0;
        p_cur_ui->state = ui_page_out_in;               // Start rendering the layers.
        p_cur_ui->pageFadeOut.in_page = terminate_page; // 将UI页面设置为上一级页面
    }
}

/**
 * @brief 得到当前页面的地址
 * @param terminate_page The address of the page to change to.
 */
Page *OLED_GetCurrentPage(void) {
    return (Page *)p_cur_ui->current_page;
}

/**
 * @brief : void OLED_Animation(float *a, float *a_tgt, uint16_t aniTime, uint16_t time)
 * @param : 非线性运动函数，n为运动步长(运动的距离占距离目标10倍的多少)
 */

/**
 * @brief 非线性运动函数
 *
 * @param a 当前值
 * @param a_tgt 目标值
 * @param aniTime 运动时间
 * @param time 轮序间隔时间
 */
void OLED_Animation(float *a, float *a_tgt, uint16_t aniTime, uint16_t time) {
    if (*a != *a_tgt) {
        if (fabs(*a - *a_tgt) < 0.5)
            *a = *a_tgt;
        else
            *a += (*a_tgt - *a) / (aniTime / time); // 慢慢接近，
    }
}
