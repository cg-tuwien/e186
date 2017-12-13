#pragma once

namespace e186
{
	// Basic timer for maximum frames per second.
	// That means, there is no fixed time step and variable delta time between frames.
	class MaxFpsTimer
	{
		double m_start_time;
		double m_abs_time;
		double m_time_since_start;
		double m_last_time;
		double m_delta_time;

	public:
		MaxFpsTimer();
		~MaxFpsTimer();

		// Call this method in every game-loop iteration
		void Tick();

		// Returns the elapsed seconds since start
		double elapsed_time() const;
		// Returns the elapsed seconds since start, cast from double to float
		float elapsed_timef() const;

		// Returns the elapsed seconds since the last frame
		double delta_time() const;
		// Returns the elapsed seconds since the last frame, cast from double to float
		float delta_timef() const;

		// Returns the absolute time
		double absolute_time() const;
		// Returns the absolute time, cast from double to float
		float absolute_timef() const;
	};

}
