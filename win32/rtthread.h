#ifndef __RT_THREAD_H__
#define __RT_THREAD_H__

typedef char  rt_int8_t;
typedef short rt_int16_t;
typedef long  rt_int32_t;
typedef unsigned char  rt_uint8_t;
typedef unsigned short rt_uint16_t;
typedef unsigned long  rt_uint32_t;
typedef long rt_base_t;
typedef unsigned long rt_ubase_t;
typedef int rt_bool_t;

typedef rt_base_t   rt_err_t;		/* Type for error number.	*/
typedef rt_uint32_t rt_time_t;		/* Type for time stamp. 	*/
typedef rt_uint32_t rt_tick_t;		/* Type for tick count. 	*/
typedef rt_base_t  	rt_flag_t;		/* Type for flags.			*/
typedef rt_uint32_t	rt_size_t;		/* Type for size number.	*/
typedef rt_uint8_t	rt_dev_t;		/* Type for device			*/
typedef rt_int32_t	rt_off_t;		/* Type for offset, supports 4G at most */

/* RT-Thread bool type definitions */
#define RT_TRUE 			1
#define RT_FALSE 			0

/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error       */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out               */
#define RT_EFULL                        3               /**< The resource is full    */
#define RT_EEMPTY                       4               /**< The resource is empty   */
#define RT_ENOMEM                       5               /**< No memory               */
#define RT_ENOSYS                       6               /**< No system               */
#define RT_EBUSY                        7               /**< Busy                    */
#define RT_EIO                          8               /**< IO error                */

#define RT_ALIGN_SIZE		4
#define RT_ALIGN(size, align)	(((size) + (align) - 1) & ~((align)-1))
#define RT_ASSERT(EX)		if (!(EX)) rt_assert(__FILE__, __LINE__);
#define RT_NULL 			((void *)0)

#define RT_IPC_FLAG_FIFO	0x00	/* FIFOed IPC. @ref IPC. */
#define RT_IPC_FLAG_PRIO	0x01	/* PRIOed IPC. @ref IPC. */

#define RT_WAITING_FOREVER	-1		/* Block forever until get resource. */
#define RT_WAITING_NO		0		/* Non-block. */

#define RT_NAME_MAX			8
#define RTGUI_NAME_MAX		16

#ifdef _MSC_VER
#define rt_inline __inline
#else
#define rt_inline static inline
#endif

struct rt_list_node
{
	struct rt_list_node *next;	/* point to next node. */
	struct rt_list_node *prev;	/* point to prev node. */
};
typedef struct rt_list_node rt_list_t;	/* Type for lists. */

struct rt_messagequeue
{
	rt_list_t list;			/* mq list */

	void* msg_pool;			/* start address of message queue. */

	rt_size_t msg_size;		/* message size of each message. */
	rt_size_t max_msgs;		/* max number of messages. */

	void* head;	/* list head. */
	void* tail;	/* list tail. */
	void* free;	/* pointer indicated the free node of queue. */

	rt_ubase_t entry;		/* index of messages in the queue. */
};
typedef struct rt_messagequeue rt_mq_t;

/*
 * message queue interface
 */
rt_mq_t* rt_mq_create (rt_size_t msg_size, rt_size_t max_msgs);
rt_err_t rt_mq_delete (rt_mq_t *mq);
extern rt_mq_t *_rtgui_mq;

rt_err_t rt_mq_send (rt_mq_t *mq, void* buffer, rt_size_t size);
rt_err_t rt_mq_recv (rt_mq_t *mq, void* buffer, rt_size_t size);

/*
* timer interface
*/
#define RT_TIMER_FLAG_ONE_SHOT		0x0	/* one shot timer. */
#define RT_TIMER_FLAG_PERIODIC		0x2	/* periodic timer. */

struct rt_timer
{
	char name[RT_NAME_MAX];	/* the name of thread.	*/
	rt_uint8_t  flag;			/* flag of kernel object1 */

	void (*timeout_func)(void* parameter);/* timeout function. */
	void *parameter;					/* timeout function's parameter. */

	void* host_timer;
};
typedef struct rt_timer* rt_timer_t;
void rt_timer_init(rt_timer_t timer,
				   const char* name,
				   void (*timeout)(void* parameter), void* parameter,
				   rt_tick_t time, rt_uint8_t flag);
rt_err_t rt_timer_detach(rt_timer_t timer);
rt_timer_t rt_timer_create(const char* name,
						   void (*timeout)(void* parameter), void* parameter,
						   rt_tick_t time, rt_uint8_t flag);
rt_err_t rt_timer_delete(rt_timer_t timer);
rt_err_t rt_timer_start(rt_timer_t timer);
rt_err_t rt_timer_stop(rt_timer_t timer);
rt_err_t rt_timer_control(rt_timer_t timer, rt_uint8_t cmd, void* arg);

/**
 * device (I/O) class type
 */
