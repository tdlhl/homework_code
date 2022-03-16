#include "thread_hdr.h"

void add_ready_thread(thread* ready_thread)
{
	// ��Ӧ�Ĵ���ʵ��
	ready_queue.push_back(ready_thread);
}

void schedule()
{
	if (current_thread != &idle_thread) 
	{
		if (ready_queue.size() != 0) 
		{
			ready_queue.push_back(current_thread);
			thread* temp;
			temp = ready_queue.front();
			current_thread=temp;
			ready_queue.pop_front();
		}
	}
	else 
	{
		if (ready_queue.size() != 0) 
		{
			current_thread = ready_queue.front();
			ready_queue.pop_front();
		}
	}
}

void current_thread_finished()
{
	// ʵ�ֵĴ���
	current_thread=&idle_thread;
	if (ready_queue.size() !=0)schedule();
}

void current_thread_blocked()
{
	// ��Ӧ�Ĵ���ʵ��
	blocked_queue.push_back(current_thread);
	current_thread=&idle_thread;
	if (ready_queue.size() !=0)
	{
		schedule();
	}
}

void notify()
{
	// ��Ӧ�Ĵ���ʵ��
	thread* temp;
	temp=blocked_queue.front();
	blocked_queue.pop_front();
	ready_queue.push_back(temp);
}

void notify_all()
{
	// ��Ӧ�Ĵ���ʵ��
	thread* temp;
	while(blocked_queue.size()!=0)
	{
		temp=blocked_queue.front();
		blocked_queue.pop_front();
		ready_queue.push_back(temp);
	}
}