/*
 * File      : color.h
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
#ifndef __RTGUI_COLOR_H__
#define __RTGUI_COLOR_H__

//#include <stdlib.h>
#include <rtgui/rtgui.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RGB_EXCHANGE_16

#define RTGUI_ARGB(r, g, b, a)	\
		((rt_uint32_t)(((rt_uint8_t)(r)|\
		(((unsigned)(rt_uint8_t)(g))<<8))|\
		(((unsigned long)(rt_uint8_t)(b))<<16)|\
		(((unsigned long)(rt_uint8_t)(a))<<24)))

#define RTGUI_RGB(r, g, b) \
		((((r) >> 3) << 11)|\
		(((g) >> 2) << 5) |\
		((b)>> 3))
//#define RTGUI_RGB(r, g, b)	RTGUI_ARGB(255, (r), (g), (b))

#define RAND_COLOR 		RTGUI_RGB(rand()%255, rand()%255, rand()%255)

#define RTGUI_RGB_R(c)	((c) & 0xff)
#define RTGUI_RGB_G(c)	(((c) >> 8)  & 0xff)
#define RTGUI_RGB_B(c)	(((c) >> 16) & 0xff)
#define RTGUI_RGB_A(c)	(((c) >> 24) & 0xff)

/*
 * RTGUI default color format
 * BBBBB GGGGGG RRRRR
 */

/* convert rtgui 32 to 16 (RGB=5:6:5) */
rt_inline rt_uint16_t rtgui_color_to_565(rt_uint32_t c)
{
	rt_uint16_t pixel;

#ifdef RGB_EXCHANGE_16
	pixel = (rt_uint16_t)(((RTGUI_RGB_B(c)>> 3) << 11) | ((RTGUI_RGB_G(c) >> 2) << 5) | (RTGUI_RGB_R(c) >> 3));
#else
	pixel = (rt_uint16_t)(((RTGUI_RGB_R(c) >> 3) << 11) | ((RTGUI_RGB_G(c) >> 2) << 5) | (RTGUI_RGB_B(c)>> 3));
#endif

	return pixel;
}

rt_inline rt_uint32_t rtgui_color_from_565(rt_uint16_t pixel)
{
	rt_uint8_t r, g, b;
	rt_uint32_t color;
	
	r = (pixel >> 11) & 0x1f;
	g = (pixel >> 5)  & 0x3f;
	b = pixel & 0x1f;
	
#ifdef RGB_EXCHANGE_16
 	color = b * 255 / 31 + ((g * 255 / 63) << 8) + ((r * 255 / 31) << 16);
#else
	color = r * 255 / 31 + ((g * 255 / 63) << 8) + ((b * 255 / 31) << 16);
#endif

	return color;
}

rt_inline rt_uint16_t rgb_exchange_16(rt_uint16_t c)
{
	rt_uint16_t color;
	color =  c&0x07E0;
	color += c<<11;
	color += c>>11;
	return color;
}

