#include "WouoUI_page.h"
#include "WouoUI.h"

//**********************通用页面处理方法
/**
 * @brief 通用的页面返回上一个页面函数
 *
 * @param page_addr 当前页面地址
 */
void OLED_PageReturn(PageAddr page_addr) {
    Page *p = (Page *)page_addr;
    p_cur_ui->state = ui_page_out_in;             // 将状态机置为页面初始化状态
    p_cur_ui->pageFadeOut.in_page = p->last_page; // 将UI页面设置为上一级页面
    // 从主页面返回
    if (p == p_cur_ui->home_page) {
        p_cur_ui->indicator.x = 0;
        p_cur_ui->indicator.y = 0;
        p_cur_ui->indicator.w = OLED_WIDTH;
        p_cur_ui->indicator.h = OLED_HEIGHT;
    }
}

/**
 * @brief
 *
 * @param page_addr 最基本的页面成员初始化，并加入到页面队列中
 * @param call_back 页面成员点击回调函数
 * @attention 没有进行页面类型初始化，因为这个由各自初始化函数执行
 */
void OLED_PageInit(PageAddr page_addr, CallBackFunc call_back) {
    Page *p = (Page *)page_addr;
    if (p_cur_ui->home_page == NULL) // 第一个页面初始化
    {
        p_cur_ui->home_page = page_addr;    // 将其初始化为主页面
        p_cur_ui->current_page = page_addr; // 同时作为当前页面
    }
    p->last_page = p_cur_ui->home_page;
    // 页面初始化时没有上一级页面,只有在页面跳转时才确定页面的上下级关系,默认将主页面(第一个初始化的页面)作为上一级页面
    p->cb = call_back;
}

//**********************每个以Page为基类的类都有对应的AnimInit、show、React方法
//--------Title相关函数
void OLED_TitlePageEnterAnim(PageAddr page_addr, uint16_t time) {
    p_cur_ui->tp_var.icon_x = 0;
    p_cur_ui->tp_var.icon_x_tgt = TILE_ICON_S;
    p_cur_ui->tp_var.icon_y = -TILE_ICON_H;
    p_cur_ui->tp_var.icon_y_tgt = TILE_ICON_U;
    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
    p_cur_ui->tp_var.bar_x_tgt = 0;
    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
    p_cur_ui->tp_var.title_y_tgt = OLED_HEIGHT - TILE_BAR_D - TILE_B_TITLE_FONT.Height;
}

void OLED_TitlePageShow(PageAddr page_addr, uint16_t time) {
    TitlePage *tp = (TitlePage *)page_addr;
    float temp = 0; // 用于存放临时的icon的x坐标
    // 计算动画参数
    OLED_Animation(&p_cur_ui->tp_var.icon_x, &p_cur_ui->tp_var.icon_x_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);   // 图标x坐标
    OLED_Animation(&p_cur_ui->tp_var.icon_y, &p_cur_ui->tp_var.icon_y_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);   // 图标y坐标
    OLED_Animation(&p_cur_ui->tp_var.bar_x, &p_cur_ui->tp_var.bar_x_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time);     // 装饰条x坐标
    OLED_Animation(&p_cur_ui->tp_var.title_y, &p_cur_ui->tp_var.title_y_tgt, p_cur_ui->upara->ani_param[TILE_ANI], time); // 文字y坐标

    // 绘制title
    my_String show_str = tp->option_array[tp->select_item].text;
    OLED_WinDrawStr(&w_all, OLED_MIDDLE_H - OLED_GetStrWidth(&(show_str[2]), TILE_B_TITLE_FONT) / 2,
                    p_cur_ui->tp_var.title_y, TILE_B_TITLE_FONT, (uint8_t *)&(show_str[2]));
    // 绘制装饰条
    OLED_WinDrawRBox(&w_all, p_cur_ui->tp_var.bar_x, OLED_HEIGHT - TILE_BAR_D - TILE_B_TITLE_FONT.Height,
                     TILE_BAR_W, TILE_BAR_H, 0);

    // 绘制图标
    if (!p_cur_ui->init_finish_flag) // 过度动画尚未完成
    {
        for (uint8_t i = 0; i < tp->item_num; i++) {
            if (p_cur_ui->upara->ufd_param[TILE_UFD])
                temp = (OLED_WIDTH - TILE_ICON_W) / 2 + i * p_cur_ui->tp_var.icon_x - TILE_ICON_S * tp->select_item;
            // 从第一个选项开始展开，最终保持选中在中间
            else
                temp = (OLED_WIDTH - TILE_ICON_W) / 2 + (i - tp->select_item) * p_cur_ui->tp_var.icon_x;
            // 保证选中的选项在中间，向两侧展开
            OLED_WinDrawBMP(&w_all, (int16_t)temp, (int16_t)(p_cur_ui->tp_var.icon_y), TILE_ICON_W, TILE_ICON_H, tp->icon_array[i], 1);
        }
        if (p_cur_ui->tp_var.icon_x == p_cur_ui->tp_var.icon_x_tgt) {
            p_cur_ui->init_finish_flag = true; // 设置过度动画已经结束
            p_cur_ui->tp_var.icon_x = p_cur_ui->tp_var.icon_x_tgt = -1 * tp->select_item * TILE_ICON_S;
        }
    } else {
        for (uint8_t i = 0; i < tp->item_num; i++) // 过度动画完成后一般选择时的切换动画
            OLED_WinDrawBMP(&w_all, (OLED_WIDTH - TILE_ICON_W) / 2 + (int16_t)(p_cur_ui->tp_var.icon_x) + i * TILE_ICON_S,
                            TILE_ICON_U, TILE_ICON_W, TILE_ICON_H, tp->icon_array[i], 1);
    }

    // indicator
    p_cur_ui->indicator.x_tgt = (OLED_WIDTH - TILE_ICON_IND_W) / 2;
    p_cur_ui->indicator.y_tgt = TILE_ICON_U - (TILE_ICON_IND_W - TILE_ICON_W) / 2;
    p_cur_ui->indicator.w_tgt = TILE_ICON_IND_W;
    p_cur_ui->indicator.h_tgt = TILE_ICON_IND_H;
}

