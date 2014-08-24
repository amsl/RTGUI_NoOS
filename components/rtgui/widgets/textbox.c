/*
 * File      : textbox.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 *
 */
#include <string.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/textbox.h>
#include <rtgui/widgets/combobox.h>

#include <ctype.h>

static void rtgui_textbox_draw_caret(rtgui_textbox_t *box);
static rt_bool_t rtgui_textbox_onkey(pvoid wdt, rtgui_event_t* event);
static rt_bool_t rtgui_textbox_onfocus(pvoid wdt, rtgui_event_t* event);
static rt_bool_t rtgui_textbox_onunfocus(pvoid wdt, rtgui_event_t* event);

void _rtgui_textbox_constructor(rtgui_textbox_t *box)
{
	rtgui_rect_t rect;

	RTGUI_WIDGET_FLAG(box) |= RTGUI_WIDGET_FLAG_FOCUSABLE;
	if(theme.style == RTGUI_BORDER_UP)
		rtgui_widget_set_border_style(box,RTGUI_BORDER_DOWN);
	else if(theme.style == RTGUI_BORDER_EXTRA)
		rtgui_widget_set_border_style(box,RTGUI_BORDER_SUNKEN);

	rtgui_widget_set_event_handler(box, rtgui_textbox_event_handler);
	rtgui_widget_set_onfocus(box, rtgui_textbox_onfocus);
	rtgui_widget_set_onunfocus(box, rtgui_textbox_onunfocus);
	rtgui_widget_set_onkey(box,rtgui_textbox_onkey);

	RTGUI_WIDGET_FC(box) = theme.foreground;
	RTGUI_WIDGET_BC(box) = theme.blankspace;
	/* set default text align */
	RTGUI_WIDGET_TEXTALIGN(box) = RTGUI_ALIGN_CENTER_VERTICAL;
	/* set proper of control */
	box->caret_timer = RT_NULL;
	box->caret = RT_NULL;

	box->line = box->line_begin = box->position = 0;
	box->flag = RTGUI_TEXTBOX_NONE;
	box->isedit = RT_TRUE;/* default textbox is edited */
	/* allocate default line buffer */
	box->text = RT_NULL;

	rtgui_font_get_string_rect(RTGUI_WIDGET_FONT(box), "H", &rect);
	box->font_width = RC_W(rect);
	box->on_change = RT_NULL;
	box->on_enter = RT_NULL;
}

void _rtgui_textbox_deconstructor(rtgui_textbox_t *textbox)
{
	if(textbox->text != RT_NULL)
	{
		rt_free(textbox->text);
		textbox->text = RT_NULL;
	}
	if(textbox->caret_timer != RT_NULL)
		rtgui_timer_destory(textbox->caret_timer);
	textbox->caret_timer = RT_NULL;

	if(textbox->caret != RT_NULL)
		rt_free(textbox->caret);
	textbox->caret = RT_NULL;

}

DEFINE_CLASS_TYPE(textbox, "textbox",
                  RTGUI_WIDGET_TYPE,
                  _rtgui_textbox_constructor,
                  _rtgui_textbox_deconstructor,
                  sizeof(struct rtgui_textbox));

static void rtgui_textbox_get_caret_rect(rtgui_textbox_t *box, rtgui_rect_t *rect, rt_uint16_t position)
{
	int font_h,box_h;

	RT_ASSERT(box != RT_NULL);

	rtgui_widget_get_rect(box, rect);

	font_h = FONT_H(RTGUI_WIDGET_FONT(box));
	box_h = RC_H(*rect);

	rect->x1 += position * box->font_width+2;
	rect->x2 = rect->x1+2;
	rect->y1 += (box_h-font_h)/2;
	rect->y2 = rect->y1 + font_h;
}

static void rtgui_textbox_init_caret(rtgui_textbox_t *box, rt_uint16_t position)
{
	int x, y;
	rtgui_color_t color;
	rtgui_rect_t rect;
	int ofs=0;

	RT_ASSERT(box != RT_NULL);
	if(!RTGUI_WIDGET_IS_FOCUSED(box)) return;

	rtgui_textbox_get_caret_rect(box, &box->caret_rect, position);
	rect = box->caret_rect;
	rtgui_widget_rect_to_device(box, &rect);

	if(box->caret == RT_NULL)
		box->caret = rt_malloc(RC_W(rect)*RC_H(rect)*sizeof(rtgui_color_t));

	if(box->caret_timer != RT_NULL)
		rtgui_timer_stop(box->caret_timer);

	for(x=rect.x1; x<rect.x2; x++)
	{
		for(y=rect.y1; y<rect.y2; y++)
		{
			hw_driver->ops->get_pixel(&color,x,y);

			*(box->caret+ofs) = color;
			ofs++;
		}
	}

	if(box->caret_timer != RT_NULL)
		rtgui_timer_start(box->caret_timer);
}