enum
{
	LightPink				= RTGUI_RGB(255,182,193),//ǳ��ɫ
	Pink					= RTGUI_RGB(255,192,203),//�ۺ�
	Crimson					= RTGUI_RGB(220,20, 60 ),//�ɺ�
	LavenderBlush			= RTGUI_RGB(255,240,245),//����ĵ���ɫ
	PaleVioletRed			= RTGUI_RGB(219,112,147),//�԰׵���������ɫ
	HotPink					= RTGUI_RGB(255,105,180),//����ķۺ�
	DeepPink				= RTGUI_RGB(255,20, 147),//���ɫ
	MediumVioletRed			= RTGUI_RGB(199,21, 133),//���е���������ɫ
	Orchid					= RTGUI_RGB(218,112,214),//��������ɫ
	Thistle					= RTGUI_RGB(216,191,216),//��
	plum					= RTGUI_RGB(221,160,221),//����
	Violet					= RTGUI_RGB(238,130,238),//������
	Magenta					= RTGUI_RGB(255,0,  255),//���
	Fuchsia					= RTGUI_RGB(255,0,  255),//��������(�Ϻ�ɫ)
	DarkMagenta				= RTGUI_RGB(139,0,  139),//�����ɫ
	Purple					= RTGUI_RGB(128,0,  128),//��ɫ
	MediumOrchid			= RTGUI_RGB(186,85, 211),//���е�������
	DarkVoilet				= RTGUI_RGB(148,0,  211),//��������ɫ
	DarkOrchid				= RTGUI_RGB(153,50, 204),//��������
	Indigo					= RTGUI_RGB(75, 0,  130),//����
	BlueViolet				= RTGUI_RGB(138,43, 226),//������������ɫ
	MediumPurple			= RTGUI_RGB(147,112,219),//���е���ɫ
	MediumSlateBlue			= RTGUI_RGB(123,104,238),//���еİ��Ұ�����ɫ
	SlateBlue				= RTGUI_RGB(106,90, 205),//���Ұ�����ɫ
	DarkSlateBlue			= RTGUI_RGB(72, 61, 139),//���Ұ�����ɫ
	Lavender				= RTGUI_RGB(230,230,250),//޹�²ݻ��ĵ���ɫ
	GhostWhite				= RTGUI_RGB(248,248,255),//����İ�ɫ
	Blue					= RTGUI_RGB(0,  0,  255),//����
	MediumBlue				= RTGUI_RGB(0,  0,  205),//���е���ɫ
	MidnightBlue			= RTGUI_RGB(25, 25, 112),//��ҹ����ɫ
	DarkBlue				= RTGUI_RGB(0,  0,  139),//����ɫ
	Navy					= RTGUI_RGB(0,  0,  128),//������
	RoyalBlue				= RTGUI_RGB(65, 105,225),//�ʾ���
	CornflowerBlue			= RTGUI_RGB(100,149,237),//ʸ���յ���ɫ
	LightSteelBlue			= RTGUI_RGB(176,196,222),//������
	LightSlateGray			= RTGUI_RGB(119,136,153),//ǳʯ���
	SlateGray				= RTGUI_RGB(112,128,144),//ʯ���
	DoderBlue				= RTGUI_RGB(30, 144,255),//������
	AliceBlue				= RTGUI_RGB(240,248,255),//����˿��
	SteelBlue				= RTGUI_RGB(70, 130,180),//����
	LightSkyBlue			= RTGUI_RGB(135,206,250),//����ɫ
	SkyBlue					= RTGUI_RGB(90,130,235),//����ɫ
	DeepSkyBlue				= RTGUI_RGB(0,  191,255),//������
	LightBLue				= RTGUI_RGB(173,216,230),//����
	PowDerBlue				= RTGUI_RGB(176,224,230),//��ҩ��
	CadetBlue				= RTGUI_RGB(95, 158,160),//��У��
	Azure					= RTGUI_RGB(240,255,255),//ε��ɫ
	LightCyan				= RTGUI_RGB(225,255,255),//����ɫ
	PaleTurquoise			= RTGUI_RGB(175,238,238),//�԰׵��̱�ʯ
	Cyan					= RTGUI_RGB(0,  255,255),//��ɫ
	Aqua					= RTGUI_RGB(0,  255,255),//ˮ��ɫ
	DarkTurquoise			= RTGUI_RGB(0,  206,209),//���̱�ʯ
	DarkSlateGray			= RTGUI_RGB(47, 79, 79 ),//��ʯ���
	DarkCyan				= RTGUI_RGB(0,  139,139),//����ɫ
	Teal					= RTGUI_RGB(0,  128,128),//ˮѼɫ
	MediumTurquoise			= RTGUI_RGB(72, 209,204),//���е��̱�ʯ
	LightSeaGreen			= RTGUI_RGB(32, 178,170),//ǳ������
	Turquoise				= RTGUI_RGB(64, 224,208),//�̱�ʯ
	Auqamarin				= RTGUI_RGB(127,255,170),//����\����ɫ
	MediumAquamarine		= RTGUI_RGB(0,  250,154),//���еı���ɫ
	MediumSpringGreen		= RTGUI_RGB(245,255,250),//���еĴ������ɫ
	MintCream				= RTGUI_RGB(0,  255,127),//��������
	SpringGreen				= RTGUI_RGB(60, 179,113),//�������ɫ
	SeaGreen				= RTGUI_RGB(46, 139,87 ),//������
	Honeydew				= RTGUI_RGB(240,255,240),//����
	LightGreen				= RTGUI_RGB(144,238,144),//����ɫ
	PaleGreen				= RTGUI_RGB(152,251,152),//�԰׵���ɫ
	DarkSeaGreen			= RTGUI_RGB(143,188,143),//�����
	LimeGreen				= RTGUI_RGB(50, 205,50 ),//�����
	Lime					= RTGUI_RGB(0,  255,0  ),//���ɫ
	ForestGreen				= RTGUI_RGB(34, 139,34 ),//ɭ����
	Green					= RTGUI_RGB(0,  128,0  ),//����
	DarkGreen				= RTGUI_RGB(0,  100,0  ),//����ɫ
	Chartreuse				= RTGUI_RGB(127,255,0  ),//���ؾ���
	LawnGreen				= RTGUI_RGB(124,252,0  ),//��ƺ��
	GreenYellow				= RTGUI_RGB(173,255,47 ),//�̻�ɫ
	OliveDrab				= RTGUI_RGB(85, 107,47 ),//�������ɫ
	Beige					= RTGUI_RGB(107,142,35 ),//��ɫ(ǳ��ɫ)
	LightGoldenrodYellow	= RTGUI_RGB(250,250,210),//ǳ�������
	Ivory					= RTGUI_RGB(255,255,240),//����
	LightYellow				= RTGUI_RGB(255,255,224),//ǳ��ɫ
	Yellow					= RTGUI_RGB(255,255,0  ),//����
	Olive					= RTGUI_RGB(128,128,0  ),//���
	DarkKhaki				= RTGUI_RGB(189,183,107),//��䲼
	LemonChiffon			= RTGUI_RGB(255,250,205),//���ʱ�ɴ
	PaleGodenrod			= RTGUI_RGB(238,232,170),//��������
	Khaki					= RTGUI_RGB(240,230,140),//���䲼
	Gold					= RTGUI_RGB(255,215,0  ),//��
	Cornislk				= RTGUI_RGB(255,248,220),//����ɫ
	GoldEnrod				= RTGUI_RGB(218,165,32 ),//������
	FloralWhite				= RTGUI_RGB(255,250,240),//���İ�ɫ
	OldLace					= RTGUI_RGB(253,245,230),//���δ�
	Wheat					= RTGUI_RGB(245,222,179),//С��ɫ
	Moccasin				= RTGUI_RGB(255,228,181),//¹ƤЬ
	Orange					= RTGUI_RGB(255,165,0  ),//��ɫ
	PapayaWhip				= RTGUI_RGB(255,239,213),//��ľ��
	BlanchedAlmond			= RTGUI_RGB(255,235,205),//Ư�׵�����
	NavajoWhite				= RTGUI_RGB(255,222,173),//
	AntiqueWhite			= RTGUI_RGB(250,235,215),//�Ŵ��İ�ɫ
	Tan						= RTGUI_RGB(210,180,140),//ɹ��
	BrulyWood				= RTGUI_RGB(222,184,135),//��ʵ����
	Bisque					= RTGUI_RGB(255,228,196),//(Ũ��)��֬,���ѵ�
	DarkOrange				= RTGUI_RGB(255,140,0  ),//���ɫ
	Linen					= RTGUI_RGB(250,240,230),//���鲼
	Peru					= RTGUI_RGB(205,133,63 ),//��³
	PeachPuff				= RTGUI_RGB(255,218,185),//��ɫ
	SandyBrown				= RTGUI_RGB(244,164,96 ),//ɳ��ɫ
	Chocolate				= RTGUI_RGB(210,105,30 ),//�ɿ���
	SaddleBrown				= RTGUI_RGB(139,69, 19),//����ɫ
	SeaShell				= RTGUI_RGB(255,245,238),//������
	Sienna					= RTGUI_RGB(160,82, 45 ),//������ɫ
	LightSalmon				= RTGUI_RGB(255,160,122),//ǳ����(����)ɫ
	Coral					= RTGUI_RGB(255,127,80 ),//ɺ��
	OrangeRed				= RTGUI_RGB(255,69, 0  ),//�Ⱥ�ɫ
	DarkSalmon				= RTGUI_RGB(233,150,122),//������(����)ɫ
	Tomato					= RTGUI_RGB(255,99, 71 ),//����
	MistyRose				= RTGUI_RGB(255,228,225),//����õ��
	Salmon					= RTGUI_RGB(250,128,114),//����(����)ɫ
	Snow					= RTGUI_RGB(255,250,250),//ѩ
	LightCoral				= RTGUI_RGB(240,128,128),//��ɺ��ɫ
	RosyBrown				= RTGUI_RGB(188,143,143),//õ����ɫ
	IndianRed				= RTGUI_RGB(205,92, 92 ),//ӡ�Ⱥ�
	Red						= RTGUI_RGB(255,0,  0  ),//����
	Brown					= RTGUI_RGB(165,42, 42 ),//��ɫ
	FireBrick				= RTGUI_RGB(178,34, 34 ),//�ͻ�ש
	DarkRed					= RTGUI_RGB(139,0,  0  ),//���ɫ
	Maroon					= RTGUI_RGB(128,0,  0  ),//��ɫ
	White					= RTGUI_RGB(255,255,255),//����
	WhiteSmoke				= RTGUI_RGB(245,245,245),//����
	Gainsboro				= RTGUI_RGB(220,220,220),//Gainsboro
	LightGrey				= RTGUI_RGB(211,211,211),//ǳ��ɫ
	Silver					= RTGUI_RGB(192,192,192),//����ɫ
	DarkGray				= RTGUI_RGB(169,169,169),//���ɫ
	Gray					= RTGUI_RGB(128,128,128),//��ɫ
	DimGray					= RTGUI_RGB(105,105,105),//�����Ļ�ɫ
	Black					= RTGUI_RGB(0,  0,  0  ),//����
	DefaultBackground		= RTGUI_RGB(226,223,226),//Ĭ�ϱ���ɫ
};

#define default_foreground	Black
#define	default_background	DefaultBackground

#ifdef __cplusplus
}
#endif

#endif

