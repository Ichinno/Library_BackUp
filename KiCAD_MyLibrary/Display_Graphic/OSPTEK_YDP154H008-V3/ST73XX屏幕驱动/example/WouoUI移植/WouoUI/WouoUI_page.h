#ifndef __WOUOUI_PAGE__
#define __WOUOUI_PAGE__

#ifdef __cplusplus
extern "C" {
#endif

#include "WouoUI_conf.h"
#include "WouoUI_oled.h"

//===================================各个页面参数==================================

// 磁贴默认参数,所有磁贴页面都使用同一套参数
#define TILE_B_TITLE_FONT Font_12_24 // 磁贴大标题字体
#define TILE_ICON_W 30               // 磁贴图标宽度
#define TILE_ICON_H 30               // 磁贴图标高度
#define TILE_ICON_IND_W 36           // 磁贴指示器宽度
#define TILE_ICON_IND_H 36           // 磁贴指示器高度
#define TILE_ICON_IND_R 5            // 磁贴指示器边长
#define TILE_ICON_S 36               // 磁贴图标间距
#define TILE_ICON_U 4                // 磁贴图标上边距
#define TILE_BAR_H 24                // 磁贴装饰条高度
#define TILE_BAR_W 8                 // 磁贴装饰条宽度
#define TILE_BAR_U 38                // 磁贴装饰条上边距
#define TILE_BAR_D 2                 // 磁贴装饰条下边距
#define ICON_BUFFSIZE (TILE_ICON_W * (TILE_ICON_H / 8 + 1))

// 列表默认参数,所有列表使用同一套参数
#define LIST_TEXT_FONT Font_6_8 // 列表每行文字字体
#define LIST_LINE_H 13          // 列表单行高度
#define LIST_TEXT_U_S 3         // 列表每行文字的上边距
#define LIST_TEXT_L_S 3         // 列表每行文字的左边距
#define LIST_TEXT_R_S 6         // 列表每行结尾的数值的右边距（包括进度条宽度 LIST_BAR_W）
#define LIST_BAR_W 3            // 列表进度条宽度，需要是奇数，因为正中间有1像素宽度的线
#define LIST_BOX_R 2            // 列表选择框倒角

// 列表行尾选择选项参数
#define CHECK_BOX_U_S 2 // 选择框在每行的上边距
#define CHECK_BOX_R_S 6 // 选择框的右边距（包括进度条宽度 LIST_BAR_W）
#define CHECK_BOX_F_W 9 // 选择框外框宽度
#define CHECK_BOX_F_H 9 // 选择框外框高度
#define CHECK_BOX_D_W 5 // 选择框里面的点宽度
#define CHECK_BOX_D_H 5 // 选择框里面的点宽度
#define CHECK_BOX_D_S 2 // 选择框里面的点距离外框的边距

// 曲线相关参数
#define WAVE_SAMPLE 1   // 采集倍数
#define WAVE_W 94       // 波形宽度
#define WAVE_L 24       // 波形左边距
#define WAVE_U 0        // 波形上边距
#define WAVE_MAX 27     // 最大值
#define WAVE_MIN 4      // 最小值
#define WAVE_BOX_W 94   // 波形边框宽度
#define WAVE_BOX_H 32   // 波形边框高度
#define WAVE_BOX_L_S 24 // 波形边框左边距

// 列表和文字背景框相关参数
#define WAVE_FONT_H 24       // 电压数字字体
#define WAVE_FONT Font_12_24 // 电压数字
#define WAVE_TEXT_BG_L_S 24  // 文字背景框左边距
#define WAVE_TEXT_BG_W 94    // 文字背景框宽度
#define WAVE_TEXT_BG_H 29

// 数字控件页面相关参数
#define DIGITAL_RECT_X 8             // 外框的横坐标
#define DIGITAL_RECT_Y 4             // 外框的纵坐标
#define DIGITAL_RECT_W 112           // 外框的宽度
#define DIGITAL_RECT_H 40            // 外框的高度
#define DIGITAL_RECT_R 3             // 外框的倒角
#define DIGITAL_NUM_T_S 8            // 数字到外框的边距
#define DIGITAL_NUM_SIZE 24          // 数字的尺寸
#define DIGITAL_NUM_FONT Font_12_24  // 数字的尺寸
#define DIGITAL_Label_SIZE 16        // 数字的尺寸
#define DIGITAL_Label_FONT Font_8_16 // 数字的尺寸
#define DIGITAL_NUM_INDEX_MAX 6      // digital页面共有6个数字

// 检查页面类型，返回PageType类型
#define OLED_CheckPageType(page_addr) (((Page *)page_addr)->page_type)

// 定义字符串类型
typedef char *my_String;

/*============================================页面类型=========================================*/
//--------页面类型枚举
typedef enum {
    type_title = 0x00, // 磁贴类
    type_list,         // 列表类
                       // 终端类页面
    type_digital,      // 数字控件类
    type_wave,         // 波形显示类
} PageType;            // 页面类型，用于标志传入的每个页面类型，方便调用对应的proc函数

typedef enum {
    Setting_none = 0x00, // 页面无设置
    Setting_radio,       // 设置为单选页面
} PageSetting;           // 页面设置

typedef enum {
    DecimalNum_0 = 0x00,
    DecimalNum_1 = 0x01,
    DecimalNum_2 = 0x02,
    DecimalNum_3 = 0x03,
} DecimalNum;

//-------页面成员类型
typedef struct Option {
    uint8_t order;   // 该选项在列表/磁贴中的排序(0-255)
    int32_t val;     // 这个列表项关联的显示的值(可以用于设置初值)
    int16_t step;    // 列表项对应变量的步长变化，只对数值弹窗有效(若是单选/多选框，该值无意义，可为0)
    int16_t int_min; // 列表项对应变量可取的最小值(若是单选/多选框，该值无意义，可为0)
    int16_t int_max; // 列表项对应变量可取的最大值(若是单选/多选框，该值无意义，可为0)
    // 浮点值，专用于微调弹窗控件
    float float_val;       // 当前值
    float float_min;       // 最小值
    float float_max;       // 最大值
    bool bit_sel;          // 位数有无选中
    // uint8_t digit_sel;     // 选中的位数
    DecimalNum decimalNum; // 小数位数

    my_String text;
    // 这个列表项显示的字符串,通过选项的第一个字符判断为:无效果(-)/
    //                          弹窗类:滑动数值弹窗(~)/逐位调整数字弹窗(%)/提示弹窗(!)/确认弹窗(#)
    //                          列表内:多选框(&)/单选框(@)
    //                          跳转:跳转页面(+)/跳转波形页面($)/跳转文本页面({)
    my_String content;
} Option; // 通用选项类,其中item_max,item_min,entity与列表选项(单选/多选/弹窗项)相关，磁贴选项可不管
// 选择类界面的回调函数类型定义,参数为确认选中项的指针。
typedef struct _page Page; // 页面基类的声明,方便作为回调函数参数
// 更改回调函数直接传入当前页面的页面地址
typedef void (*CallBackFunc)(const Page *cur_page_addr, Option *select_item);
typedef const uint8_t Icon[ICON_BUFFSIZE]; // 定义图标类型
// 页面地址类型，用于声明全局页面数组存放页面使用
#define PageAddr void *
// 每个页面都有的三种方法
typedef void (*PageEnterAnim)(PageAddr, uint16_t); // 页面的动画初始化函数
typedef void (*PageShow)(PageAddr, uint16_t);      // 页面的展示函数
typedef void (*PageReact)(PageAddr, uint16_t);     // 页面的响应函数

struct _page {
    PageType page_type;       // 页面类型，以便在处理时调用不同函数绘制
    PageAddr last_page;       // 父页面的地址
    CallBackFunc cb;          // 页面的回调函数
    PageEnterAnim enter_anim; // 进入页面动画初始化方法
    PageShow show;
    PageReact react;
}; // 最基本的页面类型(所有页面类型的基类和结构体的**第一个成员**)

//----------5种页面类
typedef struct TitlePage {
    Page page;            // 基础页面信息
    uint8_t item_num;     // 页面选项个数，option_array和icon_array个数需与此一致
    Option *option_array; // 选项类型的数组(由于数组大小不确定，使用指针代替)
    Icon *icon_array;     // 图标数组(由于数组大小不确定，使用指针代替)
    uint8_t select_item;  // 选中选项
} TitlePage;              // 磁帖页面类型(所有类型页面，类型成员为第一个，方便查看)

typedef struct ListPage {
    Page page;                // 基础页面信息
    PageSetting page_setting; // 页面设置
    uint8_t item_num;         // 页面选项个数，title和icon个数需与此一致
    uint8_t select_item;      // 选中选项
    Option *option_array;     // 选项类型的数组(由于数组大小不确定，使用指针代替)
    uint8_t line_n;           // = DISP_H / LIST_LINE_H; 屏幕内有多少行选
    float ind_y_tgt;          // 存储指示器y目标坐标
} ListPage;                   // 列表页面类型(所有类型页面，类型成员为第一个，方便查看)

#if PAGE_WAVE_ENABLE
typedef ListPage WavePage; // 定义波形页面类型
#endif

#if PAGE_RADIO_ENABLE
typedef ListPage RadioPage; // 定义单选项页面类型
#endif

#if PAGE_DIGITAL_ENABLE

typedef enum {
    Digital_Direct_None = 0x00,
    Digital_Direct_Increase = 0x01,
    Digital_Direct_Decrease = 0x02,
} DigitalDirect; // 用于Digital Show和React函数间传递信号

typedef enum {
    Digital_Mode_Observe = 0x00,   // 观察模式没有光标
    Digital_Mode_Edit = 0x01,      // 对编辑位置的编辑
    Digital_Mode_Singlebit = 0x02, // 对单位数字的编辑
} DigitalMode;                     // digital页面的模式

typedef enum // Digital页面从右往左
{
    Digital_Pos_IndexRight = 0x00, // 用于指示当前光标或者编辑的位置
    Digital_Pos_IndexMid,
    Digital_Pos_IndexLeft,
    Digital_Pos_IndexLabel, // 在标签处
    Digital_Pos_Complete,   // 编辑完成
} DigitalPosIndex;          // 用于在回调函数中检验选中项的op->order值,表示选中哪个数字位还是标签

typedef struct
{
    Page page;                        // 页面的基本信息
    char gap_char;                    // 3个digital数字间的间隔字符
    uint8_t gap_shine_time;           // 间隔字符闪烁的时间参数
    uint8_t uline_shine_time;         // 下划线闪烁的时间参数
    Option *option_array;             // 选项数组，数组大小必须为3 ，用于显示三个数字
    uint8_t label_array_num;          // 标签数组的大小
    my_String *label_array;              // 标签数组，方便标签滚动
    uint8_t select_label_index;       // 在标签数组中选中的标签的index
    DigitalPosIndex select_index : 4; // 选中项的index
    DigitalMode mod : 2;              // digital页面的当前模式
} DigitalPage;                        // 时间页面

#endif

typedef struct PageBlur {
    uint8_t state;
    uint16_t timer;
} PageBlur;

/*============================================页面共用变量=========================================*/
// Listpage 需要的共用变量集合
struct TitlePageVar {  // 因为Title选中项一直在中间，所以不需要有每个页面变量记录它真实页面的偏移位置
    float icon_x;      // 图标的x坐标距选中目标的间距的变量
    float icon_x_tgt;  // 图标的x坐标距选中目标的间距的变量目标
    float icon_y;      // 图标的y坐标
    float icon_y_tgt;  // 图标的y坐标目标
    float bar_x;       // 指示器的x坐标
    float bar_x_tgt;   // 指示器的x坐标目标值
    float title_y;     // 标题的y坐标
    float title_y_tgt; // 标题的y坐标目标值
};

// Listpage 需要的共用变量集合
struct ListPageVar {
    uint8_t slip_flag;            // 切换动画是否完成的标志位
    uint8_t radio_click_flag : 1; // 标志位，用于标记在单选项页面内是否单击了Click
    float y;                      // 列表中每个选项的间隔值(过度动画后作为正真页面的起始y坐标值，以屏幕原点为0)
    float y_tgt;                  // 列表中每个选项间隔的目标值
};

#if PAGE_WAVE_ENABLE
// 波形曲线标签全局变量，用于记录波形页面中标签的移动
struct WaveTag {
    int16_t data_que[WAVE_W - 1]; // 用于记录波形全程队列
    uint8_t p_head;               // 队列头部指针，表征队列最起始的位置
    uint8_t p_rear;               // 出队列指针
    uint8_t p_font;               // 入队列指针
    uint8_t change_flag;          // 切换过的标志
    float text_bg_r;              // 文本框动态宽度
    float text_bg_r_tgt;
};
#endif

#if PAGE_DIGITAL_ENABLE
// 只有单个位的数据,用于存储单个位的数据
typedef struct _SigleBit {
    uint8_t num : 4;
} SingleBit;
// Digital页面变量集合
struct DigitalPageVar {
    uint8_t digital_num_pos;                           // 0表示没有选中位置，1-7bit每一位置1表示该位被选中
    uint8_t temp_ripple_index;                         // 用于在ripple时记录选中位置(选中最低位位置开始，慢慢往上增)
    DigitalDirect dir : 2;                             // 用于Digital Show和React函数间传递信号
    window w_digtal[2];                                // 用限制数字时钟的窗口(1个数字窗口+1个label窗口) 2个窗口
    float rect_y;                                      // 移动外框的顶点坐标
    float rect_y_tgt;                                  // 移动外框的顶点坐标目标
    float label_y;                                     // 移动外框的顶点坐标
    float label_y_tgt;                                 // 移动外框的顶点坐标目标
    float num_y;                                       // 移动数字的顶点坐标
    float num_y_tgt;                                   // 移动数字的顶点坐标目标
    uint8_t digital_num_xindex[DIGITAL_NUM_INDEX_MAX]; // 6个单位数字的横坐标存储数组
    uint8_t Gap_shine_time;                            // 全局闪烁时间递减变量
    uint8_t Uline_shine_time;                          // 全局闪烁时间递减变量
    SingleBit num_array[6];                            // 显示的6个单个位数据
    uint8_t last_or_next_label_index;                  // 用来记录上次显示的label的index
    uint8_t temp_num_pos;                              // 用于在show函数内暂时装载num_pos变量，实现同时运动动画和ripple动画的切换
    DigitalDirect temp_dir;                            // 记录下运动方向
    DigitalDirect temp_label_dir;                      // 记录下label运动方向，防止数字和label同时需要运动的情况出现
};
#endif

typedef enum UIState {
    ui_page_out_in = 0x00, // ui层级退出时
    ui_page_in,            // ui层级深入时
    ui_page_proc,          // ui页面处理时
} UIState;                 // UI状态机

void OLED_PageReturn(PageAddr page_addr);
void OLED_PageInit(PageAddr page_addr, CallBackFunc call_back);
//-------Title页面相关函数
void OLED_TitlePageEnterAnim(PageAddr page_addr, uint16_t time);
void OLED_TitlePageShow(PageAddr page_addr, uint16_t time);
void OLED_TitlePageReact(PageAddr page_addr, uint16_t time);
void OLED_TitlePageInit(TitlePage *title_page, uint8_t item_num, Option *option_array, Icon *icon_array, CallBackFunc call_back);
//-------List页面相关函数
void OLED_ListDrawText_CheckBox(float start_y, Option *item);
void OLED_ListPageEnterAnim(PageAddr page_addr, uint16_t time);
void OLED_ListPageShow(PageAddr page_addr, uint16_t time);
void OLED_ListPageReact(PageAddr page_addr, uint16_t time);
void OLED_ListPageInit(ListPage *lp, uint8_t item_num, Option *option_array, PageSetting page_setting, CallBackFunc call_back);
// ------终端波形页面
void OLED_WavePageInit(WavePage *wp, uint8_t item_num, Option *option_array, CallBackFunc call_back);
void OLED_UIWaveUpdateVal(Option *op, int16_t val);
// -------单选框页面
void OLED_RadioPageReact(PageAddr page_addr, uint16_t time);
// void OLED_RadioPageInit(RadioPage *rp, uint8_t item_num, Option *option_array, CallBackFunc call_back);
// -------数字页面
void OLED_DigitalPageInit(DigitalPage *dp, Option *option_array, uint8_t label_array_num, my_String *label_array, char gap_char, uint8_t gap_shine_time, uint8_t uline_shine_time, CallBackFunc cb);
void OLED_DigitalPage_UpdateDigitalNumAnimation(DigitalPage *dp, uint8_t leftval, uint8_t midval, uint8_t rightval, DigitalDirect dir);
void OLED_DigitalPage_UpdateLabelAnimation(DigitalPage *dp, uint8_t label_index, DigitalDirect dir);

#ifdef __cplusplus
}
#endif

#endif