/* draw caret */
static void rtgui_textbox_draw_caret(rtgui_textbox_t *box)
{
	int x,y;
	rtgui_color_t color;
	rtgui_rect_t rect;
	int ofs=0;
	rtgui_dc_t *dc;

	RT_ASSERT(box != RT_NULL);
	if(box->caret == RT_NULL) return;

	dc = rtgui_dc_begin_drawing(box);
	if(dc == RT_NULL)return;

	rect = box->caret_rect;

	for(x=rect.x1; x<rect.x2; x++)
	{
		for(y=rect.y1; y<rect.y2; y++)
		{
			color = *(box->caret + ofs);
			ofs++;
			if(box->flag & RTGUI_TEXTBOX_CARET)
			{
				color = ~color;
				rtgui_dc_draw_color_point(dc, x,y, color);
			}
			else
			{
				rtgui_dc_draw_color_point(dc, x,y, color);
			}
		}
	}

	rtgui_dc_end_drawing(dc);
}

static void rtgui_textbox_timeout(rtgui_timer_t* timer, void* parameter)
{
	rtgui_textbox_t* box;

	box = RTGUI_TEXTBOX(parameter);
	if(box == RT_NULL) return;

	/* set caret flag */
	if(box->flag & RTGUI_TEXTBOX_CARET)
		box->flag &= ~RTGUI_TEXTBOX_CARET;
	else
		box->flag |= RTGUI_TEXTBOX_CARET;

	rtgui_textbox_draw_caret(box);
}

static void rtgui_textbox_onmouse(rtgui_textbox_t* box, struct rtgui_event_mouse* event)
{
	rt_size_t length;

	RT_ASSERT(box != RT_NULL);
	RT_ASSERT(event != RT_NULL);

	if(box->isedit == RT_FALSE)return;

	length = rt_strlen(box->text);

	if(event->button & RTGUI_MOUSE_BUTTON_LEFT && event->button & RTGUI_MOUSE_BUTTON_DOWN)
	{
		rt_int32_t x;
		if(!box->isedit)return;
		if(box->flag & RTGUI_TEXTBOX_MULTI)
		{
			/* multiline text */
			/* set widget focus */
			rtgui_widget_focus(box);

			box->position = 0;
			box->flag |= RTGUI_TEXTBOX_CARET;
			rtgui_textbox_draw_caret(box);
		}
		else
		{
			/* single line text */
			/* set caret position */
			x = event->x - RTGUI_WIDGET(box)->extent.x1;
			if(x < 0)
			{
				box->position = 0;
			}
			else if(x > length * box->font_width)
			{
				box->position = length;
			}
			else
			{
				box->position = x / box->font_width;
			}

			if(box->flag & RTGUI_TEXTBOX_CARET)
			{
				if(box->caret_timer != RT_NULL)
					rtgui_timer_stop(box->caret_timer);

				box->flag &= ~RTGUI_TEXTBOX_CARET;
				rtgui_textbox_draw_caret(box);

				if(box->caret_timer != RT_NULL)
					rtgui_timer_start(box->caret_timer);
			}

			/* set widget focus */
			rtgui_widget_focus(box);

			if(RTGUI_WIDGET_IS_FOCUSED(box))
			{
				rtgui_textbox_init_caret(box, box->position);
				box->flag |= RTGUI_TEXTBOX_CARET;
				rtgui_textbox_draw_caret(box);
			}
		}
	}
}

rt_bool_t rtgui_textbox_default_onChange(pvoid wdt, rtgui_event_t *event)
{

	//Add codes at here...
	return RT_TRUE;
}

