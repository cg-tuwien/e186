#pragma once

namespace e186
{
	template <typename TLight, typename TGpuData, typename TIndex = TGpuData>
	class LightCollection
	{
		using ExtractGpuDataFuncType = std::function<TGpuData(const TLight&)>;

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

		TIndex operator [](const int i) { return m_lights[i]; }
		auto size() const { return m_lights.size(); }
		bool has_changes() const { return m_dirty; }
		const std::vector<TGpuData>& gpu_data() const { return m_lights_gpu; }

		void Add(TLight light, ExtractGpuDataFuncType extract_gpu_data)
		{
			m_lights_gpu.push_back(extract_gpu_data(light));
			m_lights.push_back(std::move(light));
			m_lights_extract.push_back(std::move(extract_gpu_data));
			m_dirty = true;
		}

		TLight Remove(int i)
		{
			TLight light = m_lights[i];
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

		const void* GetGpuDataPtr(bool clear_dirty = true)
		{
			if(clear_dirty) ClearDirty();
			return &m_lights_gpu[0];
		}

	private:
		std::vector<TLight> m_lights;
		std::vector<TGpuData> m_lights_gpu;
		std::vector<ExtractGpuDataFuncType> m_lights_extract;
		bool m_dirty;
	};
}