enum rt_device_class_type
{
	RT_Device_Class_Char = 0,						/**< character device							*/
	RT_Device_Class_Block,							/**< block device 								*/
	RT_Device_Class_NetIf,							/**< net interface 								*/
	RT_Device_Class_MTD,							/**< memory device 								*/
	RT_Device_Class_CAN,							/**< CAN device 								*/
	RT_Device_Class_RTC,							/**< RTC device 								*/
	RT_Device_Class_Sound,							/**< Sound device 								*/
	RT_Device_Class_Graphic,						/**< Graphic device                             */
	RT_Device_Class_I2C, 							/**< I2C device                                 */
	RT_Device_Class_USBDevice,						/**< USB slave device                           */
	RT_Device_Class_USBHost,						/**< USB host bus								*/
	RT_Device_Class_Unknown							/**< unknown device 							*/
};

/**
 * device flags defitions
 */
#define RT_DEVICE_FLAG_DEACTIVATE		0x000		/**< device is not not initialized 				*/

#define RT_DEVICE_FLAG_RDONLY			0x001		/**< read only 									*/
#define RT_DEVICE_FLAG_WRONLY			0x002		/**< write only 								*/
#define RT_DEVICE_FLAG_RDWR				0x003		/**< read and write 							*/

#define RT_DEVICE_FLAG_REMOVABLE		0x004		/**< removable device 							*/
#define RT_DEVICE_FLAG_STANDALONE		0x008		/**< standalone device							*/
#define RT_DEVICE_FLAG_ACTIVATED		0x010		/**< device is activated 						*/
#define RT_DEVICE_FLAG_SUSPENDED		0x020		/**< device is suspended 						*/
#define RT_DEVICE_FLAG_STREAM			0x040		/**< stream mode 								*/

#define RT_DEVICE_FLAG_INT_RX			0x100		/**< INT mode on Rx 							*/
#define RT_DEVICE_FLAG_DMA_RX			0x200		/**< DMA mode on Rx 							*/
#define RT_DEVICE_FLAG_INT_TX			0x400		/**< INT mode on Tx 							*/
#define RT_DEVICE_FLAG_DMA_TX			0x800		/**< DMA mode on Tx								*/

#define RT_DEVICE_OFLAG_CLOSE			0x000		/**< device is closed 							*/
#define RT_DEVICE_OFLAG_RDONLY			0x001		/**< read only access							*/
#define RT_DEVICE_OFLAG_WRONLY			0x002		/**< write only access							*/
#define RT_DEVICE_OFLAG_RDWR			0x003		/**< read and write 							*/
#define RT_DEVICE_OFLAG_OPEN			0x008		/**< device is opened 							*/

/**
 * general device commands
 */
#define RT_DEVICE_CTRL_RESUME	   		0x01		/**< resume device 								*/
#define RT_DEVICE_CTRL_SUSPEND	    	0x02		/**< suspend device 							*/

/**
 * special device commands
 */
#define RT_DEVICE_CTRL_CHAR_STREAM		0x10		/**< stream mode on char device 				*/
#define RT_DEVICE_CTRL_BLK_GETGEOME		0x10		/**< get geometry information 					*/
#define RT_DEVICE_CTRL_NETIF_GETMAC		0x10		/**< get mac address 							*/
#define RT_DEVICE_CTRL_MTD_FORMAT		0x10		/**< format a MTD device 						*/
#define RT_DEVICE_CTRL_RTC_GET_TIME		0x10		/**< get time 									*/
#define RT_DEVICE_CTRL_RTC_SET_TIME		0x11		/**< set time 									*/

#define RT_DEVICE_CTRL_LCD_GET_SCREENINFO	0x10
#define RT_DEVICE_CTRL_LCD_SET_SCREENINFO	0x11
#define RT_DEVICE_CTRL_LCD_GET_FRAMEBUFFER	0x12
#define RT_DEVICE_CTRL_LCD_UPDATE			0x13

#define RT_DEVICE(device)				((struct rt_device*)(device))

typedef struct rt_device* rt_device_t;
/**
 * Device structure
 */
struct rt_device
{
	rt_list_t list;			/* mq list */
	char name[32];

	enum rt_device_class_type type;					/**< device type 								*/
	rt_uint16_t flag, open_flag;					/**< device flag and device open flag			*/

	/* device call back */
	rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
	rt_err_t (*tx_complete)(rt_device_t dev, void* buffer);

	/* common device interface */
	rt_err_t  (*init)	(rt_device_t dev);
	rt_err_t  (*open)	(rt_device_t dev, rt_uint16_t oflag);
	rt_err_t  (*close)	(rt_device_t dev);
	rt_size_t (*read)	(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size);
	rt_size_t (*write)	(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size);
	rt_err_t  (*control)(rt_device_t dev, rt_uint8_t cmd, void *args);

	void* user_data;								/**< device private data 						*/
};
rt_device_t rt_device_find(const char* name);

