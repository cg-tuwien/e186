#pragma once
#include <chrono>

namespace e186
{
	class Stopwatch
	{
	public:
		Stopwatch();
		Stopwatch(Stopwatch&&) = default;
		Stopwatch(const Stopwatch&) = default;
		Stopwatch& operator=(Stopwatch&&) = default;
		Stopwatch& operator=(const Stopwatch&) = default;
		~Stopwatch();
		void Measure();
		void Measure(const char* location_description);

	private:
		std::chrono::high_resolution_clock::time_point m_start_time;
		std::chrono::high_resolution_clock::time_point m_last_measure_time;
	};
}