void OLED_TitlePageReact(PageAddr page_addr, uint16_t time) {
    TitlePage *tp = (TitlePage *)page_addr;
    my_String selcet_string = NULL;
    InputMsg msg = OLED_MsgQueRead(); // 空时读出msg_none
    switch (msg) {
    case msg_add:
    case msg_sub:
        break;
    case msg_up:
        if (p_cur_ui->init_finish_flag) // 已经完成过渡动画了
        {
            if (tp->select_item > 0) { // 不是第一个选项
                tp->select_item--;
                p_cur_ui->tp_var.icon_x_tgt += TILE_ICON_S;
                p_cur_ui->indicator.x = (OLED_WIDTH - TILE_ICON_IND_W) / 2 + (TILE_ICON_IND_W - TILE_ICON_W);
                p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                p_cur_ui->tp_var.title_y = OLED_HEIGHT;
            } else {                                        // 是第一个选项
                if (p_cur_ui->upara->loop_param[TILE_LOOP]) // 开启循环的话
                {
                    tp->select_item = tp->item_num - 1;
                    p_cur_ui->tp_var.icon_x_tgt = -1 * TILE_ICON_S * (tp->item_num - 1);
                    p_cur_ui->indicator.x = (OLED_WIDTH - TILE_ICON_IND_W) / 2 - (TILE_ICON_IND_W - TILE_ICON_W);
                    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
                }
            }
        }
        break;
    case msg_down:
        if (p_cur_ui->init_finish_flag) { // 不是最后一个选项
            if (tp->select_item < (tp->item_num - 1)) {
                tp->select_item++;
                p_cur_ui->tp_var.icon_x_tgt -= TILE_ICON_S;
                p_cur_ui->indicator.x = (OLED_WIDTH - TILE_ICON_IND_W) / 2 - (TILE_ICON_IND_W - TILE_ICON_W);
                p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                p_cur_ui->tp_var.title_y = OLED_HEIGHT;
            } else { // 是最后一个选项
                if (p_cur_ui->upara->loop_param[TILE_LOOP]) {
                    tp->select_item = 0;
                    p_cur_ui->tp_var.icon_x_tgt = 0;
                    p_cur_ui->indicator.x = (OLED_WIDTH - TILE_ICON_IND_W) / 2 + (TILE_ICON_IND_W - TILE_ICON_W);
                    p_cur_ui->tp_var.bar_x = -TILE_BAR_W;
                    p_cur_ui->tp_var.title_y = OLED_HEIGHT;
                }
            }
        }
        break;
    case msg_click:
        selcet_string = tp->option_array[tp->select_item].text;
        if (selcet_string[0] == '~') { // 滑动数值弹窗的话，进入数值弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            // 当数值弹窗step设为0为只读是，进入数值弹窗前会调用一次回调，以便外界更改val值
            p_cur_ui->slideValWin.win.state = 1; // 进入弹窗初始化
            if ((tp->option_array[tp->select_item].step == 0) && (tp->page.cb != NULL))
                tp->page.cb(&(tp->page), &(tp->option_array[tp->select_item]));
        } else if (selcet_string[0] == '%') { // 微调数值弹窗
            p_cur_ui->spinWin.win.state = 1;  // 进入弹窗初始化
        } else if (selcet_string[0] == '!') {
            p_cur_ui->infoWin.win.state = 1; // 进入弹窗初始化
        } else if (selcet_string[0] == '#')
            p_cur_ui->confWin.win.state = 1; // 进入确认弹窗初始化
                                             // 确认弹窗内只有确认弹窗内的click才会触发回调
        else if (tp->page.cb != NULL)
            tp->page.cb(&(tp->page), &(tp->option_array[tp->select_item]));
        // 当前页面id和确认选中项的指针传入
        break;
    case msg_return:
        OLED_PageReturn(page_addr);
        break;
    case msg_home:
        break;
    default:
        break;
    }
}

void OLED_TitlePageInit(
    TitlePage *title_page,  // 磁贴页面对象
    uint8_t item_num,       // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    Icon *icon_array,       // 整个页面的icon数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
{
    // 缺少选项个数检查，Icon个数检查，
    title_page->page.page_type = type_title;
    OLED_PageInit((PageAddr)title_page, call_back);
    title_page->page.enter_anim = OLED_TitlePageEnterAnim;
    title_page->page.show = OLED_TitlePageShow;
    title_page->page.react = OLED_TitlePageReact; // 关联处理函数(方法)
    title_page->select_item = 0;
    title_page->item_num = item_num;
    title_page->option_array = option_array;
    title_page->icon_array = icon_array;
    for (uint8_t i = 0; i < title_page->item_num; i++)
        title_page->option_array[i].order = i; // 选项序号标号
}

//-------List页面相关函数
// 绘制列表项文字和对应的行末数字或选框
void OLED_ListDrawText_CheckBox(float start_y, Option *item) {
    // 绘制文字
    OLED_WinDrawStr(&w_all, LIST_TEXT_L_S, start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)(item->text));
    // 绘制表尾
    char float_buff[16];
    sprintf(float_buff, "%g", item->float_val);
    switch (item->text[0]) {
    case '~': // 数值弹窗的话 // 画行结尾的数值
        OLED_WinDrawStr(&w_all, OLED_WIDTH - LIST_TEXT_R_S - OLED_GetStrWidth(ui_itoa(item->val), LIST_TEXT_FONT),
                        start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)(ui_itoa(item->val)));
        break;
    case '%':
        OLED_WinDrawStr(&w_all, OLED_WIDTH - LIST_TEXT_R_S - OLED_GetStrWidth(float_buff, LIST_TEXT_FONT),
                        start_y + LIST_TEXT_U_S, LIST_TEXT_FONT, (uint8_t *)float_buff);
        break;
    case '&': // 二值选择框的话 //画结尾外框
    case '@': // 如果是单选框页面内的单选选项的话
    case '#': // 如果是确认弹窗的话
        OLED_WinDrawRBoxEmpty(&w_all, OLED_WIDTH - CHECK_BOX_R_S - CHECK_BOX_F_W,
                              start_y + CHECK_BOX_U_S, CHECK_BOX_F_W, CHECK_BOX_F_H, 2);
        if (item->val != 0) // 非0即是选中，画框内的点
            OLED_WinDrawRBox(&w_all, OLED_WIDTH - CHECK_BOX_R_S - CHECK_BOX_F_W + CHECK_BOX_D_S,
                             start_y + CHECK_BOX_U_S + CHECK_BOX_D_S, CHECK_BOX_D_W, CHECK_BOX_D_H, 1);
        break;
    default:
        break;
    }
}

void OLED_ListPageEnterAnim(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    p_cur_ui->lp_var.y = 0;
    p_cur_ui->lp_var.y_tgt = LIST_LINE_H;
    p_cur_ui->scrollBar.display = true;
}

void OLED_ListPageShow(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    static float temp = 0; // 用于临时存放列表每一项的y坐标

    // 计算动画过渡值
    OLED_Animation(&(p_cur_ui->lp_var.y), &(p_cur_ui->lp_var.y_tgt), p_cur_ui->upara->ani_param[LIST_ANI], time);

    if (p_cur_ui->lp_var.slip_flag && p_cur_ui->indicator.y == lp->ind_y_tgt)
        p_cur_ui->lp_var.slip_flag = false;
    // 该标志位是为了防止 切换动画导致的box_y移动对过度动画造成影响
    if (!p_cur_ui->init_finish_flag) // 过度动画
    {
        for (uint8_t i = 0; i < (lp->item_num); i++) {
            if (p_cur_ui->upara->ufd_param[LIST_UFD]) // 从头展开
                temp = i * (p_cur_ui->lp_var.y) - LIST_LINE_H * (lp->select_item) + lp->ind_y_tgt;
            else // 选中项展开
                temp = (i - (lp->select_item)) * (p_cur_ui->lp_var.y) + lp->ind_y_tgt;
            OLED_ListDrawText_CheckBox(temp, &(lp->option_array[i]));
        }
        if (p_cur_ui->lp_var.y == p_cur_ui->lp_var.y_tgt) // 动画抵达目标位置
        {
            p_cur_ui->init_finish_flag = true; // 过度动画完成
            p_cur_ui->lp_var.y = p_cur_ui->lp_var.y_tgt = -LIST_LINE_H * (lp->select_item) + lp->ind_y_tgt;
            // 第一个选项到光标(盒子)所在的距离确定下来
        }
    } else // 过度后，一般上下切换选项时的动画，但目标参数不在这儿设置
        for (uint8_t i = 0; i < (lp->item_num); i++) {
            temp = LIST_LINE_H * i + (p_cur_ui->lp_var.y);
            OLED_ListDrawText_CheckBox(temp, &(lp->option_array[i]));
        }

    // indicator
    p_cur_ui->indicator.x_tgt = 0;
    p_cur_ui->indicator.y_tgt = lp->ind_y_tgt;
    p_cur_ui->indicator.w_tgt = OLED_GetStrWidth(lp->option_array[lp->select_item].text, LIST_TEXT_FONT) + LIST_TEXT_L_S * 2;
    p_cur_ui->indicator.h_tgt = LIST_LINE_H;
    // scrollBar
    p_cur_ui->scrollBar.y_tgt = (int16_t)(lp->select_item * OLED_HEIGHT / (lp->item_num - 1)) + 1;
}

