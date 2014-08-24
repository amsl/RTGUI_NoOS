/*
 * File      : widget.h
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
#ifndef __RTGUI_WIDGET_H__
#define __RTGUI_WIDGET_H__

#include <rtgui/rtgui_object.h>
#include <rtgui/image.h>
#include <rtgui/kbddef.h>
#include <rtgui/event.h>
#include <rtgui/font.h>
#include <rtgui/rtgui_theme.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rtgui_widget rtgui_widget_t;

DECLARE_CLASS_TYPE(widget);

/** Gets the type of a widget */
#define RTGUI_WIDGET_TYPE       (RTGUI_TYPE(widget))
/** Casts the object to a rtgui_widget */
#define RTGUI_WIDGET(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_WIDGET_TYPE, rtgui_widget_t))
/** Check if the object is a rtgui_widget */
#define RTGUI_IS_WIDGET(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_WIDGET_TYPE))

/*
 * the base widget object
 */
struct rtgui_widget
{
	rtgui_object_t 		object;				/* inherit from rtgui_object_t */
	rtgui_widget_t 		*parent;			/* the parent widget */
	rtgui_widget_t		*toplevel;			/* the root widget */
	rtgui_list_t 		sibling;			/* the widget child and sibling */
	rt_uint32_t 		flag;				/* widget flag */
	rtgui_dc_t			dc;					/* hardware device context */
	rtgui_gc_t 			gc;					/* the graphic context of widget */
	rt_uint16_t			tab_index;			/* table detect order */
	rt_uint16_t			tab_stop;
	rtgui_rect_t 		extent;				/* the widget extent */
	rt_uint32_t 		align;				/* widget align */
	rt_uint16_t 		border;
	rt_uint16_t			style;
	rtgui_region_t 		clip; 				/* the rect clip */
	
	rt_bool_t (*event_handler)(pvoid wdt, rtgui_event_t* event);/* the event handler */
	
	rt_bool_t (*on_focus_in)(pvoid wdt, rtgui_event_t* event);/* call back */
	rt_bool_t (*on_focus_out)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_draw)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_mouseclick)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_key)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_size)(pvoid wdt, rtgui_event_t* event);
	rt_bool_t (*on_command)(pvoid wdt, rtgui_event_t* event);

	rt_uint32_t user_data;			/* reserved user data */
};
					
#define RTGUI_WIDGET_FC(w)				((RTGUI_WIDGET(w))->gc.fc)
#define RTGUI_WIDGET_BC(w)				((RTGUI_WIDGET(w))->gc.bc)
#define RTGUI_WIDGET_FONT(w)			((RTGUI_WIDGET(w))->gc.font)
#define RTGUI_WIDGET_TEXTALIGN(w)		((RTGUI_WIDGET(w))->gc.textalign)

#define RTGUI_WIDGET_FLAG(w)			(RTGUI_WIDGET(w))->flag
#define RTGUI_WIDGET_ALIGN(w)			(RTGUI_WIDGET(w))->align
#define RTGUI_WIDGET_EVENT_HANDLE(w)	(RTGUI_WIDGET(w))->event_handler 
#define RTGUI_WIDGET_EVENT_CALL(w,e)	(RTGUI_WIDGET(w))->event_handler(w,e) 
#define RTGUI_WIDGET_PARENT(w)			(RTGUI_WIDGET(w))->parent
#define RTGUI_WIDGET_EXTENT(w)			(RTGUI_WIDGET(w))->extent
#define RTGUI_WIDGET_BORDER_SIZE(w)			(RTGUI_WIDGET(w))->border
#define RTGUI_WIDGET_BORDER_STYLE(w)			(RTGUI_WIDGET(w))->style
#define RTGUI_WIDGET_CLIP(w)			(RTGUI_WIDGET(w))->clip
#define RTGUI_WIDGET_USER_DATA(w)		(RTGUI_WIDGET(w))->user_data

#define RTGUI_WIDGET_FLAG_DEFAULT		0x0000	
#define RTGUI_WIDGET_FLAG_HIDE			0x0001	
#define RTGUI_WIDGET_FLAG_DISABLE		0x0002	
#define RTGUI_WIDGET_FLAG_FOCUS			0x0004	
#define RTGUI_WIDGET_FLAG_TRANSPARENT	0x0008	
#define RTGUI_WIDGET_FLAG_FOCUSABLE		0x0010	
#define RTGUI_WIDGET_FLAG_DC_VISIBLE	0x0100
#define RTGUI_WIDGET_FLAG_FONT_STROKE	0x0200

#define RTGUI_WIDGET_UNHIDE(w)			RTGUI_WIDGET_FLAG(w) &= ~RTGUI_WIDGET_FLAG_HIDE
#define RTGUI_WIDGET_SHOW(w)			RTGUI_WIDGET_UNHIDE(w)
#define RTGUI_WIDGET_HIDE(w)			RTGUI_WIDGET_FLAG(w) |= RTGUI_WIDGET_FLAG_HIDE
#define RTGUI_WIDGET_IS_HIDE(w)			(RTGUI_WIDGET_FLAG(w) & RTGUI_WIDGET_FLAG_HIDE)

