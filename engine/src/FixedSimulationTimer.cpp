#include "FixedSimulationTimer.h"

namespace e186
{
	FixedSimulationTimer::FixedSimulationTimer() : 
		m_time_since_start(0.0), 
		m_delta_time(0.0), 
		m_min_render_hz(1.0),
		m_max_render_deltat_time(1.0 / 1.0),
		m_cur_render_hz(0.0),
		m_fixed_hz(60.0), 
		m_fixed_delta_time(1.0 / 60.0)
	{
		m_last_fixed_tick = m_last_time = m_abs_time = m_start_time = glfwGetTime();
		m_next_fixed_tick = m_last_fixed_tick + m_fixed_delta_time;
	}

	FixedSimulationTimer::~FixedSimulationTimer()
	{
	}

	FixedSimulationTimer::FrameType FixedSimulationTimer::Tick()
	{
		m_abs_time = glfwGetTime();
		m_time_since_start = m_abs_time - m_start_time;

		auto dt = m_time_since_start - m_last_time;

		// should we simulate or render?
		if (m_abs_time > m_next_fixed_tick && dt < m_max_render_deltat_time)
		{
			m_last_fixed_tick = m_next_fixed_tick;
			m_next_fixed_tick += m_fixed_delta_time;
			return FrameType_Simulate;
		}

		m_delta_time = dt;
		m_last_time = m_time_since_start;
		m_cur_render_hz = 1.0 / m_delta_time;
		return FrameType_Render;
	}

	void FixedSimulationTimer::set_min_render_hertz(double minHz)
	{
		m_min_render_hz = minHz;
		m_max_render_deltat_time = 1.0 / m_min_render_hz;
	}

	void FixedSimulationTimer::set_fixed_simulation_hertz(double hz)
	{
		m_fixed_hz = hz;
		m_fixed_delta_time = 1.0 / m_fixed_hz;
	}

	double FixedSimulationTimer::fixed_simulation_delta_time() const
	{
		return m_fixed_delta_time;
	}

	double FixedSimulationTimer::fixed_simulation_hertz() const
	{
		return m_fixed_hz;
	}

	double FixedSimulationTimer::elapsed_time() const
	{
		return m_time_since_start;
	}

	double FixedSimulationTimer::render_delta_time() const
	{
		return m_delta_time;
	}

	double FixedSimulationTimer::absolute_time() const
	{
		return m_abs_time;
	}

	double FixedSimulationTimer::current_render_hertz() const
	{
		return m_cur_render_hz;
	}

	float FixedSimulationTimer::elapsed_timef() const
	{
		return static_cast<float>(m_time_since_start);
	}

	float FixedSimulationTimer::render_delta_timef() const
	{
		return static_cast<float>(m_delta_time);
	}

	float FixedSimulationTimer::absolute_timef() const
	{
		return static_cast<float>(m_abs_time);
	}
}