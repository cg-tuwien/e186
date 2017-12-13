#include "Stopwatch.h"

namespace e186
{
	Stopwatch::Stopwatch() : m_start_time(std::chrono::high_resolution_clock::now())
	{
		m_last_measure_time = m_start_time;
		log_info("____________________");
		log_info("| TICK TOCK M.F. ...");
	}

	Stopwatch::~Stopwatch()
	{
		Measure("Stopwatch destructor");
		log_info("|____________________");
	}

	void Stopwatch::Measure()
	{
		Measure(nullptr);
	}

	void Stopwatch::Measure(const char* location_description)
	{
		auto current = std::chrono::high_resolution_clock::now();

		log_info("|");

		if (nullptr != location_description)
		{
			log_info("| %s", location_description);
		}

		// since start
		{
			auto time_span = current - m_start_time;
			auto int_min = std::chrono::duration_cast<std::chrono::minutes>(time_span).count();
			auto int_sec = std::chrono::duration_cast<std::chrono::seconds>(time_span).count();
			auto fp_ms = std::chrono::duration<double, std::milli>(time_span).count();
			log_info("| elapsed time since start: %d min, %lld sec %lf ms", int_min, int_sec - int_min * 60, fp_ms - 1000.0 * int_sec);
		}

		// since last measure
		{
			auto time_span = current - m_last_measure_time;
			auto int_min = std::chrono::duration_cast<std::chrono::minutes>(time_span).count();
			auto int_sec = std::chrono::duration_cast<std::chrono::seconds>(time_span).count();
			auto fp_ms = std::chrono::duration<double, std::milli>(time_span).count();
			log_info("| elapsed time since last measure: %d min, %lld sec %lf ms", int_min, int_sec - int_min * 60, fp_ms - 1000.0 * int_sec);
		}

		log_info("|");

		m_last_measure_time = current;
	}
}