void OLED_ListPageReact(PageAddr page_addr, uint16_t time) {
    ListPage *lp = (ListPage *)page_addr;
    my_String selcet_string = NULL;
    int16_t select_val = 0;
    InputMsg msg = OLED_MsgQueRead();
    switch (msg) {
    case msg_add:
    case msg_sub:
        break;
    case msg_up:                                                         // 上一个
        if (p_cur_ui->init_finish_flag && !(p_cur_ui->lp_var.slip_flag)) // 初始化动画已完成
        {
            if (lp->select_item == 0) // 选中第一个的话
            {
                if (p_cur_ui->upara->loop_param[LIST_LOOP]) // 同时loop参数开的话
                {
                    p_cur_ui->lp_var.slip_flag = true;
                    lp->select_item = lp->item_num - 1; // 选中最后一个
                    if (lp->item_num > lp->line_n)      // 数目超出一页的最大数目
                    {                                   // 更改box到最底
                        lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        // 同时更改文字到最底
                        p_cur_ui->lp_var.y_tgt = OLED_HEIGHT - (lp->item_num) * LIST_LINE_H;
                    } else // 没有超出数目则是到最后一个
                        lp->ind_y_tgt = (lp->item_num - 1) * LIST_LINE_H;
                }
            } else // 没有选中第一个
            {
                lp->select_item -= 1;                                            // 选中减1
                if (lp->select_item < -((p_cur_ui->lp_var.y_tgt) / LIST_LINE_H)) // 光标盒子到页面顶了????
                {
                    if (!(OLED_HEIGHT % LIST_LINE_H))          // 上面剩余完整的行
                        p_cur_ui->lp_var.y_tgt += LIST_LINE_H; // 文字下移
                    else                                       // 上面的行不完整(LIST_LINE_H该项用于页面和行高非整除时)
                    {
                        if (lp->ind_y_tgt == OLED_HEIGHT - LIST_LINE_H * lp->line_n) { // 文字往下走一行，且光标盒子置于0处，把上面的非整行去掉
                            p_cur_ui->lp_var.y_tgt += (lp->line_n + 1) * LIST_LINE_H - OLED_HEIGHT;
                            lp->ind_y_tgt = 0;
                        } else if (lp->ind_y_tgt == LIST_LINE_H)
                            lp->ind_y_tgt = 0; // 上面整行直接移动光标盒子
                        else
                            p_cur_ui->lp_var.y_tgt += LIST_LINE_H; // 顶页整行，文字直接往下走
                    }
                } else                            // 光标盒子没到页面顶
                    lp->ind_y_tgt -= LIST_LINE_H; // 直接光标盒子往上走即可。
            }
        }
        break;
    case msg_down:                                                       // 下一个
        if (p_cur_ui->init_finish_flag && !(p_cur_ui->lp_var.slip_flag)) // 初始化动作已完成
        {
            if (lp->select_item == (lp->item_num) - 1) // 到最后一个选项了
            {
                if (p_cur_ui->upara->loop_param[LIST_LOOP]) // loop开关开
                {                                           // 全部回到顶部
                    p_cur_ui->lp_var.slip_flag = true;
                    lp->select_item = 0;
                    p_cur_ui->lp_var.y_tgt = 0;
                    lp->ind_y_tgt = 0;
                    p_cur_ui->lp_var.y_tgt = 0;
                }
            } else // 不是最后一个选项
            {
                lp->select_item += 1;
                if ((lp->select_item + 1) > ((lp->line_n) - (p_cur_ui->lp_var.y_tgt) / LIST_LINE_H)) { // 光标到页面底
                    if (!(OLED_HEIGHT % LIST_LINE_H))
                        p_cur_ui->lp_var.y_tgt -= LIST_LINE_H;
                    else // 非整行的情况
                    {
                        if (lp->ind_y_tgt == LIST_LINE_H * (lp->line_n - 1)) {
                            p_cur_ui->lp_var.y_tgt -= (lp->line_n + 1) * LIST_LINE_H - OLED_HEIGHT;
                            // 取出要向下移动的距离，对y_tgt来说就是向上移动的距离
                            lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        } else if (lp->ind_y_tgt == OLED_HEIGHT - LIST_LINE_H * 2)
                            // 这种情况什么时候会出现呢嗯?
                            lp->ind_y_tgt = OLED_HEIGHT - LIST_LINE_H;
                        else
                            p_cur_ui->lp_var.y_tgt -= LIST_LINE_H;
                    }
                } else
                    lp->ind_y_tgt += LIST_LINE_H;
            }
        }
        break;
    case msg_click:
        selcet_string = lp->option_array[lp->select_item].text;
        select_val = lp->option_array[lp->select_item].val;
        if (selcet_string[0] == '&' || selcet_string[0] == '@') { // 二值选框或者单选框

            if (lp->option_array[lp->select_item].step)
                lp->option_array[lp->select_item].val = !select_val; // 将对应值取反

            if (lp->page_setting == Setting_radio)
                p_cur_ui->lp_var.radio_click_flag = true; // 标记为1，用于OLED_RadioReact进行轮询判断单选
        }
// 为没有使能确认弹窗的宏定义时，打个补丁，$标识的确认弹窗和一般的二值选择框一样，将值取反，并调用回调
#if !UI_CONWIN_ENABLE
        if (selcet_string[0] == '#')
            if (lp->option_array[lp->select_item].step)
                lp->option_array[lp->select_item].val = !select_val; // 将对应值取反
#endif
        // 回调函数和状态机部分
        if (selcet_string[0] == '~') { // 数值弹窗的话，进入数值弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            // 当数值弹窗step设为0为只读是，进入数值弹窗前会调用一次回调，以便外界更改val值
            p_cur_ui->slideValWin.win.state = 1; // 进入弹窗初始化
            if ((lp->option_array[lp->select_item].step == 0) && (lp->page.cb != NULL))
                lp->page.cb(&(lp->page), &(lp->option_array[lp->select_item]));
        } else if (selcet_string[0] == '%') {
            p_cur_ui->spinWin.win.state = 1;  // 进入弹窗初始化
        } else if (selcet_string[0] == '!') { // 信息弹窗的话，进入信息弹窗这个动作不触发回调，但弹窗内不论点击什么都会实时触发回调
            p_cur_ui->infoWin.win.state = 1;  // 进入弹窗初始化
        } else if (selcet_string[0] == '#')
            p_cur_ui->confWin.win.state = 1; // 进入确认弹窗初始化
                                             // 确认弹窗内只有确认弹窗内的click才会触发回调
        else if (lp->page.cb != NULL)
            lp->page.cb(&(lp->page), &(lp->option_array[lp->select_item]));
        break;
    case msg_return:
        OLED_PageReturn((PageAddr)lp);
        break;
    case msg_home:
        return;
    default:
        break;
    }

    // 单选框页面
    if (lp->page_setting == Setting_radio) {
        if (p_cur_ui->lp_var.radio_click_flag) {
            for (uint8_t i = 0; i < lp->item_num; i++) {
                if (lp->option_array[i].text[0] == '@' || lp->option_array[i].text[0] == '&' || lp->option_array[i].text[0] == '#') {
                    if (i != lp->select_item)
                        lp->option_array[i].val = false;
                    else
                        lp->option_array[i].val = true; // 确保单选
                }
            }
            p_cur_ui->lp_var.radio_click_flag = false;
        }
    }
}

