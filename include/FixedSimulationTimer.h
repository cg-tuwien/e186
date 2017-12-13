#pragma once

namespace e186
{
	// Timer whith a fixed simulation rate (i.e. constant simulation delta time, see SetFixedSimHz)
	// and variable rendering rate (i.e. as many render fps as possible, variable rendering delta time), 
	// however, also providing minimum render-fps (see SetMinRendHz)
	class FixedSimulationTimer
	{
	public:
		enum FrameType
		{
			FrameType_Simulate,
			FrameType_Render
		};

	private:
		double m_start_time;
		double m_abs_time;
		double m_time_since_start;

		double m_last_time;
		double m_delta_time;
		double m_cur_render_hz;
		double m_min_render_hz;
		double m_max_render_deltat_time;

		double m_fixed_delta_time;
		double m_fixed_hz;
		double m_last_fixed_tick;
		double m_next_fixed_tick;

	public:
		FixedSimulationTimer();
		~FixedSimulationTimer();

		// Call this method in every game-loop iteration
		FrameType Tick();

		// Set the minimum rendering hertz
		void set_min_render_hertz(double minHz);

		// Set Hz and fixed timestep
		void set_fixed_simulation_hertz(double hz);

		// Returns the fixed update rate a.k.a. fixed timestep in seconds
		double fixed_simulation_delta_time() const;

		// Returns the updates per second for the fixed timestep a.k.a. hertz
		double fixed_simulation_hertz() const;

		// Returns the current render hertz
		double current_render_hertz() const;

		// Returns the elapsed seconds since game start
		double elapsed_time() const;
		// Returns the elapsed seconds since game start, cast from double to float
		float elapsed_timef() const;

		// Returns the elapsed seconds since the last frame
		double render_delta_time() const;
		/// Returns the elapsed seconds since the last frame, cast from double to float
		float render_delta_timef() const;

		// Returns the absolute time
		double absolute_time() const;
		// Returns the absolute time, cast from double to float
		float absolute_timef() const;

	};
}