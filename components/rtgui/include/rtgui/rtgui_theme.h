#ifndef __RTGUI_THEME_H__
#define __RTGUI_THEME_H__

#include <rtgui/color.h>

struct rtgui_theme
{
	rt_uint8_t	style;
	rtgui_color_t foreground;	//ϵͳǰ��ɫ
	rtgui_color_t background;	//ϵͳ����ɫ
	rtgui_color_t win_active;	//�������ɫ
	rtgui_color_t win_deactive; //�ǻ������ɫ
	rtgui_color_t blankspace;   //�հ�����ɫ,��༭��,�б��ĵ�ɫ
};

void rtgui_theme_init(void);

extern struct rtgui_theme	theme;



#endif

