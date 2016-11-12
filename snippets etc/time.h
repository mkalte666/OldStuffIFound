/*
File: time.h	
Purpose: Heder for the Time-class. Is responsible for Registering timers, Timer-callbacks, etc.
Author(s): Malte Kieﬂling (mkalte666)
*/
#include "base.h"

typedef void (*TimerCallback)(float, void*);
typedef unsigned int TimerIdent;

struct timer
{
	
	float m_initoffset;
	float m_curtime; //Time the timer moved on
	float m_timeelapsed; //Time since last call
	float m_endtime;
	bool m_running;
	bool m_hasCallback;
	TimerCallback callback;
	void* m_callbackArg;
};

class time
{
public:
	time(void);
	~time(void);

	void		Update(void);

	TimerIdent	RegisterTimer(bool started=true);
	TimerIdent	RegisterTimer(float timeToEnd, bool started=true);
	TimerIdent	RegisterTimer(TimerCallback callback, float timeToCall, bool started=true);
	TimerIdent	RegisterTimer(TimerCallback callback, float timeToCall, void* arg, bool started=true);
	
	void		RemoveTimer(TimerIdent ident);
	void		RemoveTimer(timer* ident);

	void		StartTimer(TimerIdent ident);
	void		StopTimer(TimerIdent ident);

	timer&		GetTimer(TimerIdent ident);

private:
	std::vector<std::shared_ptr<timer>>	m_timers;
	float						m_time;
	float						m_timeElapsed;

protected:
};