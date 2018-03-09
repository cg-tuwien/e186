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

		bool m_dirty;

	public:
		LightCollection() { m_dirty = false; }
		LightCollection(int capacity)
			: LightCollection()
		{
			m_lights.reserve(capacity);
			m_lights_gpu.reserve(capacity);
			m_lights_extract.reserve(capacity);
		}
		LightCollection(const LightCollection& other) noexcept = default;
		LightCollection(LightCollection&& other) noexcept = default;
		LightCollection& operator=(const LightCollection& other) noexcept = default;
		LightCollection& operator=(LightCollection&& other) noexcept = default;
		~LightCollection() { }

		L_type& operator [](int i) { return m_lights[i]; }
		L_type& at(int i) { return m_lights[i]; }
		size_t size() const { return m_lights.size(); }
		bool has_changes() const { return m_dirty; }
		const std::vector<L_gpu_type>& gpu_data() const { return m_lights_gpu; }

		void Add(const L_type& light, fn_extract_gpu_data extract_gpu_data)
		{
			m_lights.push_back(light);
			m_lights_extract.push_back(extract_gpu_data);
			m_lights_gpu.push_back(extract_gpu_data(light));
			m_dirty = true;
		}

		L_type Remove(int i)
		{
			L_type light = m_lights[i];
			m_lights.erase(m_lights.begin() + i);
			m_lights_gpu.erase(m_lights_gpu.begin() + i);
			m_lights_extract.erase(m_lights_extract.begin() + i);
			m_dirty = true;
			return light;
		}

		void UpdateGpuData()
		{
			for (size_t i = 0; i < size(); i++) {
				m_lights_gpu[i] = m_lights_extract[i](m_lights[i]);
			}
			m_dirty = true;
		}

		void UpdateGpuData(int i)
		{
			m_lights_gpu[i] = m_lights_extract[i](m_lights[i]);
			m_dirty = true;
		}

		void ClearDirty() 
		{
			m_dirty = false; 
		}

		const void* GetGpuDataPtr(bool clearDirty = true)
		{
			if(clearDirty) ClearDirty();
			return &m_lights_gpu[0];
		}
	};
}