#define RTGUI_WIDGET_ENABLE(w)			RTGUI_WIDGET_FLAG(w) &= ~RTGUI_WIDGET_FLAG_DISABLE
#define RTGUI_WIDGET_DISABLE(w)			RTGUI_WIDGET_FLAG(w) |= RTGUI_WIDGET_FLAG_DISABLE
#define RTGUI_WIDGET_IS_ENABLE(w)		!(RTGUI_WIDGET_FLAG(w) &  RTGUI_WIDGET_FLAG_DISABLE)

#define RTGUI_WIDGET_UNFOCUS(w)			RTGUI_WIDGET_FLAG(w) &= ~RTGUI_WIDGET_FLAG_FOCUS
#define RTGUI_WIDGET_FOCUS(w)			RTGUI_WIDGET_FLAG(w) |= RTGUI_WIDGET_FLAG_FOCUS
#define RTGUI_WIDGET_IS_FOCUSED(w)		(RTGUI_WIDGET_FLAG(w) & RTGUI_WIDGET_FLAG_FOCUS)

#define RTGUI_WIDGET_IS_FOCUSABLE(w) 	(RTGUI_WIDGET_FLAG(w) & RTGUI_WIDGET_FLAG_FOCUSABLE)

#define RTGUI_WIDGET_IS_DC_VISIBLE(w)	(RTGUI_WIDGET_FLAG(w) & RTGUI_WIDGET_FLAG_DC_VISIBLE)
#define RTGUI_WIDGET_DC_SET_VISIBLE(w)	RTGUI_WIDGET_FLAG(w) |= RTGUI_WIDGET_FLAG_DC_VISIBLE 
#define RTGUI_WIDGET_DC_SET_UNVISIBLE(w) RTGUI_WIDGET_FLAG(w) &= ~RTGUI_WIDGET_FLAG_DC_VISIBLE
#define RTGUI_WIDGET_DC(w)				(&(RTGUI_WIDGET(w)->dc))

pvoid rtgui_widget_create(rtgui_type_t *widget_type);
void rtgui_widget_destroy(pvoid wdt);

/* set the event handler of widget */
void rtgui_widget_set_event_handler(pvoid wdt, rtgui_event_handler_ptr handler);
/* widget default event handler */
rt_bool_t rtgui_widget_event_handler(pvoid wdt, rtgui_event_t* event);

/* focus and unfocus */
pvoid rtgui_widget_get_focus(pvoid wdt);
void rtgui_widget_focus(pvoid wdt);
void rtgui_widget_unfocus(pvoid wdt);

/* event handler for each command */
void rtgui_widget_set_onfocus(pvoid wdt, rtgui_event_handler_ptr handler);
void rtgui_widget_set_onunfocus(pvoid wdt, rtgui_event_handler_ptr handler);

void rtgui_widget_set_ondraw(pvoid wdt, rtgui_event_handler_ptr handler);
void rtgui_widget_set_onclick(pvoid wdt, rtgui_event_handler_ptr handler);
void rtgui_widget_set_onkey(pvoid wdt, rtgui_event_handler_ptr handler);
void rtgui_widget_set_onsize(pvoid wdt, rtgui_event_handler_ptr handler);
void rtgui_widget_set_oncommand(pvoid wdt, rtgui_event_handler_ptr handler);

void rtgui_widget_get_rect(pvoid wdt, rtgui_rect_t *rect);/* get and set rect of widget */
void rtgui_widget_set_rect(pvoid wdt, rtgui_rect_t* rect);
void rtgui_widget_get_position(pvoid wdt, rtgui_point_t *p);
rt_uint16_t rtgui_widget_get_width(pvoid wdt);
rt_uint16_t rtgui_widget_get_height(pvoid wdt);

void rtgui_widget_set_border_style(pvoid wdt, rt_uint32_t style);
rt_uint16_t rtgui_widget_get_border(pvoid wdt);
rt_bool_t rtgui_widget_contains_point(pvoid wdt, int x, int y);
void rtgui_widget_point_to_device(pvoid wdt, rtgui_point_t *point);/* get the physical position of a logic point on widget */
void rtgui_widget_rect_to_device(pvoid wdt, rtgui_rect_t *rect);/* get the physical position of a logic rect on widget */
void rtgui_widget_point_to_logic(pvoid wdt, rtgui_point_t *point);/* get the logic position of a physical point on widget */
void rtgui_widget_rect_to_logic(pvoid wdt, rtgui_rect_t *rect);/* get the logic position of a physical rect on widget */
void rtgui_widget_move_to_logic(pvoid wdt, int dx, int dy);/* move widget and its child to a logic point */

/* update the clip info of widget */
void rtgui_widget_update_clip(pvoid wdt);
void rtgui_widget_update_clip_pirate(pvoid wdt,pvoid topwdt);

pvoid rtgui_widget_get_toplevel(pvoid wdt);	/* get the toplevel widget of widget */
void rtgui_widget_show(pvoid wdt);
void rtgui_widget_hide(pvoid wdt);
void rtgui_widget_update(pvoid wdt);
/* get parent color */
rtgui_color_t rtgui_widget_get_parent_fc(pvoid wdt);
rtgui_color_t rtgui_widget_get_parent_bc(pvoid wdt);
/* get the next sibling of widget */
rtgui_widget_t* rtgui_widget_get_next_sibling(pvoid wdt);
/* get the prev sibling of widget */
rtgui_widget_t* rtgui_widget_get_prev_sibling(pvoid wdt);

#ifdef __cplusplus
}
#endif

#endif
