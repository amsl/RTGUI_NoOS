#include <rtthread.h>
#include "list.h"

#include <SDL/SDL.h>

rt_mq_t *_rtgui_mq;

void rt_mq_system_init()
{
	_rtgui_mq = rt_mq_create(256, 32);
}

struct rt_mq_message
{
	struct rt_mq_message* next;
};

rt_mq_t* rt_mq_create (rt_size_t msg_size, rt_size_t max_msgs)
{
	size_t index;
	rt_mq_t *mq;
	struct rt_mq_message* head;

	/* allocate object */
	mq = (rt_mq_t*) rt_malloc(sizeof(rt_mq_t));
	if (mq == RT_NULL) return mq;

	/* get correct message size */
	mq->msg_size	= RT_ALIGN(msg_size, RT_ALIGN_SIZE);
	mq->max_msgs	= max_msgs;

	/* allocate message pool */
	mq->msg_pool = rt_malloc((mq->msg_size + sizeof(struct rt_mq_message))* mq->max_msgs);
	if (mq->msg_pool == RT_NULL)
	{
		rt_mq_delete(mq);
		return RT_NULL;
	}

	/* init message list */
	mq->head = RT_NULL;
	mq->tail = RT_NULL;

	/* init message empty list */
	mq->free = RT_NULL;

	for (index = 0; index < mq->max_msgs; index ++)
	{
		head = (struct rt_mq_message*)((rt_uint8_t*)mq->msg_pool +
			index * (mq->msg_size + sizeof(struct rt_mq_message)));
		head->next = mq->free;
		mq->free = head;
	}

	/* the initial entry is zero */
	mq->entry		= 0;

	return mq;
}

rt_err_t rt_mq_delete (rt_mq_t *mq)
{
	/* parameter check */
	RT_ASSERT(mq != RT_NULL);

	/* remove from list */
	rt_list_remove(&(mq->list));

	/* free mailbox pool */
	rt_free(mq->msg_pool);

	/* delete mailbox object */
	rt_free(mq);

	return RT_EOK;
}

rt_err_t rt_mq_send (rt_mq_t *mq, void* buffer, rt_size_t size)
{
	struct rt_mq_message *msg;

	/* greater than one message size */
	if (size > mq->msg_size) return -RT_ERROR;

	/* get a free list, there must be an empty item */
	msg = (struct rt_mq_message*)mq->free;

	/* message queue is full */
	if (msg == RT_NULL)
	{
		return -RT_EFULL;
	}

	/* move free list pointer */
	mq->free = msg->next;
	/* the msg is the new tail of list, the next shall be NULL */
	msg->next = RT_NULL;

	/* copy buffer */
	rt_memcpy(msg + 1, buffer, size);

	/* link msg to message queue */
	if (mq->tail != RT_NULL)
	{
		/* if the tail exists, */
		((struct rt_mq_message*)mq->tail)->next = msg;
	}
	
	/* set new tail */
	mq->tail = msg;

	/* if the head is empty, set head */
	if (mq->head == RT_NULL)mq->head = msg;

	/* increase message entry */
	mq->entry ++;

	return RT_EOK;
}

rt_err_t rt_mq_recv (rt_mq_t *mq, void* buffer, rt_size_t size)
{
	rt_err_t r;
	struct rt_mq_message *msg;

	/* mq is empty */
	if (mq->entry == 0)
	{
		return -RT_ETIMEOUT;
	}

	/* get message from queue */
	msg = (struct rt_mq_message*) mq->head;

	/* move message queue head */
	mq->head = msg->next;

	/* reach queue tail, set to NULL */
	if (mq->tail == msg) mq->tail = RT_NULL;

	/* decrease message entry */
	mq->entry --;

	/* copy message */
	rt_memcpy(buffer, msg + 1, size > mq->msg_size ? (mq->msg_size) : size);

	/* put message to free list */
	msg->next = (struct rt_mq_message*)mq->free;
	mq->free = msg;

	return RT_EOK;
}