static rt_bool_t rtgui_textbox_onkey(pvoid wdt, rtgui_event_t* event)
{
	rtgui_textbox_t* box = RTGUI_TEXTBOX(wdt);
	struct rtgui_event_kbd *ekbd = (struct rtgui_event_kbd*)event;
	rt_size_t length;

	RT_ASSERT(box != RT_NULL);
	RT_ASSERT(ekbd != RT_NULL);

	length = rt_strlen(box->text);
	if(ekbd->key == RTGUIK_DELETE)
	{
		/* delete latter character */
		if(box->position == length - 1)
		{
			box->text[box->position] = '\0';
		}
		else
		{
			char *c;

			/* remove character */
			for(c = &box->text[box->position]; c[1] != '\0'; c++)
				*c = c[1];
			*c = '\0';
		}

		if(box->on_change != RT_NULL)box->on_change(box,event);
	}
	else if(ekbd->key == RTGUIK_BACKSPACE)
	{
		/* delete front character */
		if(box->position == 0)
			return RT_FALSE;
		else if(box->position == length)
		{
			box->text[box->position-1] = '\0';
			box->position --;
		}
		else if(box->position != 0)
		{
			/* remove current character */
			if(box->position != 0)
			{
				char *c;

				/* remove character */
				for(c = &box->text[box->position - 1]; c[1] != '\0'; c++)
					*c = c[1];
				*c = '\0';
			}
			box->position --;
		}

		if(box->on_change != RT_NULL)box->on_change(box,event);
	}
	else if(ekbd->key == RTGUIK_LEFT)
	{
		/* move to prev */
		if(box->position > 0)
		{
			box->position --;
		}
	}
	else if(ekbd->key == RTGUIK_RIGHT)
	{
		/* move to next */
		if(box->position < length)
		{
			box->position ++;
		}
	}
	else if(ekbd->key == RTGUIK_HOME)
	{
		/* move cursor to start */
		box->position = 0;
	}
	else if(ekbd->key == RTGUIK_END)
	{
		/* move cursor to end */
		box->position = length;
	}
	else if(ekbd->key == RTGUIK_RETURN)
	{
		if(box->on_enter != RT_NULL)
		{
			box->on_enter(box, event);
		}
	}
	else if(ekbd->key == RTGUIK_NUMLOCK)
	{
		/* change numlock state */
	}
	else
	{
		if(isprint(ekbd->key))
		{
			/* it's may print character */
			/* no buffer on this line */
			if(box->flag & RTGUI_TEXTBOX_DIGIT)
			{
				/* only input digit */
				if(!isdigit(ekbd->key))
				{
					/* exception: '.' and '-' */
					if(ekbd->key != '.' && ekbd->key !='-')return RT_FALSE;
					if(ekbd->key == '.' && strchr(box->text,'.'))return RT_FALSE;

					if(ekbd->key == '-')
					{
						if(length+1 > box->line_length) return RT_FALSE;
						if(length+1 > box->dis_length) return RT_FALSE;

						if(strchr(box->text,'-'))
						{
							char* c;
							for(c = &box->text[0]; c != &box->text[length]; c++)
								*c = *(c+1);
							box->text[length] = '\0';
							box->position --;
							goto _exit;
						}
						else
						{
							char* c;
							for(c = &box->text[length]; c != &box->text[0]; c--)
								*c = *(c-1);
							box->text[0] = '-';
							box->text[length+1] = '\0';
							box->position ++;
							goto _exit;
						}
					}
				}
				//rt_kprintf("%c ",ekbd->key);//debug printf
			}
			if(length+1 > box->line_length) return RT_FALSE;
			if(length+1 > box->dis_length) return RT_FALSE;

			if(box->position <= length-1)
			{
				char* c;

				for(c = &box->text[length]; c != &box->text[box->position]; c--)
					*c = *(c-1);
				box->text[length+1] = '\0';
			}

			box->text[box->position] = ekbd->key;
			box->position ++;

			if(box->on_change != RT_NULL)box->on_change(box,event);
		}
	}

_exit:
	if(box->flag & RTGUI_TEXTBOX_CARET)
	{
		if(box->caret_timer != RT_NULL)
			rtgui_timer_stop(box->caret_timer);

		box->flag &= ~RTGUI_TEXTBOX_CARET;
		rtgui_textbox_draw_caret(box);/* refresh it */
		if(box->caret_timer != RT_NULL)
			rtgui_timer_start(box->caret_timer);
	}

	/* re-draw text box */
	rtgui_textbox_ondraw(box);

	if(RTGUI_WIDGET_IS_FOCUSED(box))
	{
		rtgui_textbox_init_caret(box, box->position);
		box->flag |= RTGUI_TEXTBOX_CARET;
		rtgui_textbox_draw_caret(box);
	}
	return RT_TRUE;
}

static rt_bool_t rtgui_textbox_onfocus(pvoid wdt, rtgui_event_t* event)
{
	rtgui_textbox_t* box = RTGUI_TEXTBOX(wdt);

	if(box->isedit == RT_TRUE)
	{
		if(box->caret_timer== RT_NULL)
		{
			box->caret_timer = rtgui_timer_create("tbox", 100, RT_TIMER_FLAG_PERIODIC,
			                                      rtgui_textbox_timeout, box);
		}
		/* set caret to show */
		box->flag |= RTGUI_TEXTBOX_CARET;
		/* start caret timer */
		if(box->caret_timer != RT_NULL)
			rtgui_timer_start(box->caret_timer);
	}
	return RT_TRUE;
}

