/*
 * File      : rtgui.h
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 */
#ifndef __RT_GUI_H__
#define __RT_GUI_H__

#include <rtthread.h>
#include <rtgui/rtgui_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RT_INT16_MAX		32767
#define RT_INT16_MIN		(-RT_INT16_MAX-1)

#define RTGUI_SEL_H			20

#define RT_COUNT(array)	(sizeof(array)/sizeof(array[0]))

typedef rt_uint16_t             rtgui_color_t;
typedef struct rtgui_point      rtgui_point_t;
typedef struct rtgui_rect       rtgui_rect_t;
typedef struct rtgui_event      rtgui_event_t;
typedef struct rtgui_dc         rtgui_dc_t;
typedef struct rtgui_gc         rtgui_gc_t;

typedef void*                   pvoid;
typedef struct rtgui_type       rtgui_type_t;
typedef struct rtgui_object     rtgui_object_t;
typedef struct rtgui_widget     rtgui_widget_t;
typedef struct rtgui_panel      rtgui_panel_t;
typedef struct rtgui_container  rtgui_container_t;
typedef struct rtgui_win        rtgui_win_t;
typedef struct rtgui_staticline rtgui_staticline_t;
typedef struct rtgui_label      rtgui_label_t;
typedef struct rtgui_textbox    rtgui_textbox_t;
typedef struct rtgui_button     rtgui_button_t;
typedef struct rtgui_view       rtgui_view_t;
typedef struct rtgui_checkbox   rtgui_checkbox_t;
typedef struct rtgui_radiobox   rtgui_radiobox_t;
typedef struct rtgui_listbox    rtgui_listbox_t;
typedef struct rtgui_listview   rtgui_listview_t;
typedef struct rtgui_fileview   rtgui_fileview_t;
typedef struct rtgui_scrollbar  rtgui_scrollbar_t;
typedef struct rtgui_iconbox    rtgui_iconbox_t;
typedef struct rtgui_menu_item  rtgui_menu_item_t;
typedef struct rtgui_menu       rtgui_menu_t;
typedef struct rtgui_combo      rtgui_combo_t;
typedef struct rtgui_rttab      rtgui_rttab_t;;
typedef struct rtgui_grid 		rtgui_grid_t;
typedef struct rtgui_terminal 	rtgui_terminal_t;

typedef rt_bool_t (*rtgui_event_handler_ptr)(pvoid wdt, rtgui_event_t* event);

struct rtgui_point
{
	rt_int16_t x, y;
};

struct rtgui_rect
{
	rt_int16_t x1, y1, x2, y2;
};

extern rtgui_point_t rtgui_empty_point;

#define  rtgui_rect_width(r)	((r).x2 - (r).x1)
#define  rtgui_rect_height(r)	((r).y2 - (r).y1)
#define  RC_W(r)	rtgui_rect_width(r)
#define  RC_H(r)	rtgui_rect_height(r)

#define rtgui_rect_set(r, xa, ya, xb, yb) \
	(r).x1 = (xa); \
	(r).y1 = (ya); \
	(r).x2 = (xb); \
	(r).y2 = (yb);

struct rtgui_gc
{
	/* foreground and background color */
	rtgui_color_t fc, bc;
	/* text style */
	rt_uint16_t textstyle;
	/* text align */
	rt_uint16_t textalign;
	/* font */
	struct rtgui_font* font;
};

enum RTGUI_BORDER_STYLE
{
	RTGUI_BORDER_NONE = 0,
	RTGUI_BORDER_SIMPLE,
	RTGUI_BORDER_RAISE,
	RTGUI_BORDER_SUNKEN,
	RTGUI_BORDER_BOX,
	RTGUI_BORDER_STATIC,
	RTGUI_BORDER_EXTRA,
	RTGUI_BORDER_UP,
	RTGUI_BORDER_DOWN,
};

#define RTGUI_BORDER_DEFAULT_WIDTH	2
#define RTGUI_WIDGET_DEFAULT_MARGIN	3
#define RTGUI_BORDER				RTGUI_BORDER_DEFAULT_WIDTH
#define RTGUI_MARGIN				RTGUI_WIDGET_DEFAULT_MARGIN

enum RTGUI_ORIENTATION
{
    RTGUI_HORIZONTAL		= 0x01,
	RTGUI_VERTICAL			= 0x02,
	RTGUI_ORIENTATION_BOTH	= RTGUI_HORIZONTAL | RTGUI_VERTICAL
};

enum RTGUI_ALIGN
{
	RTGUI_ALIGN_NOT					= 0x00,
	RTGUI_ALIGN_CENTER_HORIZONTAL	= 0x01,
	RTGUI_ALIGN_LEFT				= RTGUI_ALIGN_NOT,
	RTGUI_ALIGN_TOP					= RTGUI_ALIGN_NOT,
	RTGUI_ALIGN_RIGHT				= 0x02,
	RTGUI_ALIGN_BOTTOM				= 0x04,
	RTGUI_ALIGN_CENTER_VERTICAL		= 0x08,
	RTGUI_ALIGN_CENTER				= RTGUI_ALIGN_CENTER_HORIZONTAL | RTGUI_ALIGN_CENTER_VERTICAL,
	RTGUI_ALIGN_EXPAND				= 0x10,
	RTGUI_ALIGN_STRETCH				= 0x20,
};

enum RTGUI_TEXTSTYLE
{
	RTGUI_TEXTSTYLE_NORMAL 			= 0x00,
	RTGUI_TEXTSTYLE_DRAW_BACKGROUND = 0x01,
	RTGUI_TEXTSTYLE_SHADOW 			= 0x02,
	RTGUI_TEXTSTYLE_OUTLINE 		= 0x04,
};

enum RTGUI_MODAL_CODE
{
	RTGUI_MODAL_OK,
	RTGUI_MODAL_CANCEL
};
typedef enum RTGUI_MODAL_CODE rtgui_modal_code_t;

#ifdef __cplusplus
}
#endif

#endif
