/*
 * File      : server.c
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

#include <rtgui/rtgui.h>
#include <topwin.h>
#include <rtgui/event.h>
#include <rtgui/widgets/panel.h>
#include <mouse.h>
#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/window.h>

extern rtgui_win_t* rtgui_server_focus_win;

void rtgui_server_handle_update(struct rtgui_event_update* event)
{
	if(hw_driver != RT_NULL)
	{
		rtgui_gdev_update(hw_driver, &(event->rect));
	}
}

//鼠标点击事件
void rtgui_server_handle_mouse_btn(struct rtgui_event_mouse* event)
{
	rtgui_win_t* win;
	rtgui_panel_t* panel=rtgui_panel_get();

	/* re-init to server thread */
	RTGUI_EVENT_MOUSE_BUTTON_INIT(event);

#ifdef RTGUI_USING_WINMOVE
	if(rtgui_winrect_is_moved())
	{
		if(event->button & (RTGUI_MOUSE_BUTTON_LEFT|RTGUI_MOUSE_BUTTON_UP))
		{
			/* update window location */
			rtgui_win_t *wid;
			rtgui_rect_t rect;
			
			if (rtgui_winrect_moved_done(&rect, &wid) == RT_TRUE)
			{
				struct rtgui_event_win_move ewin;

				/* move window */
				RTGUI_EVENT_WIN_MOVE_INIT(&ewin);
				ewin.wid = wid;
				ewin.x = rect.x1;
				ewin.y = rect.y1;
				
				/* send to client thread */
				rtgui_send(&(ewin.parent), sizeof(ewin));
				return;
			}
		}
	}
#endif

	/* the point in a win? */
	win = rtgui_topwin_get_wnd(event->x, event->y);
	if(win != RT_NULL)
	{
		rtgui_rect_t rect;

		event->wid = win;
		if(rtgui_server_focus_win != win)
		{
			/* no focused window, raise it. */
			rtgui_topwin_raise(win);
		}

		rtgui_win_get_title_rect(win, &rect);
		rtgui_widget_rect_to_device(win, &rect);
		if(rtgui_rect_contains_point(&rect, event->x, event->y) == RT_EOK)
		{
			/* in title bar */
			rtgui_topwin_title_onmouse(win, event);
		}
		else
		{
			/* in client area */
			/* send mouse event to thread */
			rtgui_send((rtgui_event_t*)event, sizeof(struct rtgui_event_mouse));
		}
		return;
	}

	/* send event to panel */
	/* deactivate old window */
	if(rtgui_server_focus_win != RT_NULL)
	{
		rtgui_topwin_deactivate(rtgui_server_focus_win);
	}
	rtgui_server_focus_win = RT_NULL;
	event->wid = RT_NULL;

	rtgui_send((rtgui_event_t*)event,sizeof(struct rtgui_event_mouse));
}

//鼠标手势事件
void rtgui_server_handle_mouse_motion(struct rtgui_event_mouse* event)
{
	rtgui_win_t* win=RT_NULL;

	RTGUI_EVENT_MOUSE_MOTION_INIT(event);

	win = rtgui_topwin_get_wnd(event->x, event->y);
	if(win != RT_NULL)
	{
#ifdef RTGUI_USING_WINMOVE
		if(!rtgui_winrect_is_moved())
#endif
		{
			event->wid = win;
			/* send mouse event to thread */
			rtgui_send((rtgui_event_t*)event, sizeof(struct rtgui_event_mouse));
		}
	}
	else
	{
#ifdef RTGUI_USING_WINMOVE
		if(!rtgui_winrect_is_moved())
#endif
		{
			/* send event to panel */
			rtgui_send((rtgui_event_t*)event,sizeof(struct rtgui_event_mouse));
		}
	}

	/* move mouse to (x, y) */
	rtgui_mouse_moveto(event->x, event->y);
}

void rtgui_server_handle_kbd(struct rtgui_event_kbd* event)
{
	rtgui_win_t *win;

	/* re-init to server thread */
	RTGUI_EVENT_KBD_INIT(event);

	win = rtgui_server_focus_win;
	if(win != RT_NULL && win->status & RTGUI_WIN_STATUS_ACTIVATE)
	{
		event->wid = win;
		rtgui_send((rtgui_event_t*)event, sizeof(struct rtgui_event_kbd));
	}
	else
	{
		/* send event to panel */
		event->wid = RT_NULL;
		rtgui_send((rtgui_event_t*)event,sizeof(struct rtgui_event_kbd));
	}
}

/**
 * rtgui server thread's entry
 */
static char _server_event_buffer[RTGUI_EVENT_SIZE];
static void rtgui_server_entry(void* parameter)
{
	/* init mouse and show */
	rtgui_mouse_init();
#ifdef RTGUI_USING_MOUSE_CURSOR
	rtgui_mouse_show_cursor();
#endif
}
void rtgui_server_todo(void)
{
	rtgui_event_t* event;
	rt_err_t result;
	event = (rtgui_event_t*)_server_event_buffer;

	/* the buffer uses to receive event */
	result = rtgui_recv(event, RTGUI_EVENT_SIZE);
	if(result == RT_EOK)
	{
		/* dispatch event */
		switch(event->type)
		{
		case RTGUI_EVENT_WIN_CREATE:
			rtgui_topwin_add(((struct rtgui_event_win*)event)->wid);
			break;

		case RTGUI_EVENT_WIN_DESTROY:
			rtgui_topwin_remove(((struct rtgui_event_win*)event)->wid);
			break;

		case RTGUI_EVENT_WIN_SHOW:
			rtgui_topwin_show(((struct rtgui_event_win*)event)->wid);
			break;

		case RTGUI_EVENT_WIN_HIDE:
			rtgui_topwin_hide(((struct rtgui_event_win*)event)->wid);
			break;

		case RTGUI_EVENT_WIN_MOVE:
			rtgui_topwin_move(((struct rtgui_event_win_move*)event)->wid,
			                     ((struct rtgui_event_win_move*)event)->x,
			                     ((struct rtgui_event_win_move*)event)->y);
			break;

		case RTGUI_EVENT_WIN_RESIZE:
			rtgui_topwin_resize(((struct rtgui_event_win_resize*)event)->wid,
			                    &(((struct rtgui_event_win_resize*)event)->rect));
			break;

		case RTGUI_EVENT_UPDATE:
			/* handle screen update */
			rtgui_server_handle_update((struct rtgui_event_update*)event);
#ifdef RTGUI_USING_MOUSE_CURSOR
			/* show cursor */
			rtgui_mouse_show_cursor();
#endif
			break;

		case RTGUI_EVENT_MOUSE_MOTION:
			rtgui_server_handle_mouse_motion((struct rtgui_event_mouse*)event);
			break;

		case RTGUI_EVENT_MOUSE_BUTTON:
			rtgui_server_handle_mouse_btn((struct rtgui_event_mouse*)event);
			break;

		case RTGUI_EVENT_KBD:
			rtgui_server_handle_kbd((struct rtgui_event_kbd*)event);
			break;

		case RTGUI_EVENT_COMMAND:
			break;
		}
	}
}

void rtgui_server_init(void)
{
	rtgui_server_entry(RT_NULL);
}
