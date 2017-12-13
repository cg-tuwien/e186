#include "MaxFpsTimer.h"

namespace e186
{
	

	MaxFpsTimer::MaxFpsTimer()
		: m_start_time(0.0), 
		m_last_time(0.0), 
		m_abs_time(0.0), 
		m_time_since_start(0.0), 
		m_delta_time(0.0)
	{
		m_last_time = m_abs_time = m_start_time = glfwGetTime();
	}


	MaxFpsTimer::~MaxFpsTimer()
	{
	}		
	
	void MaxFpsTimer::Tick()
	{
		m_abs_time = glfwGetTime();
		m_time_since_start = m_abs_time - m_start_time;
		m_delta_time = m_time_since_start - m_last_time;
		m_last_time = m_time_since_start;
	}

	double MaxFpsTimer::elapsed_time() const
	{
		return m_time_since_start;
	}
	float MaxFpsTimer::elapsed_timef() const
	{
		return static_cast<float>(m_time_since_start);
	}

	double MaxFpsTimer::delta_time() const
	{
		return m_delta_time;
	}
	float MaxFpsTimer::delta_timef() const
	{
		return static_cast<float>(m_delta_time);
	}

	double MaxFpsTimer::absolute_time() const
	{
		return m_abs_time;
	}
	float MaxFpsTimer::absolute_timef() const
	{
		return static_cast<float>(m_abs_time);
	}

}
