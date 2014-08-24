/*
 * File      : button.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-16     Bernard      first version
 */
#ifndef __RTGUI_BUTTON_H__
#define __RTGUI_BUTTON_H__

#include <rtgui/widgets/label.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rtgui_button rtgui_button_t;

DECLARE_CLASS_TYPE(button);
/** Gets the type of a button */
#define RTGUI_BUTTON_TYPE       (RTGUI_TYPE(button))//(rtgui_button_type_get())
/** Casts the object to an rtgui_button_t */
#define RTGUI_BUTTON(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_BUTTON_TYPE, rtgui_button_t))
/** Checks if the object is an rtgui_button_t */
#define RTGUI_IS_BUTTON(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_BUTTON_TYPE))

#define RTGUI_BUTTON_FLAG_PRESS     0x01
#define RTGUI_BUTTON_FLAG_SHADE		0x02 
	
/*
 * the button widget
 */
struct rtgui_button
{
	/* inherit from label */
	rtgui_label_t parent;
	/* button flag */
	rt_uint32_t flag;
	/* pressed and unpressed image */
	rtgui_image_t *image;
	/* click button event handler */
	void (*click)(pvoid wdt, rtgui_event_t *event);
};
typedef void (*rtgui_click_func_t)(pvoid wdt, rtgui_event_t *event);

rtgui_button_t* rtgui_button_create(pvoid parent,const char* text,int left,int top,int w,int h);
void rtgui_button_destroy(rtgui_button_t* btn);
void rtgui_button_ondraw(rtgui_button_t* btn);
void rtgui_button_set_image(rtgui_button_t* btn, rtgui_image_t* image);

void rtgui_button_set_click(rtgui_button_t* btn, rtgui_click_func_t func);

rt_bool_t rtgui_button_event_handler(pvoid wdt, rtgui_event_t* event);

void rtgui_button_set_text(rtgui_button_t* button, const char* text);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
