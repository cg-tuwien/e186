#pragma once

namespace e186
{
	class TexData : public TexInfo
	{
	protected:
		std::variant<uint8_t*, float*> m_data;
		std::variant<std::function<void(uint8_t*)>, std::function<void(float*)>> m_destroyer;
			
	public:
		TexData(GLenum texTarget);
		TexData(GLenum texTarget, uint8_t* data, std::function<void(uint8_t*)>&& destroyerFu);
		TexData(GLenum texTarget, float* data, std::function<void(float*)>&& destroyerFu);
		TexData(const TexData& other) = delete;
		TexData& operator=(const TexData& other) = delete;
		TexData(TexData&& other) noexcept;
		TexData& operator=(TexData&& other) noexcept;
		virtual ~TexData();

		void DestroyOffline(bool is_destructing = false);
	};
}