void OLED_ListPageInit(
    ListPage *lp,             // 列表页面对象
    uint8_t item_num,         // 选项个数，需与title数组大小，icon数组大小一致
    Option *option_array,     // 整个页面的选项数组(数组大小需与item_num一致)
    PageSetting page_setting, // 页面设置
    CallBackFunc call_back    // 回调函数，参数为确认选中项index（1-256）0表示未确认哪个选项
) {
    // 缺少选项个数检查，检查，
    lp->page.page_type = type_list;
    lp->page.enter_anim = OLED_ListPageEnterAnim;
    lp->page.show = OLED_ListPageShow;
    lp->page.react = OLED_ListPageReact;
    OLED_PageInit((PageAddr)lp, call_back);
    lp->page_setting = page_setting;
    lp->item_num = item_num;
    lp->select_item = 0;
    lp->option_array = option_array;
    lp->line_n = OLED_HEIGHT / LIST_LINE_H;
    for (uint8_t i = 0; i < lp->item_num; i++)
        lp->option_array[i].order = i; // 选项序号标号
}

// ------终端波形页面
#if PAGE_WAVE_ENABLE

void OLED_WavePageAnimInit(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    OLED_ListPageEnterAnim(wp, time);
    p_cur_ui->wt_var.p_rear = 0;
    p_cur_ui->wt_var.p_head = 0; // 重新进入页面时，会重新开始显示波形
    p_cur_ui->wt_var.text_bg_r = 0;
    p_cur_ui->wt_var.text_bg_r_tgt = WAVE_TEXT_BG_W;
}

void OLED_WavePageShow(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    int16_t temp; // 临时变量用于存放波形的点，暂时显示
    my_String temp_str = NULL;
    OLED_ListPageShow(wp, time);
    OLED_WinDrawRBoxEmpty(&w_all, WAVE_BOX_L_S, 0, WAVE_BOX_W, WAVE_BOX_H, 0);
    OLED_WinDrawRBoxEmpty(&w_all, WAVE_BOX_L_S + 1, 1, WAVE_BOX_W - 2, WAVE_BOX_H - 2, 0);

    // 数据入队列
    if (wp->option_array[wp->select_item].step == 1) // 有数据更新，才将数据入队
    {
        p_cur_ui->wt_var.p_rear++;
        p_cur_ui->wt_var.p_rear %= (WAVE_W - 1);
        p_cur_ui->wt_var.data_que[p_cur_ui->wt_var.p_rear] = wp->option_array[wp->select_item].val; // 放入数据
        wp->option_array[wp->select_item].step = 0;                                                 // 将数据更新的标志位置0
    }
    if ((p_cur_ui->wt_var.p_rear + 1) % (WAVE_W - 1) == p_cur_ui->wt_var.p_head) // 队列满了
    {
        p_cur_ui->wt_var.p_head++;
        p_cur_ui->wt_var.p_head %= (WAVE_W - 1); // 环状前进
    }
    p_cur_ui->wt_var.p_font = p_cur_ui->wt_var.p_head;                                                   // 获取起始地址
    if (p_cur_ui->indicator.y == p_cur_ui->lp_var.y_tgt && p_cur_ui->lp_var.y == p_cur_ui->lp_var.y_tgt) // 完成过度动画
    {
        for (uint8_t i = 1; i < WAVE_W - 1; i++) {
            if (p_cur_ui->wt_var.p_rear != p_cur_ui->wt_var.p_font) // 队列非空
            {
                temp = p_cur_ui->wt_var.data_que[p_cur_ui->wt_var.p_font];
                p_cur_ui->wt_var.p_font++;
                p_cur_ui->wt_var.p_font %= (WAVE_W - 1); // 出队指针前移
                // 画点或者差值画斜线函数
                OLED_WinDrawPoint(&w_all, WAVE_L + i,
                                  WAVE_BOX_H - 2 - (temp - wp->option_array[wp->select_item].int_min) * (WAVE_BOX_H - 4) / (wp->option_array[wp->select_item].int_max - wp->option_array[wp->select_item].int_min));
                // 因为0会占掉一个像素点，所以波形没有完全对称。
            } else
                break;
        }
        // 绘制文字
        temp_str = ui_itoa(wp->option_array[wp->select_item].val);
        OLED_WinDrawStr(&w_all, WAVE_TEXT_BG_L_S + (WAVE_TEXT_BG_W - OLED_GetStrWidth(temp_str, WAVE_FONT)) / 2,
                        OLED_HEIGHT - 28, WAVE_FONT, (uint8_t *)temp_str);
        // 绘制选框
        OLED_SetPointColor(2); // 反色选框
        // 注意这个动态宽度在过度动画完成后才执行，且从0到宽度，不是从中间开始的
        OLED_Animation(&p_cur_ui->wt_var.text_bg_r, &p_cur_ui->wt_var.text_bg_r_tgt, p_cur_ui->upara->ani_param[TAG_ANI], time);
        OLED_WinDrawRBox(&w_all, WAVE_TEXT_BG_L_S, OLED_HEIGHT - WAVE_TEXT_BG_H, p_cur_ui->wt_var.text_bg_r, WAVE_TEXT_BG_H, 0);
        OLED_SetPointColor(1); // 恢复实色
    }
}

void OLED_WavePageReact(PageAddr page_addr, uint16_t time) {
    WavePage *wp = (WavePage *)page_addr;
    OLED_ListPageReact(wp, time);
    if (p_cur_ui->wt_var.change_flag) {
        p_cur_ui->wt_var.p_rear = 0;
        p_cur_ui->wt_var.p_head = 0; // 切换选项时，重新开始显示波形
        p_cur_ui->wt_var.text_bg_r = 0;
        p_cur_ui->wt_var.text_bg_r_tgt = WAVE_TEXT_BG_W; // 切换选项时唤醒tag动画
        p_cur_ui->wt_var.change_flag = false;
    }
}

void OLED_WavePageInit(
    WavePage *wp,           // 波形页面对象
    uint8_t item_num,       // 选项个数，需与option_array数组大小，icon数组大小一致
    Option *option_array,   // 整个页面的选项数组(数组大小需与item_num一致)
    CallBackFunc call_back) // 回调函数，参数为 self_id 和 选中项option指针
{
    OLED_ListPageInit(wp, item_num, option_array, Setting_none, call_back); // 先初始化为list
    wp->page.page_type = type_wave;                                         // 指定为波形页面
    wp->page.enter_anim = OLED_WavePageAnimInit;
    wp->page.show = OLED_WavePageShow;
    wp->page.react = OLED_WavePageReact;
}

