/*
File: time.cpp
Purpose: contains the functions for time.h
Author(s): Malte Kießling (mkalte666)
*/

#include "time.h"

/*
* @name: time
* @param: void
* @return: void
* @description: constructor for the time-class. Resets the glfw-time and fills everything with the initial values.
*/
time::time(void)
{
	glfwSetTime(0.0f);
	m_time = (float)glfwGetTime();
	m_timeElapsed = 0;
}

/*
* @name: ~time
* @param: void
* @return: void
* @description: destructor for the time-class. Clears the callback-container.
*/
time::~time(void)
{
	glfwSetTime(0);
	m_timers.clear();
}

/*
* @name: Update
* @param: void
* @return: void
* @description: Updates global time, updates times and ggf. calles timer callbacks
*/
void time::Update(void)
{
	m_timeElapsed = (float)glfwGetTime() - m_time;
	m_time = (float)glfwGetTime();

	//We update our timers. If we have timers...
	if(m_timers.size()>0) {
		for(unsigned int i = 0; i < m_timers.size(); i++) {
			//And there is a timer on that adress
			if(m_timers[i]) {
				//Lets test if its running
				if(m_timers[i]->m_running) {
					//If yes, we refresh the times. 
					m_timers[i]->m_curtime += m_timeElapsed;
					m_timers[i]->m_timeelapsed = m_timeElapsed;
					//Oh, and if it have an endtime (timer::m_endtime >0) then we want to stop it
					if(m_timers[i]->m_endtime >0 && m_timers[i]->m_curtime >= m_timers[i]->m_endtime) {
						m_timers[i]->m_running = false;
						//Oh, and if it has a callback, we want to call it
						if(m_timers[i]->m_hasCallback) 
							m_timers[i]->callback(m_timeElapsed, m_timers[i]->m_callbackArg);
					}
				} 
				//If the timer is not running, to time elapsed. so set it to null!
				else {
					m_timers[i]->m_timeelapsed = 0.0f;
				}
			}
		}
	}	
}


/*
* @name: RegisterTimer
* @param:	bool started=true: gets the timer started when it's added?
* @return: TimerIdent
* @description: Creates a new timer and returns and TimerIdent to acces it.
*/
TimerIdent time::RegisterTimer(bool started)
{
	std::shared_ptr<timer> newtimer( new timer);
	newtimer->m_hasCallback = false;
	newtimer->m_initoffset = m_time;
	newtimer->m_endtime = 0.0f;
	newtimer->m_curtime = 0.0f;
	newtimer->m_timeelapsed = 0.0f;
	newtimer->m_callbackArg = NULL;
	newtimer->callback = NULL;
	newtimer->m_running = started;
	m_timers.push_back(newtimer);
	return (TimerIdent)(m_timers.size()-1);
}

/*
* @name: RegisterTimer
* @param:	float timeToEnd: time after the timer will end
			bool started=true: gets the timer started when it's added?
* @return: TimerIdent
* @description: Creates a new timer and returns and TimerIdent to acces it.
*/
TimerIdent time::RegisterTimer(float timeToEnd, bool started)
{
	std::shared_ptr<timer> newtimer( new timer);
	newtimer->m_hasCallback = false;
	newtimer->m_initoffset = m_time;
	newtimer->m_endtime = timeToEnd;
	newtimer->m_curtime = 0.0f;
	newtimer->m_timeelapsed = 0.0f;
	newtimer->m_callbackArg = NULL;
	newtimer->callback = NULL;
	newtimer->m_running = started;
	m_timers.push_back(newtimer);
	return (TimerIdent)(m_timers.size()-1);
}

/*
* @name: RegisterTimer
* @param:	TimerCallback callback: callback that will be called when the timer ends.
			float timeToEnd: time after the timer will end
			bool started=true: gets the timer started when it's added?
* @return: TimerIdent
* @description: Creates a new timer and returns and TimerIdent to acces it.
*/
TimerIdent time::RegisterTimer(TimerCallback callback, float timeToCall, bool started)
{
	std::shared_ptr<timer> newtimer( new timer);
	newtimer->m_hasCallback = true;
	newtimer->m_initoffset = m_time;
	newtimer->m_endtime = timeToCall;
	newtimer->m_curtime = 0.0f;
	newtimer->m_timeelapsed = 0.0f;
	newtimer->m_callbackArg = NULL;
	newtimer->callback = callback;
	newtimer->m_running = started;
	m_timers.push_back(newtimer);
	return (TimerIdent)(m_timers.size()-1);
}

/*
* @name: RegisterTimer
* @param:	TimerCallback callback: callback that will be called when the timer ends.
			float timeToEnd: time after the timer will end
			void* arg: argument for the timer-callback
			bool started=true: gets the timer started when it's added?
* @return: TimerIdent
* @description: Creates a new timer and returns and TimerIdent to acces it.
*/
TimerIdent time::RegisterTimer(TimerCallback callback, float timeToCall, void* arg, bool started)
{
	std::shared_ptr<timer> newtimer( new timer);
	newtimer->m_hasCallback = true;
	newtimer->m_initoffset = m_time;
	newtimer->m_endtime = timeToCall;
	newtimer->m_curtime = 0.0f;
	newtimer->m_timeelapsed = 0.0f;
	newtimer->m_callbackArg = arg;
	newtimer->callback = callback;
	newtimer->m_running = started;
	m_timers.push_back(newtimer);
	return (TimerIdent)(m_timers.size()-1);
}

//TODO: MAKE THE m_timer[n] - MEMBER AVAILBE FOR NEW TIMERS

/*
* @name: RemoveTimer
* @param: TimerIdent ident: Timer to remove
* @return: void
* @description: Removes timer by TimerIdent
*/
void time::RemoveTimer(TimerIdent ident)
{
	if(m_timers[ident]) {
		m_timers[ident] = NULL;
	}
}

/*
* @name: RemoveTimer
* @param: timer* ident: Timer to remove
* @return: void
* @description: Removes timer by TimerIdent
*/
void time::RemoveTimer(timer* ident)
{
	for(unsigned int i = 0; i < m_timers.size(); i++) {
		if(ident == m_timers[i].get()) {
			m_timers[i] = NULL;
		}
	}
}

timer& time::GetTimer(TimerIdent ident)
{
	
	if(m_timers.size()-1 < ident || !m_timers[ident])
		std::cout << "error im timer-id! returning nothing (emtpty timer!)!\n";
	else
		return  *m_timers[ident].get();
	
}