rt_err_t rt_device_register(rt_device_t dev, const char* name, rt_uint16_t flags);
rt_err_t rt_device_unregister(rt_device_t dev);
rt_err_t rt_device_init_all(void);

rt_err_t rt_device_set_rx_indicate(rt_device_t dev, rt_err_t (*rx_ind )(rt_device_t dev, rt_size_t size));
rt_err_t rt_device_set_tx_complete(rt_device_t dev, rt_err_t (*tx_done)(rt_device_t dev, void *buffer));

rt_err_t  rt_device_init (rt_device_t dev);
rt_err_t  rt_device_open (rt_device_t dev, rt_uint16_t oflag);
rt_err_t  rt_device_close(rt_device_t dev);
rt_size_t rt_device_read (rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size);
rt_size_t rt_device_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size);
rt_err_t  rt_device_control(rt_device_t dev, rt_uint8_t cmd, void* arg);

/**
 * graphic device control command 
 */
#define RTGRAPHIC_CTRL_RECT_UPDATE	0
#define RTGRAPHIC_CTRL_POWERON		1
#define RTGRAPHIC_CTRL_POWEROFF		2
#define RTGRAPHIC_CTRL_GET_INFO		3
#define RTGRAPHIC_CTRL_SET_MODE		4

/* graphic deice */
enum 
{
	RTGRAPHIC_PIXEL_FORMAT_MONO = 0,
	RTGRAPHIC_PIXEL_FORMAT_GRAY4,
	RTGRAPHIC_PIXEL_FORMAT_GRAY16,
	RTGRAPHIC_PIXEL_FORMAT_RGB332,
	RTGRAPHIC_PIXEL_FORMAT_RGB444,
	RTGRAPHIC_PIXEL_FORMAT_RGB565,
	RTGRAPHIC_PIXEL_FORMAT_RGB565P,
	RTGRAPHIC_PIXEL_FORMAT_RGB666,
	RTGRAPHIC_PIXEL_FORMAT_RGB888,
	RTGRAPHIC_PIXEL_FORMAT_ARGB888
};
/**
 * build a pixel position according to (x, y) coordinates.
 */
#define RTGRAPHIC_PIXEL_POSITION(x, y)	((x << 16) | y)

/**
 * graphic device information structure
 */
struct rt_device_graphic_info
{
	rt_uint8_t  pixel_format;		/**< graphic format 		*/
	rt_uint8_t  bits_per_pixel;		/**< bits per pixel 		*/
	rt_uint16_t reserved;			/**< reserved field			*/

	rt_uint16_t width;				/**< width of graphic device  */
	rt_uint16_t height;				/**< height of graphic device */

	rt_uint8_t *framebuffer;		/**< frame buffer 			*/
};

/**
 * rectangle information structure
 */
struct rt_device_rect_info
{
	rt_uint16_t x, y;				/**< x, y coordinate 		*/
	rt_uint16_t w, h;				/**< width and height       */
};

/**
 * graphic operations
 */
struct rt_device_graphic_ops
{
	void (*set_pixel) (const char* pixel, int x, int y);
	void (*get_pixel) (char* pixel, int x, int y);

	void (*draw_hline)(const char* pixel, int x1, int x2, int y);
	void (*draw_vline)(const char* pixel, int x, int y1, int y2);

	void (*blit_line) (const char* pixel, int x, int y, rt_size_t size);
};
#define rt_graphix_ops(device)		((struct rt_device_graphic_ops*)(device->user_data))

/*
 * general kernel service
 */
void rt_kprintf(const char *fmt, ...);

#include <stdio.h>
#include <string.h>
#define rt_sprintf sprintf
#define rt_snprintf _snprintf
#define rt_strstr strstr
#define rt_strcmp strcmp

void* rt_memset(void *src, int c, rt_ubase_t n);
void* rt_memcpy(void *dest, const void *src, rt_ubase_t n);
int rt_memcmp(const void *dst, const void *src, rt_ubase_t n);
void *rt_memmove(void *dst, const void *src, rt_ubase_t count);

rt_ubase_t rt_strncmp(const char * cs, const char * ct, rt_ubase_t count);
char *rt_strncpy(char *dest, const char *src, rt_ubase_t n);

rt_ubase_t rt_strlen (const char *src);
char* rt_strdup(const char* str);

void* rt_malloc(rt_size_t nbytes);
void* rt_realloc(void *ptr, rt_size_t size);
void rt_free (void *ptr);
void rt_assert(const char* str, int line);

#define RT_TICK_PER_SECOND  100

void rt_system_tick_init(void);
rt_tick_t rt_tick_get(void);

#define RTGUI_IMAGE_PNG
#define RTGUI_IMAGE_JPEG
#define RTGUI_IMAGE_BMP
#define RTGUI_IMAGE_XPM
#define RTGUI_USING_FONT16
#define RTGUI_USING_DFS_FILERW 

#endif
