/*
 * File      : rtgui_object.h
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
#ifndef __RTGUI_OBJECT_H__
#define __RTGUI_OBJECT_H__


#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* rtgui object type */
#define RTGUI_CONTAINER_OF(obj, type, member)	\
	((type *)((char *)(obj) - (unsigned long)(&((type *)0)->member)))

/** Casts the function pointer to an rtgui_constructor */
#define RTGUI_CONSTRUCTOR(c)  ((rtgui_constructor_t)(c))
/** Casts the function pointer to an rtgui_constructor */
#define RTGUI_DESTRUCTOR(d)    ((rtgui_destructor_t)(d))

/* pre-definetion */
typedef struct rtgui_type   rtgui_type_t;
typedef struct rtgui_object rtgui_object_t;

typedef void (*rtgui_constructor_t)(rtgui_object_t *object);
typedef void (*rtgui_destructor_t)(rtgui_object_t *object);

/* rtgui type structure */
struct rtgui_type
{
	/* type name */
	char* name;

	/* parent type link */
	struct rtgui_type *parent;

	/* constructor and destructor */
	rtgui_constructor_t constructor;
	rtgui_destructor_t destructor;

	/* size of type */
	int size;
};
typedef struct rtgui_type rtgui_type_t;
#define RTGUI_TYPE(type)			(struct rtgui_type*)&(_rtgui_##type)

#define DECLARE_CLASS_TYPE(type)	extern const struct rtgui_type _rtgui_##type
#define DEFINE_CLASS_TYPE(type, name, parent, constructor, destructor, size) \
	const struct rtgui_type _rtgui_##type = { \
	name, \
	parent, \
	RTGUI_CONSTRUCTOR(constructor), \
	RTGUI_DESTRUCTOR(destructor), \
	size }

void  rtgui_type_object_construct(const rtgui_type_t *type, rtgui_object_t *object);
void  rtgui_type_destructors_call(const rtgui_type_t *type, rtgui_object_t *object);
rt_bool_t  rtgui_type_inherits_from(const rtgui_type_t *type, const rtgui_type_t *parent);
const rtgui_type_t  *rtgui_type_parent_type_get(const rtgui_type_t *type);
const char *rtgui_type_name_get(const rtgui_type_t *type);
rtgui_type_t  *GetTypeFromName(const char *name);

#ifdef RTGUI_USING_CAST_CHECK
	#define RTGUI_OBJECT_CAST(obj, obj_type, c_type) \
		((c_type *)rtgui_object_check_cast((rtgui_object_t *)(obj), (obj_type), __FUNCTION__, __LINE__))
#else
	#define RTGUI_OBJECT_CAST(obj, obj_type, c_type)     ((c_type *)(obj))
#endif

#define RTGUI_OBJECT_CHECK_TYPE(_obj, _type) \
	(rtgui_type_inherits_from(((rtgui_object_t *)(_obj))->type, (_type)))

DECLARE_CLASS_TYPE(object);
/** Gets the type of an object */
#define RTGUI_OBJECT_TYPE       RTGUI_TYPE(object)
/** Casts the object to an rtgui_object_t */
#define RTGUI_OBJECT(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_OBJECT_TYPE, rtgui_object_t))
/** Checks if the object is an rtgui_Object */
#define RTGUI_IS_OBJECT(obj) 	(RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_OBJECT_TYPE))

enum rtgui_object_flag
{
    RTGUI_OBJECT_FLAG_NONE     = 0x00,
    RTGUI_OBJECT_FLAG_STATIC   = 0x01,
    RTGUI_OBJECT_FLAG_DISABLED = 0x02
};

/* rtgui base object */
struct rtgui_object
{
	/* object type */
	const rtgui_type_t* type;

	rt_bool_t is_static;//enum rtgui_object_flag flag;
};

rtgui_object_t *rtgui_object_create(rtgui_type_t *object_type);
void         rtgui_object_destroy(rtgui_object_t *object);

void       rtgui_object_name_set(rtgui_object_t *object, const char *name);
const char *rtgui_object_name_get(rtgui_object_t *object);

rtgui_object_t *rtgui_object_check_cast(rtgui_object_t *object, rtgui_type_t *type, const char* func, int line);

#ifdef __cplusplus
}
#endif

#endif
