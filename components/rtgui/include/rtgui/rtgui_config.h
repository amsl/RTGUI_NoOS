/*
 * File      : rtgui_config.h
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
 * 2010-02-08     Bernard      move some RTGUI options to bsp
 */

#ifndef __RTGUI_CONFIG_H__
#define __RTGUI_CONFIG_H__

/* RTGUI options */
/* native running under RT-Thread */
#ifndef RT_USING_DFS
	#undef RTGUI_USING_DFS_FILERW
	#undef RTGUI_USING_HZ_FILE
#endif

#if RTGUI_DEFAULT_FONT_SIZE == 0
#define RTGUI_DEFAULT_FONT_SIZE 16
#endif

//#define RTGUI_USING_CAST_CHECK

//#define RTGUI_USING_WINMOVE

#endif
