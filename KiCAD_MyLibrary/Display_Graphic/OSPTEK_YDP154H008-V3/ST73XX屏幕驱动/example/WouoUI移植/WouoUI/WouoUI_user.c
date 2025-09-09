#include "WouoUI_user.h"
#include "WouoUI.h"
#include "math.h"
#include "string.h"

//--------定义页面对象
TitlePage main_page;
ListPage setting_page;
ListPage about_page;
RadioPage about_origin_page;
RadioPage about_wououi_page;
RadioPage about_version_page;
//--------定义每个页面需要的一些参数
// 部分页面内选项个数
#define MAIN_PAGE_NUM 3
#define SETTING_PAGE_NUM 13
#define ABOUT_PAGE_NUM 3
#define ABOUT_ORIGIN_PAGE_NUM 6
#define ABOUT_WOUOUI_PAGE_NUM 9
#define ABOUT_PAGEVERSION_NUM 8

// 定义部分页面选项
//********main页面的选项
Option mian_option_array[MAIN_PAGE_NUM] =
    {
        // 前两个字符是前缀，不显示，用于标识一般选项(-),二值选项(+),数值弹窗(~),确认弹窗($)
        //(通过选项的第一个字符判断为:无效果(-)/
        //                          滑动数值弹窗(~)/逐位调整数字弹窗(%)/提示弹窗页面(!)/确认弹窗(#)/开关选项(/)
        //                          跳转页面(+)/跳转波形页面($)/跳转多选框页面(&)/跳转单选框页面(|)
        {.text = (char *)"+ Setting"},
        {.text = (char *)"! Info", .content = (char *)"Disabling the\nfeature may\nbe unstable!"},
        {.text = (char *)"+ Aoubt"},
};
// main页面的图标
Icon main_icon_array[MAIN_PAGE_NUM] =
    {
        [0] = {0xFC, 0xFE, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x83, 0x81, 0x01, 0x01, 0x81, 0xE1, 0xE1, 0xE1,
               0xE1, 0x81, 0x01, 0x81, 0x81, 0x83, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFE, 0xFC, 0xFF, 0x01,
               0x00, 0x00, 0x00, 0xE0, 0xE0, 0xF3, 0xFF, 0xFF, 0x3F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x07, 0x07,
               0x0F, 0x3F, 0xFF, 0xFF, 0xF7, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xE0, 0x80, 0x00,
               0x00, 0x01, 0x01, 0x3B, 0x7F, 0x7F, 0x7F, 0x3C, 0x78, 0xF8, 0xF0, 0xF0, 0xF8, 0x78, 0x3C, 0x3F,
               0x7F, 0x7F, 0x33, 0x01, 0x01, 0x00, 0x00, 0x80, 0xE0, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFE, 0xFC,
               0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE1, 0xE1, 0xE1, 0xE1, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0,
               0xF0, 0xF8, 0xFC, 0xFC, 0xFF, 0xFF, 0xDF, 0xCF}, // Setting
        [1] = {0xFC, 0xFE, 0x07, 0xF3, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0x03, 0x03,
               0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xF3, 0x07, 0xFE, 0xFC, 0xFF, 0xFF,
               0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xBF, 0x3F, 0x7F, 0x20, 0x08, 0x8F, 0x07,
               0x61, 0x30, 0x36, 0x38, 0x39, 0x39, 0x39, 0x39, 0x39, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xE7,
               0xE7, 0xE7, 0xE7, 0xE7, 0xC7, 0xDB, 0xC3, 0xE1, 0xF8, 0xFC, 0x04, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xCF, 0xDF, 0xF8, 0xF3, 0xF7, 0xF7,
               0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF0, 0xF0, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7,
               0xF7, 0xF7, 0xF7, 0xF7, 0xF3, 0xF8, 0xDF, 0xCF}, // Curve
        [2] = {0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07,
               0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFF, 0xFF,
               0xFF, 0x3F, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xEE, 0xE6, 0x66, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
               0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1F, 0x1F, 0x08, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x60, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
               0xF8, 0xF8, 0xF8, 0xF8, 0xFF, 0xFF, 0xDF, 0xCF} // about
};