static rt_bool_t rtgui_textbox_onunfocus(pvoid wdt, rtgui_event_t* event)
{
	rtgui_textbox_t* box = RTGUI_TEXTBOX(wdt);

	/* stop caret timer */
	if(box->caret_timer != RT_NULL)
	{
		rtgui_timer_stop(box->caret_timer);
		rtgui_timer_destory(box->caret_timer);
		box->caret_timer = RT_NULL;
	}
	/* set caret to hide */
	box->flag &= ~RTGUI_TEXTBOX_CARET;
	rtgui_textbox_draw_caret(box);

	if(box->on_enter != RT_NULL)box->on_enter(box,event);

	return RT_TRUE;
}

rtgui_textbox_t* rtgui_textbox_create(pvoid parent,const char* text,int left,int top,int w,int h, rt_uint32_t flag)
{
	rtgui_container_t *container;
	rtgui_textbox_t* box;

	RT_ASSERT(parent != RT_NULL);
	container = RTGUI_CONTAINER(parent);

	box = rtgui_widget_create(RTGUI_TEXTBOX_TYPE);
	if(box != RT_NULL)
	{
		rtgui_rect_t rect;
		rtgui_widget_get_rect(container, &rect);
		rtgui_widget_rect_to_device(container,&rect);
		rect.x1 += left;
		rect.y1 += top;
		rect.x2 = rect.x1+w;
		rect.y2 = rect.y1+h;
		rtgui_widget_set_rect(box,&rect);

		/* allocate default line buffer */
		rtgui_textbox_set_value(box, text);

		box->flag = flag;
		/* set character number */
		box->dis_length = (w-5)/FONT_W(RTGUI_WIDGET_FONT(box));

		rtgui_container_add_child(container, box);
	}

	return box;
}

void rtgui_textbox_destroy(rtgui_textbox_t* box)
{
	rtgui_widget_destroy(box);
}


