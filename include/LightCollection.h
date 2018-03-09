#pragma once

namespace e186
{
	template <typename L_type, typename L_gpu_type>
	class LightCollection 
	{
	private:
		typedef std::function<L_gpu_type(const L_type&)> fn_extract_gpu_data;

		std::vector<L_type> m_lights;
		std::vector<L_gpu_type> m_lights_gpu;
		std::vector<fn_extract_gpu_data> m_lights_extract;


	public:
		LightCollection() { }
		LightCollection(int capacity) {
			m_lights.reserve(capacity);
			m_lights_gpu.reserve(capacity);
			m_lights_extract.reserve(capacity);
		}

		~LightCollection() { }
		
		void Add(const L_type& light, fn_extract_gpu_data extract_gpu_data)
		{
			m_lights.push_back(light);
			m_lights_extract.push_back(extract_gpu_data);
			m_lights_gpu.push_back(extract_gpu_data(light));
		}

		L_type& operator [](int i)
		{
			return m_lights[i];
		}

		void UpdateGpuData(int i)
		{
			m_lights_gpu[i] = m_lights_extract[i](m_lights[i]);
		}

		const std::vector<L_gpu_type>& GetGpuData() const
		{
			return m_lights_gpu;
		}

		const void* GetGpuDataPtr() const
		{
			return &m_lights_gpu[0];
		}

		const size_t size() const 
		{
			return m_lights.size();
		}

	};
}