// 更新波形选项的显示值(后期会加上断言)，以防止输入NULL
void OLED_UIWaveUpdateVal(Option *op, int16_t val) {
    // 这里可以添加断言
    op->step = 1; // wave中的step用于指示是否更新这个波形的数值
    op->val = val;
}

#endif

#if PAGE_DIGITAL_ENABLE

/**
 * @brief 在数字标签上绘制下划线(光标)
 *
 * @param dp 数字页面指针
 * @param color 颜色值
 */
void OLED_DigitalDrawUline(DigitalPage *dp, uint8_t color) {
    // 标签的下划线偏移值不同
    uint8_t end_x = 0, start_y = 0, start_x = 0;
    if (dp->select_index == Digital_Pos_IndexLabel) {
        start_x = (OLED_WIDTH - OLED_GetStrWidth(dp->label_array[dp->select_label_index], DIGITAL_Label_FONT)) >> 1;
        end_x = start_x + OLED_GetStrWidth(dp->label_array[dp->select_label_index], DIGITAL_Label_FONT);
        start_y = p_cur_ui->dp_var.w_digtal[1].start_y + DIGITAL_Label_SIZE;
    } else if (dp->select_index < Digital_Pos_IndexLabel) // 是数字的话
    {
        start_y = p_cur_ui->dp_var.w_digtal[0].start_y + DIGITAL_NUM_SIZE + 2;
        start_x = 16 + p_cur_ui->dp_var.digital_num_xindex[(dp->select_index) * 2 + 1]; // 012->135 《*2》
        end_x = start_x + DIGITAL_NUM_SIZE;                                             // 两个字符宽
    }
    if (dp->select_index != Digital_Pos_Complete) {
        if (color == 0) {
            OLED_SetPointColor(0);                              // 更改颜色
            OLED_WinDrawHLine(&w_all, start_x, end_x, start_y); // 如果在python中，应该是套个装饰器
            OLED_SetPointColor(1);
        } else
            OLED_WinDrawHLine(&w_all, start_x, end_x, start_y);
    }
}

/*
函数：void OLED_DigitalDirChangey(DigitalPage *dp, DigitalDirect dir)
参数：dp 数字页面指针 ， dir方向枚举类型
返回值：None
说明：根据输入的方向，设置数字运动的目标值
*/
void OLED_DigitalDirChangey(DigitalPage *dp, DigitalDirect dir) {
    p_cur_ui->dp_var.num_y = 0;
    switch (dir) {
    case Digital_Direct_Increase:
        p_cur_ui->dp_var.num_y_tgt = -24;
        break;
    case Digital_Direct_Decrease:
        p_cur_ui->dp_var.num_y_tgt = 24;
        break;
    default:
        break;
    }
}
// 创建过度动画使用的string
static char *OLED_DigitalCreateText(DigitalPage *dp) {
    static char str[10] = {0};
    memset(str, 0, sizeof(str));
    sprintf(str, "%02d%c%02d%c%02d", dp->option_array[Digital_Pos_IndexLeft].val, dp->gap_char,
            dp->option_array[Digital_Pos_IndexMid].val, dp->gap_char,
            dp->option_array[Digital_Pos_IndexRight].val);
    return str;
}

/**
* @brief : void OLED_DigitalSetSingleBit(DigitalPage* dp, uint8_t index, SingleBit sbit_array[6])
            将设置的数字写入到显示在屏幕上6个单个位的数组中
* @param : dp 数字页面指针, index 下标，sbit_array 十位和个位的数组(共6个位)
*/
void OLED_DigitalSetSingleBit(DigitalPage *dp, uint8_t index, SingleBit sbit_array[6]) {
    if (index > 5)
        return;                                                                  // 防止数组越界
    if (index % 2)                                                               // 个位
        sbit_array[index].num = ((dp->option_array[index >> 1].val) % 100) / 10; // 取出十位
    else
        sbit_array[index].num = (dp->option_array[index >> 1].val) % 10; // 取出个位
}

/**
 * @brief : void OLED_DigitalSetValAnim(DigitalPage * dp, DigitalPosIndex bit, uint8_t val, DigitalDirect dir)
 * @param : dp 数字页面指针, bit 下标，val 更新的值，dir 方向枚举类型
 * @attention : 这个函数将更新的值设置在选项中，并触发一次更新动画，让show函数中按模式要求更新显示
 * @todo : 后期有一处细节可以更新，将根据输入方向，决定先更新十位还是个位，这样，可以使上下的ripple动画有区分。
 */
// 建议可以判断两个位更新时再等待运动结束
void OLED_DigitalSetValAnim(DigitalPage *dp, DigitalPosIndex bit, uint8_t val, DigitalDirect dir) {
    uint8_t last_val = dp->option_array[bit].val;
    if ((val >= dp->option_array[bit].int_min) && (val <= dp->option_array[bit].int_max) && (last_val != val)) // 更新的数据符合要求
    {
        dp->option_array[bit].val = val;
        p_cur_ui->dp_var.dir = dir;
        if ((val % 100) / 10 != (last_val % 100) / 10) // 十位发生变化
        {
            p_cur_ui->dp_var.digital_num_pos |= (1 << (bit * 2 + 1)); // 012->135
            p_cur_ui->dp_var.temp_ripple_index = (bit * 2 + 1);       // 记下这个十位
        }
        if (val % 10 != (last_val % 10)) // 个位发生变化
        {
            p_cur_ui->dp_var.digital_num_pos |= (1 << (bit * 2)); // 012->024
            p_cur_ui->dp_var.temp_ripple_index = (bit * 2);       // 记下这个个位(个位在十位后，所以最低位会被记录)
        }
    }
}

/**
 * @brief 在数字页面上绘制标签
 *
 * @param dp 数字页面指针
 * @param y 标签的纵坐标
 * @param label 标签的文本
 * @brief 为了减少代码量而专门为digital[1] 绘制标签设置的函数
 */
