namespace e186
{
	TexData::TexData(GLenum texTarget) :
		TexInfo(texTarget)
	{
	}

	TexData::TexData(GLenum texTarget, uint8_t* data, std::function<void(uint8_t*)>&& destroyerFu) :
		TexInfo(texTarget)
	{
		m_data_type = GL_FLOAT;
	}

	TexData::TexData(GLenum texTarget, float* data, std::function<void(float*)>&& destroyerFu) :
		TexInfo(texTarget)
	{ 
		m_data_type = GL_UNSIGNED_BYTE;
	}

	TexData::TexData(TexData&& other) noexcept :
		TexInfo(std::move(other)),
		m_data(std::move(other.m_data)),
		m_destroyer(std::move(other.m_destroyer))
	{
		// do not unset other's m_data and m_destroyer, it will do no harm!
	}

	TexData& TexData::operator=(TexData&& other) noexcept
	{
		TexInfo::operator=(std::move(other));
		m_data = std::move(other.m_data);
		m_destroyer = std::move(other.m_destroyer);
		// do not unset other's m_data and m_destroyer, it will do no harm!
		return *this;
	}

	TexData::~TexData()
	{
		DestroyOffline(true);
	}

	void TexData::DestroyOffline(bool is_destructing)
	{
		if (m_data.index() == std::variant_npos || (GL_FLOAT != m_data_type && GL_UNSIGNED_BYTE != m_data_type))
		{
			// No m_data in TexData::DestroyOffline(), so nothing to destroy
			return;
		}

		if (GL_FLOAT == m_data_type)
		{
			auto destructor = std::get<std::function<void(float*)>>(m_destroyer);
			auto data = std::get<float*>(m_data);
			destructor(data);
		}
		else
		{
			auto destructor = std::get<std::function<void(uint8_t*)>>(m_destroyer);
			auto data = std::get<uint8_t*>(m_data);
			destructor(data);
		}

		if (is_destructing)
		{
			m_data_type = 0;
		}
	}
}