void rtgui_textbox_ondraw(rtgui_textbox_t* box)
{
	/* draw button */
	rtgui_rect_t rect;
	rtgui_dc_t* dc;

	RT_ASSERT(box != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(box);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(box, &rect);
	rtgui_rect_inflate(&rect,-RTGUI_WIDGET_BORDER_SIZE(box));

	/* fill widget rect with white color */
	if(box->isedit == RT_TRUE)
	{
		RTGUI_DC_BC(dc) = RTGUI_WIDGET_BC(box);
		rtgui_dc_fill_rect(dc,&rect);
	}
	else
	{
		if(RTGUI_IS_COMBO(RTGUI_WIDGET(box)->parent))
		{
			if(RTGUI_WIDGET_IS_ENABLE(RTGUI_WIDGET(box)->parent))
				RTGUI_DC_BC(dc) = theme.blankspace;
			else
				RTGUI_DC_BC(dc) = theme.background;
			rtgui_dc_fill_rect(dc,&rect);
		}
		else
		{
			RTGUI_DC_BC(dc) = theme.background;
			rtgui_dc_fill_rect(dc,&rect);
		}
	}

	rtgui_rect_inflate(&rect,RTGUI_WIDGET_BORDER_SIZE(box));
	/* draw border */
	rtgui_dc_draw_border(dc,&rect,RTGUI_WIDGET_BORDER_STYLE(box));

	/* draw text */
	if(box->text != RT_NULL)
	{
		rect.x1 += RTGUI_MARGIN;
		if(box->flag & RTGUI_TEXTBOX_MULTI)
		{
			/* draw multiline text */
			int start,end,alllen,fh,fw,rw;
			fw  = FONT_W(RTGUI_WIDGET_FONT(box));
			fh  = FONT_H(RTGUI_WIDGET_FONT(box));
			rw  = RC_W(rect);
			alllen = rt_strlen(box->text);/* all character length */
			start=end=0;
			rect.y1 += 2;
			while(end<alllen)
			{
				/* is line feed? */
				if(*(box->text+end) == '\n')
				{
					int i, mlen;
					char* string;

					end++;
					mlen = end-start;

					/* we use rtgui_dc_draw_text(),need remake string,add '\0' at ending */
					string = rt_malloc(mlen);
					for(i=0; i<mlen; i++)
					{
						if(box->flag & RTGUI_TEXTBOX_MASK) /* ciphertext */
							string[i] = '*';
						else
							string[i] = *(box->text+start+i);
						if(i==(mlen-1))string[i] = '\0';/* add '\0' by oneself */
					}

					for(i=0; i<mlen;)
					{
						int linelen = rw/fw;
						rect.y2 = rect.y1+fh;

						if(linelen > mlen)
						{
							linelen = mlen;
							rtgui_dc_draw_text(dc, string+i, &rect);
						}
						else
						{
							/* auto-line feed */
							int k;
							char* tmpstr = rt_malloc(linelen+1);/* temporary memory */

							for(k=0; k<(linelen+1); k++)
							{
								if(box->flag & RTGUI_TEXTBOX_MASK) /* ciphertext */
									tmpstr[k] = '*';
								else						 /* plain code text */
									tmpstr[k] = *(string+i+k);
								if(k==linelen)tmpstr[k]='\0';/* add '\0' by oneself */
							}
							rtgui_dc_draw_text(dc, tmpstr, &rect);
							rt_free(tmpstr);
						}
						i += linelen;
						rect.y1 = rect.y2+1;
					}
					rt_free(string);

					start = end;/* next line start position */
				}
				end++;
			}
		}
		else
		{
			/* draw single text */
			if(box->flag & RTGUI_TEXTBOX_MASK)
			{
				/* draw '*' */
				rt_size_t len = rt_strlen(box->text);
				if(len > 0)
				{
					char *text_mask = rt_malloc(len + 1);
					rt_memset(text_mask, '*', len + 1);
					text_mask[len] = 0;
					rtgui_dc_draw_text(dc, text_mask, &rect);
					rt_free(text_mask);
				}
			}
			else
			{
				rtgui_dc_draw_text(dc, box->text, &rect);
			}
		}
	}

	rtgui_dc_end_drawing(dc);
}

/* set textbox text */
void rtgui_textbox_set_value(rtgui_textbox_t* box, const char* text)
{
	if(box->text != RT_NULL)
	{
		/* yet exist something */
		/* free the old text */
		rt_free(box->text);
		box->text = RT_NULL;
	}

	/* no something */
	box->line_length = ((rt_strlen(text)+1)/RTGUI_TEXTBOX_LINE_MAX+1)*RTGUI_TEXTBOX_LINE_MAX;

	/* allocate line buffer */
	box->text = rt_malloc(box->line_length);
	rt_memset(box->text, 0, box->line_length);

	/* copy text */
	rt_memcpy(box->text, text, rt_strlen(text) + 1);

	/* set current position */
	box->position = rt_strlen(text);
}

const char* rtgui_textbox_get_value(rtgui_textbox_t* box)
{
	return (const char*)box->text;
}

void rtgui_textbox_set_line_length(rtgui_textbox_t* box, rt_size_t length)
{
	rt_uint8_t* new_line;

	RT_ASSERT(box != RT_NULL);

	/* invalid length */
	if(length <= 0) return;

	new_line = rt_malloc(length);
	if(length < box->line_length)
	{
		rt_memcpy(new_line, box->text, length - 1);
		new_line[length] = '\0';
	}
	else
	{
		rt_memcpy(new_line, (const char*)box->text, rt_strlen((const char*)box->text));
	}

	/* set line length */
	box->line_length = length;
}

/* get textbox text area */
void rtgui_textbox_get_edit_rect(rtgui_textbox_t *box,rtgui_rect_t *rect)
{
	rtgui_widget_get_rect(box, rect);
	rtgui_rect_inflate(rect,-1);
}

rt_bool_t rtgui_textbox_event_handler(pvoid wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = RTGUI_WIDGET(wdt);
	rtgui_textbox_t* box = RTGUI_TEXTBOX(wdt);

	switch(event->type)
	{
		case RTGUI_EVENT_PAINT:
			if(widget->on_draw != RT_NULL)
				widget->on_draw(widget, event);
			else
				rtgui_textbox_ondraw(box);
			return RT_FALSE;

		case RTGUI_EVENT_MOUSE_BUTTON:
			if(widget->on_mouseclick != RT_NULL)
				widget->on_mouseclick(widget, event);
			else
				rtgui_textbox_onmouse(box, (struct rtgui_event_mouse*)event);
			return RT_TRUE;

		case RTGUI_EVENT_KBD:
			if(widget->on_key != RT_NULL)
				widget->on_key(widget, event);

			return RT_TRUE;

		default:
			//return rtgui_widget_event_handler(widget,event);
		return RT_FALSE;
	}
}