// 设置的列表选项数组
Option setting_option_array[SETTING_PAGE_NUM] =
    {
        {.text = (char *)"- Setting"}, // 第一个做说明标签，没有功能
        {.float_val = 123.456, .float_min = -500, .float_max = 500, .decimalNum = DecimalNum_2, .text = (char *)"% spinbox 0"},
        {.float_val = 0, .float_min = 0, .float_max = 9999, .decimalNum = DecimalNum_0, .text = (char *)"% spinbox 1"},
        {.step = 1, .text = (char *)"! Info test 1", .content = (char *)"This is a\ninfomation\ntest!"},
        {.step = 1, .text = (char *)"! Info test 2", .content = (char *)"I can eat glass\nit doesn't\nhurt me."},
        {.val = 10, .step = 10, .int_min = -100, .int_max = 149, .text = (char *)"~ slide val 0"},
        {.val = 20, .step = 5, .int_min = 0, .int_max = 100, .text = (char *)"~ slide val 1"},
        {.step = 1, .text = (char *)"# conf test 0", .content = (char *)"Sure to open\n/close the\nbuzzer?"},
        {.step = 1, .text = (char *)"# conf test 1", .content = (char *)"Sure to enable\n/disable the\nfunction?"},
        {.step = 1, .text = (char *)"& T Ufd Fm Scr"},
        {.step = 1, .text = (char *)"& T Loop Mode"},
        {.step = 1, .text = (char *)"& L Loop Mode"},
        {.step = 1, .text = (char *)"& RaderPicMode"},
};

// about页面的选项数组
Option about_option_array[ABOUT_PAGE_NUM] =
    {
        {.text = (char *)"+ WouoUI origin"}, // 原wououi的说明
        {.text = (char *)"+ About wouo"},    // 第一项只是说明
        {.text = (char *)"+ Page version"},  // Page version的说明
};

// about wououi页面的数组
Option about_origin_array[ABOUT_ORIGIN_PAGE_NUM] =
    {
        {.text = (char *)"- WouoUI origin"},
        {.val = 1, .step = 1, .text = (char *)"# test0"},
        {.val = 0, .step = 1, .text = (char *)"# test1"},
        {.step = 1, .text = (char *)"@ test2"},
        {.step = 1, .text = (char *)"@ test3"},
        {.step = 1, .text = (char *)"@ test4"},
};

Option about_wououi_array[ABOUT_WOUOUI_PAGE_NUM] =
    {
        {.text = (char *)"- From WouoUI1.2"},
        {.text = (char *)"- Thanks for the "},
        {.text = (char *)"  original author"},
        {.text = (char *)"! Bilibili UID:"},
        {.text = (char *)"@ 9182439"},
        {.text = (char *)"- Github URL: "},
        {.text = (char *)"@ https://"},
        {.text = (char *)"  github.com/"},
        {.text = (char *)"  RQNG/WouoUI"},
};

Option about_version_array[ABOUT_PAGEVERSION_NUM] =
    {
        {.text = (char *)"- Page Version"},
        {.text = (char *)"- MyBilili UID:"},
        {.text = (char *)"@ 679703519"},
        {.text = (char *)"- Github URL:"},
        {.text = (char *)"@ https://"},
        {.text = (char *)"  github.com/"},
        {.text = (char *)"  Sheep118/WouoUI"},
        {.text = (char *)"  -PageVersion"},
};
//--------定义每个页面的回调函数

void MainPage_CallBack(const Page *cur_page_addr, Option *select_item) {
    if (!strcmp(select_item->text, "+ Setting")) {
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &setting_page);
    } else if (!strcmp(select_item->text, "# Info")) {
    }
    if (!strcmp(select_item->text, "+ Aoubt")) {
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_page);
    }
}

