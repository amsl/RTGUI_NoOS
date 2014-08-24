#include <rtgui/rtgui.h>
#include <rtgui/dc.h>
#include <rtgui/rtgui_theme.h>

struct rtgui_theme	theme;

void rtgui_theme_init(void)
{
	theme.style = RTGUI_BORDER_UP; //ϵͳ��ʾ�����ʽ
	theme.foreground = Black;//ϵͳǰ��ɫ
	theme.background = RTGUI_RGB(225,225,225);//ϵͳ����ɫ
	theme.win_active = Blue;//�������ɫ
	theme.win_deactive = Gray;//�ǻ������ɫ
	theme.blankspace = White;
}