void OLED_DigitalDrawLabel(DigitalPage *dp, int16_t y, my_String label) {
    int16_t x = (OLED_WIDTH - OLED_GetStrWidth(label, DIGITAL_Label_FONT)) >> 1;                    // 计算起始横坐标
    OLED_WinDrawStr(&(p_cur_ui->dp_var.w_digtal[1]), x, y, DIGITAL_Label_FONT, (uint8_t *)(label)); // 绘制标签
}
void OLED_DigitalShow(PageAddr page_addr, uint16_t time) {
    DigitalPage *dp = (DigitalPage *)page_addr;
    if (!p_cur_ui->init_finish_flag) // 绘制过度动画
    {
        OLED_Animation(&(p_cur_ui->dp_var.rect_y), &(p_cur_ui->dp_var.rect_y_tgt), p_cur_ui->upara->ani_param[WIN_ANI], time);   // 使用弹窗的速度
        OLED_Animation(&(p_cur_ui->dp_var.label_y), &(p_cur_ui->dp_var.label_y_tgt), p_cur_ui->upara->ani_param[WIN_ANI], time); // 使用弹窗的速度
        OLED_WinDrawRBoxEmpty(&w_all, DIGITAL_RECT_X, p_cur_ui->dp_var.rect_y, DIGITAL_RECT_W, DIGITAL_RECT_H, DIGITAL_RECT_R);
        p_cur_ui->dp_var.w_digtal[0].start_y = p_cur_ui->dp_var.rect_y + DIGITAL_NUM_T_S;
        OLED_WinDrawStr(&(p_cur_ui->dp_var.w_digtal[0]), 0, 0, DIGITAL_NUM_FONT, (uint8_t *)OLED_DigitalCreateText(dp));
        // 这儿可以将label的移动改为label窗口的移动，再将label绘制在label窗口中
        p_cur_ui->dp_var.w_digtal[1].start_y = p_cur_ui->dp_var.label_y; // 更新label窗口的纵坐标
        OLED_DigitalDrawLabel(dp, 0, dp->label_array[dp->select_label_index]);
        if (p_cur_ui->dp_var.rect_y == p_cur_ui->dp_var.rect_y_tgt && p_cur_ui->dp_var.label_y == p_cur_ui->dp_var.label_y_tgt)
            p_cur_ui->init_finish_flag = true;
    } else // 绘制切换动画
    {
        // 绘制不需要移动的框
        OLED_WinDrawRBoxEmpty(&w_all, DIGITAL_RECT_X, DIGITAL_RECT_Y, DIGITAL_RECT_W, DIGITAL_RECT_H, DIGITAL_RECT_R);
        // 闪烁
        if (p_cur_ui->dp_var.Gap_shine_time <= ((dp->gap_shine_time) >> 1)) // 恒定为0是显示的内容
        {
            OLED_WinDrawASCII(&w_all, p_cur_ui->dp_var.digital_num_xindex[4] + 28, p_cur_ui->dp_var.w_digtal[0].start_y, DIGITAL_NUM_FONT, dp->gap_char);
            OLED_WinDrawASCII(&w_all, p_cur_ui->dp_var.digital_num_xindex[2] + 28, p_cur_ui->dp_var.w_digtal[0].start_y, DIGITAL_NUM_FONT, dp->gap_char);
        } else {
            OLED_WinDrawASCII(&w_all, p_cur_ui->dp_var.digital_num_xindex[4] + 28, p_cur_ui->dp_var.w_digtal[0].start_y, DIGITAL_NUM_FONT, ' ');
            OLED_WinDrawASCII(&w_all, p_cur_ui->dp_var.digital_num_xindex[2] + 28, p_cur_ui->dp_var.w_digtal[0].start_y, DIGITAL_NUM_FONT, ' ');
        }
        if (p_cur_ui->dp_var.Gap_shine_time == dp->gap_shine_time)
            p_cur_ui->dp_var.Gap_shine_time = 0;
        else
            p_cur_ui->dp_var.Gap_shine_time++;
        // 根据模式横杠闪烁提示
        switch (dp->mod) {
        case Digital_Mode_Edit:
            if (p_cur_ui->dp_var.Uline_shine_time <= ((dp->uline_shine_time) >> 1)) // 恒定为0是显示的内容
                OLED_DigitalDrawUline(dp, 0);
            else
                OLED_DigitalDrawUline(dp, 1);
            if (p_cur_ui->dp_var.Uline_shine_time == dp->uline_shine_time)
                p_cur_ui->dp_var.Uline_shine_time = 0;
            else
                p_cur_ui->dp_var.Uline_shine_time++;
            break;
        case Digital_Mode_Observe: // 未进入编辑模式时，没有横杠
            OLED_DigitalDrawUline(dp, 0);
            break;
        case Digital_Mode_Singlebit: // 编辑单个位时，常亮
            OLED_DigitalDrawUline(dp, 1);
            break;
        default:
            break;
        }
        // 数字上下移动
        if (p_cur_ui->dp_var.dir != Digital_Direct_None) {
            if ((p_cur_ui->dp_var.digital_num_pos & 0x3F) && !(p_cur_ui->dp_var.digital_num_pos & 0x40)) {
                p_cur_ui->dp_var.temp_dir = p_cur_ui->dp_var.dir; // 记录数字传入的方向，
                // ripple模式和一起滚动模式的区别只对数字移动有效(对标签滚动无效)
                // 排除标签是为了防止在数字运动是标签运动重复进入
                if (p_cur_ui->upara->digital_ripple) // 第一次接收到运动消息时，
                {
                    OLED_DigitalDirChangey(dp, p_cur_ui->dp_var.temp_dir);
                    p_cur_ui->dp_var.temp_num_pos = (p_cur_ui->dp_var.digital_num_pos & 0x3F) & (1 << p_cur_ui->dp_var.temp_ripple_index);
                    OLED_DigitalSetSingleBit(dp, p_cur_ui->dp_var.temp_ripple_index, p_cur_ui->dp_var.num_array); // 更新第一个需要运动的位的数据
                    (p_cur_ui->dp_var.temp_ripple_index)++;
                } else // 一起滚动的话
                {
                    p_cur_ui->dp_var.temp_num_pos = (p_cur_ui->dp_var.digital_num_pos & 0x3F); // 设置需要滚动的位
                    for (uint8_t i = 0; i < 6; i++)
                        OLED_DigitalSetSingleBit(dp, i, p_cur_ui->dp_var.num_array);
                    // 更新选中位的值
                    OLED_DigitalDirChangey(dp, p_cur_ui->dp_var.temp_dir);
                }
            } else if (p_cur_ui->dp_var.digital_num_pos & 0x40) // 如果是标签
            {
                p_cur_ui->dp_var.temp_label_dir = p_cur_ui->dp_var.dir;
            }
            p_cur_ui->dp_var.dir = Digital_Direct_None; // 保证只有接收到运动消息的第一次进来
        }
        if (p_cur_ui->dp_var.temp_dir != Digital_Direct_None && (p_cur_ui->dp_var.digital_num_pos & 0x3F))                        // 持续运动//移动的是数字
            OLED_Animation(&(p_cur_ui->dp_var.num_y), &(p_cur_ui->dp_var.num_y_tgt), p_cur_ui->upara->ani_param[DIGI_ANI], time); // 从0-24
        if (p_cur_ui->dp_var.temp_label_dir != Digital_Direct_None && (p_cur_ui->dp_var.digital_num_pos & 0x40))                  // 移动的是标签
            OLED_Animation(&(p_cur_ui->dp_var.label_y), &(p_cur_ui->dp_var.label_y_tgt), p_cur_ui->upara->ani_param[DIGI_ANI], time);
        for (uint8_t i = 0; i < DIGITAL_NUM_INDEX_MAX; i++) // 绘制每个数字win的位置
        {
            if ((p_cur_ui->dp_var.temp_num_pos & (1 << i)) && p_cur_ui->dp_var.temp_dir == Digital_Direct_Decrease) // 选中了数字且收到下的消息
            {
                OLED_WinDrawASCII(&(p_cur_ui->dp_var.w_digtal[0]), p_cur_ui->dp_var.digital_num_xindex[i], p_cur_ui->dp_var.num_y - DIGITAL_NUM_SIZE, DIGITAL_NUM_FONT, ((p_cur_ui->dp_var.num_array[i].num) % 10) + '0'); // 上一个数字
                OLED_WinDrawASCII(&(p_cur_ui->dp_var.w_digtal[0]), p_cur_ui->dp_var.digital_num_xindex[i], p_cur_ui->dp_var.num_y, DIGITAL_NUM_FONT, ((p_cur_ui->dp_var.num_array[i].num + 1) % 10) + '0');                // 这个数字
            } else if ((p_cur_ui->dp_var.temp_num_pos & (1 << i)) && p_cur_ui->dp_var.temp_dir == Digital_Direct_Increase)                                                                                                 // 选中数字且收到上的消息
            {
                OLED_WinDrawASCII(&(p_cur_ui->dp_var.w_digtal[0]), p_cur_ui->dp_var.digital_num_xindex[i], p_cur_ui->dp_var.num_y + DIGITAL_NUM_SIZE, DIGITAL_NUM_FONT, ((p_cur_ui->dp_var.num_array[i].num) % 10) + '0'); // 下一个数字
                OLED_WinDrawASCII(&(p_cur_ui->dp_var.w_digtal[0]), p_cur_ui->dp_var.digital_num_xindex[i], p_cur_ui->dp_var.num_y, DIGITAL_NUM_FONT, ((p_cur_ui->dp_var.num_array[i].num - 1) % 10) + '0');                // 这个
            } else                                                                                                                                                                                                         // 直接绘制这个数字就可以
            {
                OLED_WinDrawASCII(&(p_cur_ui->dp_var.w_digtal[0]), p_cur_ui->dp_var.digital_num_xindex[i], 0, DIGITAL_NUM_FONT, (p_cur_ui->dp_var.num_array[i].num % 10) + '0'); // 因为只有单位数字
            }
        }
        if (p_cur_ui->dp_var.num_y == p_cur_ui->dp_var.num_y_tgt && (p_cur_ui->dp_var.digital_num_pos & 0x3F)) // 单次字符运动结束时
        {
            if (p_cur_ui->upara->digital_ripple) {
                do {
                    p_cur_ui->dp_var.temp_num_pos = (p_cur_ui->dp_var.digital_num_pos & 0x3F) & (1 << p_cur_ui->dp_var.temp_ripple_index);
                    if (p_cur_ui->dp_var.temp_num_pos) {
                        OLED_DigitalDirChangey(dp, p_cur_ui->dp_var.temp_dir);                                        // 设置运动坐标
                        OLED_DigitalSetSingleBit(dp, p_cur_ui->dp_var.temp_ripple_index, p_cur_ui->dp_var.num_array); // 更新下一位数据
                        (p_cur_ui->dp_var.temp_ripple_index)++;                                                       // 准备检测下一位
                        break;
                    }
                    p_cur_ui->dp_var.temp_ripple_index++;        // 准备检测下一位
                    if (p_cur_ui->dp_var.temp_ripple_index >= 6) // 所有位到扫描完，才完全退出
                    {
                        p_cur_ui->dp_var.temp_dir = Digital_Direct_None;
                        p_cur_ui->dp_var.digital_num_pos &= 0xC0;
                        p_cur_ui->dp_var.temp_num_pos = 0; // 全部数字运动完清零
                        break;                             // 所有位检测完了break;
                    }
                } while (1); // 不需要运动继续往下检测
            } else {
                p_cur_ui->dp_var.temp_dir = Digital_Direct_None;
                p_cur_ui->dp_var.digital_num_pos &= 0xC0;
            }
        }
        // 标签上下移动
        // 绘制移动标签
        if ((p_cur_ui->dp_var.digital_num_pos & 0x40) && p_cur_ui->dp_var.temp_label_dir == Digital_Direct_Increase) {
            OLED_DigitalDrawLabel(dp, p_cur_ui->dp_var.label_y + DIGITAL_Label_SIZE, dp->label_array[dp->select_label_index]);
            OLED_DigitalDrawLabel(dp, p_cur_ui->dp_var.label_y, dp->label_array[p_cur_ui->dp_var.last_or_next_label_index]);
        } else if (((p_cur_ui->dp_var.digital_num_pos) & 0x40) && p_cur_ui->dp_var.temp_label_dir == Digital_Direct_Decrease) {
            OLED_DigitalDrawLabel(dp, p_cur_ui->dp_var.label_y - DIGITAL_Label_SIZE, dp->label_array[dp->select_label_index]);
            OLED_DigitalDrawLabel(dp, p_cur_ui->dp_var.label_y, dp->label_array[p_cur_ui->dp_var.last_or_next_label_index]);
        } else
            OLED_DigitalDrawLabel(dp, 0, dp->label_array[dp->select_label_index]);
        // 运动完成
        if (p_cur_ui->dp_var.label_y == p_cur_ui->dp_var.label_y_tgt && ((p_cur_ui->dp_var.digital_num_pos) & 0x40)) {
            p_cur_ui->dp_var.temp_label_dir = Digital_Direct_None;
            p_cur_ui->dp_var.digital_num_pos &= 0x3F;
        }
    }
}