void SettingPage_CallBack(const Page *cur_page_addr, Option *select_item) {
    // switch (select_item->order) // 对选中项的真实参数值赋值
    // {                           // 由于第0项是说明文字“Setting”
    // case 1:
    //     g_default_ui_para.ani_param[TILE_ANI] = select_item->val;
    //     break; // ani_tile
    // case 2:
    //     g_default_ui_para.ani_param[LIST_ANI] = select_item->val;
    //     break; // ani_list
    // case 3:
    //     g_default_ui_para.ufd_param[TILE_UFD] = select_item->val;
    //     break; // ani_tile
    // case 4:
    //     g_default_ui_para.ufd_param[LIST_UFD] = select_item->val;
    //     break; // ani_list
    // case 5:
    //     g_default_ui_para.loop_param[TILE_UFD] = select_item->val;
    //     break; // loop_tile
    // case 6:
    //     g_default_ui_para.loop_param[LIST_UFD] = select_item->val;
    //     break; // loop_list
    // case 7:
    //     g_default_ui_para.valwin_broken = select_item->val;
    //     break; // ValWin Broken
    // case 8:
    //     g_default_ui_para.conwin_broken = select_item->val;
    //     break; // ConWin Broken
    // case 9:
    //     g_default_ui_para.digital_ripple = select_item->val;
    //     break; // Digital Ripple Enable/not
    // case 10:
    //     g_default_ui_para.raderpic_scan_mode = select_item->val;
    //     break; // RaderPic scan mode
    // case 11:
    //     g_default_ui_para.raderpic_scan_rate = select_item->val;
    //     break; // RaderPic scan rate
    // case 12:
    //     g_default_ui_para.raderpic_move_rate = select_item->val;
    //     break; // RaderPic move rate
    // default:
    //     break;
    // }
}

void About_CallBack(const Page *cur_page_addr, Option *select_item) {
    switch (select_item->order) { // 第0项是说明文字
    case 0:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_origin_page);
        break;
    case 1:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_wououi_page);
        break;
    case 2:
        OLED_UIJumpToPage((PageAddr)cur_page_addr, &about_version_page);
        break;
    default:
        break;
    }
}

//--------------给主函数调用的接口函数
void TestUI_Init(void) {
    // OLED_Init();  //硬件的初始化
    OLED_ClearBuff();      // 清空缓存
    OLED_RefreshBuff();    // 刷新屏幕(清空屏幕)
    OLED_SetPointColor(1); // 设置绘制颜色
    // 补充列表页面的初值
    // setting_option_array[1].val = g_default_ui_para.ani_param[TILE_ANI];
    // setting_option_array[2].val = g_default_ui_para.ani_param[LIST_ANI];
    // setting_option_array[3].val = g_default_ui_para.ani_param[TILE_UFD];
    // setting_option_array[4].val = g_default_ui_para.ani_param[LIST_UFD];
    // setting_option_array[5].val = g_default_ui_para.ani_param[TILE_LOOP];
    // setting_option_array[6].val = g_default_ui_para.ani_param[LIST_LOOP];
    // setting_option_array[7].val = g_default_ui_para.valwin_broken;
    // setting_option_array[8].val = g_default_ui_para.conwin_broken;
    // setting_option_array[9].val = g_default_ui_para.digital_ripple;
    // setting_option_array[10].val = g_default_ui_para.raderpic_scan_mode;
    // setting_option_array[11].val = g_default_ui_para.raderpic_scan_rate;
    // setting_option_array[12].val = g_default_ui_para.raderpic_move_rate;

    // 设置界面选项
    OLED_TitlePageInit(&main_page, MAIN_PAGE_NUM, mian_option_array, main_icon_array, MainPage_CallBack);

    OLED_ListPageInit(&setting_page, SETTING_PAGE_NUM, setting_option_array, Setting_none, SettingPage_CallBack);
    OLED_ListPageInit(&about_page, ABOUT_PAGE_NUM, about_option_array, Setting_none, About_CallBack);

    OLED_ListPageInit(&about_origin_page, ABOUT_ORIGIN_PAGE_NUM, about_origin_array, Setting_radio, NULL);
    OLED_ListPageInit(&about_wououi_page, ABOUT_WOUOUI_PAGE_NUM, about_wououi_array, Setting_none, NULL);
    OLED_ListPageInit(&about_version_page, ABOUT_PAGEVERSION_NUM, about_version_array, Setting_none, NULL);
}

void TestUI_Proc(void) {
    OLED_UIProc(10);
}