void OLED_DigitalAnimInit(PageAddr page_addr, uint16_t time) {
    DigitalPage *dp = (DigitalPage *)page_addr;
    dp->mod = Digital_Mode_Observe;          // 默认观察模式
    dp->select_index = Digital_Pos_Complete; // 默认光标位置为完成状态(即没有选中任何位)
    for (uint8_t i = 0; i < 6; i++)
        OLED_DigitalSetSingleBit(dp, i, p_cur_ui->dp_var.num_array); // 设置显示数字
    p_cur_ui->dp_var.rect_y = -DIGITAL_RECT_Y - DIGITAL_RECT_H;
    p_cur_ui->dp_var.rect_y_tgt = DIGITAL_RECT_Y;
    p_cur_ui->dp_var.label_y = OLED_HEIGHT;
    p_cur_ui->dp_var.label_y_tgt = OLED_HEIGHT - DIGITAL_Label_SIZE - 4;
    p_cur_ui->dp_var.dir = Digital_Direct_None; // 默认进入页面没有动作
    p_cur_ui->dp_var.digital_num_pos = 0;       // 默认没有选中数字
    p_cur_ui->dp_var.temp_ripple_index = 0;     // 默认没有开始滚动
}

/**
 * @brief 设置数字页面选项和回调函数
 *
 * @param dp 数字页面指针
 * @param op 选项指针
 */
void OLED_DigitalSetOptionAndCallback(DigitalPage *dp, Option *op) {
    // 如果选择索引小于标签索引，则设置选项为选项数组中的值
    if (dp->select_index < Digital_Pos_IndexLabel)
        *op = dp->option_array[dp->select_index];
    else {
        // 设置选项的顺序和文本为标签数组中的值
        op->order = dp->select_index;
        op->text = dp->label_array[dp->select_label_index];
    }
    // 如果页面回调函数不为空，则调用回调函数并传入页面ID和选项指针
    if (dp->page.cb != NULL)
        dp->page.cb(&(dp->page), op);
}

// Digital页面仅在每一位(数字/标签)发生变化并单击click或者返回到observe模式时，才调用回调，其他情况下不会调用回调
void OLED_DigitalReact(PageAddr page_addr, uint16_t time) {
    DigitalPage *dp = (DigitalPage *)page_addr;
    InputMsg msg = OLED_MsgQueRead();
    Option output_option = {0};
    switch (dp->mod) {
    case Digital_Mode_Observe:
        if (msg == msg_up || msg == msg_click) {
            dp->mod = Digital_Mode_Edit;
            dp->select_index = Digital_Pos_IndexRight;
        } else if (msg == msg_down) {
            dp->mod = Digital_Mode_Edit;
            dp->select_index = Digital_Pos_IndexLabel;
        } else if (msg == msg_return) {
            p_cur_ui->dp_var.temp_dir = Digital_Direct_None;
            p_cur_ui->dp_var.temp_num_pos = 0; // 退出时将临时变量全部清零，防止动画运行一半退出时，在其他DigitalPage页面造成影响。
            OLED_PageReturn((PageAddr)dp);
        }
        break;

    case Digital_Mode_Edit:
        if (msg == msg_return) {
            dp->select_index = Digital_Pos_Complete;
            OLED_DigitalSetOptionAndCallback(dp, &output_option); // 要退回到观察模式时会有一次complete回调
            dp->mod = Digital_Mode_Observe;
        } else if (msg == msg_click) {
            dp->mod = Digital_Mode_Singlebit;
        } else if (msg == msg_up) {
            // dp->select_index++;
            if (dp->select_index > Digital_Pos_IndexLabel)
                dp->select_index = Digital_Pos_IndexRight;
        } else if (msg == msg_down) {
            if (dp->select_index == Digital_Pos_IndexRight)
                dp->select_index = Digital_Pos_IndexLabel;
            // else
            //     dp->select_index--;
        }
        break;

    case Digital_Mode_Singlebit:
        if (msg == msg_click)
            dp->mod = Digital_Mode_Edit;
        if (msg == msg_return) {
            dp->select_index = Digital_Pos_Complete;
            OLED_DigitalSetOptionAndCallback(dp, &output_option); // 要退回到观察模式时会有一次complete回调
            dp->mod = Digital_Mode_Observe;
        } else if (msg == msg_up) {
            if (dp->select_index < Digital_Pos_IndexLabel) // 数字的话
                OLED_DigitalSetValAnim(dp, dp->select_index,
                                       dp->option_array[dp->select_index].val + dp->option_array[dp->select_index].step, Digital_Direct_Increase);
            else // 在eidt页面做了selcet_index的上限限制，输入这边一定是index_label
            {
                OLED_DigitalPage_UpdateLabelAnimation(dp, (dp->select_label_index) + 1, Digital_Direct_Increase);
            }
        } else if (msg == msg_down) {
            if (dp->select_index < Digital_Pos_IndexLabel) // 数字的话
                OLED_DigitalSetValAnim(dp, dp->select_index,
                                       dp->option_array[dp->select_index].val - dp->option_array[dp->select_index].step, Digital_Direct_Decrease);
            else // 在eidt页面做了selcet_index的上限限制，输入这边一定是index_label
            {
                OLED_DigitalPage_UpdateLabelAnimation(dp, (dp->select_label_index) - 1, Digital_Direct_Decrease);
            }
        }
        if (msg == msg_click) // 编辑值完成后click时会调用回调，将对应选项的值输出给回调
            OLED_DigitalSetOptionAndCallback(dp, &output_option);
        break;
    default:
        break;
    }
}

void OLED_DigitalPageInit(
    DigitalPage *dp,          // 数字显示页面
    Option *option_array,     // 选项数组，必须包含3个选项，因为要显示3个数字
    uint8_t label_array_num,  // 标签数组的大小
    my_String *label_array,      // 标签数组，方便标签滚动,数组大小必须和label_array_num一致
    char gap_char,            // 间隔字符
    uint8_t gap_shine_time,   // 间隔字符闪烁时间间隔
    uint8_t uline_shine_time, // 下划线标识符闪烁时间间隔
    CallBackFunc cb           // 回调函数
) {
    dp->page.page_type = type_digital; // 赋值页面类型
    OLED_PageInit((PageAddr)dp, cb);
    // 对方法赋值
    dp->page.enter_anim = OLED_DigitalAnimInit;
    dp->page.show = OLED_DigitalShow;
    dp->page.react = OLED_DigitalReact;

    dp->option_array = option_array;
    dp->label_array_num = label_array_num;
    dp->select_label_index = 0;              // 默认选中第一个标签
    dp->select_index = Digital_Pos_Complete; // 默认没有选中任何一个数字/标签，即是选择完成状态。
    dp->label_array = label_array;
    dp->gap_char = gap_char;
    dp->gap_shine_time = gap_shine_time;
    dp->uline_shine_time = uline_shine_time;
}

/**
 * @brief 更新数字页面标签动画
 *
 * @param dp 数字页面指针
 * @param label_index 标签索引
 * @param dir 运动方向
 */
void OLED_DigitalPage_UpdateLabelAnimation(DigitalPage *dp, uint8_t label_index, DigitalDirect dir) {
    if (label_index == 255)
        label_index = dp->label_array_num - 1; // 如果标签索引为255，则设置为最后一个标签索引
    else if (label_index >= dp->label_array_num)
        label_index = 0;                               // 如果标签索引大于等于标签数组长度，则设置为第一个标签索引
    if (dp == (DigitalPage *)(p_cur_ui->current_page)) // 只有当前页面是要更新的页面时，才需要更新动画，否则直接更新值就可以
    {
        // 这里需要考虑下，给外界调用需不需要等待运动完成？？？？？？if(digital_num_pos == 0)
        p_cur_ui->dp_var.dir = dir;               // 确定方向
        p_cur_ui->dp_var.label_y = 0;             // 设置标签的初始位置
        p_cur_ui->dp_var.digital_num_pos |= 0x40; // 设置数字位置标志位
        if (dir == Digital_Direct_Increase)
            p_cur_ui->dp_var.label_y_tgt = -DIGITAL_Label_SIZE; // 如果方向为增加，则设置运动目标值为负数
        else
            p_cur_ui->dp_var.label_y_tgt = DIGITAL_Label_SIZE;              // 如果方向为减少，则设置运动目标值为正数
        p_cur_ui->dp_var.last_or_next_label_index = dp->select_label_index; // 存储上一次的值
    }
    dp->select_label_index = label_index; // 更新选中项
}

/**
 * @brief 更新数字页面的数字动画
 *
 * @param dp 数字页面指针
 * @param leftval 左侧数字值
 * @param midval 中间数字值
 * @param rightval 右侧数字值
 * @param dir 数字方向
 */
void OLED_DigitalPage_UpdateDigitalNumAnimation(DigitalPage *dp, uint8_t leftval, uint8_t midval, uint8_t rightval, DigitalDirect dir) {
    uint8_t val_array[3] = {rightval, midval, leftval};
    if (dp == (DigitalPage *)(p_cur_ui->current_page)) // 当当前页面是要更新的页面时，才触发动画更新，否则直接更新数据就可以
    {
        if ((p_cur_ui->dp_var.digital_num_pos & 0x3F) == 0) // 数字运动完成就可以更新
        // 运动结束赋值发生变化,才能从外部更新数据,(是否需要处于obeserve模式由外界自己决定，这样就可以在回调函数中使用这个函数了)
        {
            // 触发数字的动画(调用时倒着循环调用)(保证能让temp_ripplr_index记录到最低需要滚动的位)
            for (int8_t i = Digital_Pos_IndexLeft; i >= Digital_Pos_IndexRight; i--) // 从高到低更新，这样ripple才能从低到高
                if (val_array[i] != dp->option_array[i].val)
                    OLED_DigitalSetValAnim(dp, (DigitalPosIndex)i, val_array[i], dir);
        }
    } else {
        for (int8_t i = Digital_Pos_IndexLeft; i >= Digital_Pos_IndexRight; i--) // 从高到低更新，这样ripple才能从低到高
            dp->option_array[i].val = val_array[i];                              // 直接更新值没有触发动画
    }
}

#endif
