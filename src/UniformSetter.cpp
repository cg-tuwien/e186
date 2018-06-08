#include "UniformSetter.h"

namespace e186
{

	UniformSubSetter::UniformSubSetter() noexcept : m_func()
	{
	}

	UniformSubSetter::UniformSubSetter(UniformSubSetterFunc func) noexcept : m_func(std::move(func))
	{
	}

	UniformSubSetter::UniformSubSetter(UniformSubSetter&& other) noexcept : m_func(std::move(other.m_func))
	{
	}

	UniformSubSetter::UniformSubSetter(const UniformSubSetter& other) noexcept : m_func(other.m_func)
	{
	}

	UniformSubSetter& UniformSubSetter::operator= (UniformSubSetter&& other) noexcept
	{
		m_func = std::move(other.m_func);
		return *this;
	}

	UniformSubSetter& UniformSubSetter::operator= (const UniformSubSetter& other)
	{
		m_func = other.m_func;
		return *this;
	}

	UniformSubSetter::~UniformSubSetter()
	{
	}

	void UniformSubSetter::operator()(const Shader& shader, const MaterialData& mat_data)
	{
		m_func(shader, mat_data, *this);
	}

	void UniformSubSetter::set_func(UniformSubSetterFunc func)
	{
		m_func = std::move(func);
	}

	// - - - - - - - - - - - - UniformSetter - - - - - - - - - - - - 

	UniformSetter::UniformSetter() noexcept
		: m_shader(nullptr),
		m_func(),
		m_usage_mode(UniformSetterUsageMode::Static)
	{
	}

	UniformSetter::UniformSetter(Shader& shader, UniformSetterFunc func, UniformSetterUsageMode usage_mode) noexcept
		: m_shader(&shader),
		m_func(std::move(func)),
		m_usage_mode(std::move(usage_mode))
	{
		// inform the shader about the new UniformSetter
		assert(is_valid());
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter::UniformSetter(UniformSetter&& other) noexcept
		: m_shader(std::move(other.m_shader)),
		m_func(std::move(other.m_func)),
		m_usage_mode(std::move(other.m_usage_mode))
	{
		// invalidate other's shader-pointer to indicate it has been moved from
		other.m_shader = nullptr;
		// inform the shader about the new memory location of the UniformSetter
		assert(is_valid());
		m_shader->HandleUniformSetterMoved(&other, this);
	}

	UniformSetter::UniformSetter(const UniformSetter& other) noexcept
		: m_shader(other.m_shader),
		m_func(other.m_func),
		m_usage_mode(other.m_usage_mode)
	{
		// inform the shader about the new UniformSetter (copy)
		assert(is_valid());
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter& UniformSetter::operator= (UniformSetter&& other) noexcept
	{
		assert(other.is_valid());
		m_shader = std::move(other.m_shader);
		m_func = std::move(other.m_func);
		m_usage_mode = std::move(other.m_usage_mode);
		// invalidate other's shader-pointer to indicate it has been moved from
		other.m_shader = nullptr;
		// inform the shader about the new memory location of the UniformSetter
		assert(is_valid());
		m_shader->HandleUniformSetterMoved(&other, this);
		return *this;
	}

	UniformSetter& UniformSetter::operator= (const UniformSetter& other) noexcept
	{
		auto* shader_at_the_beginning = m_shader;
		auto valid_at_the_beginning = is_valid();
		m_shader = other.m_shader;
		m_func = other.m_func;
		m_usage_mode = other.m_usage_mode;
		// inform the shader about the new UniformSetter, IF it is new:
		if (!valid_at_the_beginning && is_valid())
		{
			m_shader->HandleUniformSetterCreated(this);
		}
		else if (valid_at_the_beginning && !is_valid())
		{
			shader_at_the_beginning->HandleUniformSetterDeleted(this);
		}
		else if (valid_at_the_beginning && is_valid() && shader_at_the_beginning != m_shader)
		{
			shader_at_the_beginning->HandleUniformSetterDeleted(this);
			m_shader->HandleUniformSetterCreated(this);
		}
		return *this;
	}

	UniformSetter::~UniformSetter()
	{
		if (is_valid())
		{
			m_shader->HandleUniformSetterDeleted(this);
		}
	}

	void UniformSetter::operator()(const MaterialData& mat_data)
	{
		m_func(*m_shader, mat_data);
	}

	void UniformSetter::operator()(const Shader& shader, const MaterialData& mat_data)
	{
		m_func(shader, mat_data);
	}

	void UniformSetter::ShaderUpdated(Shader& new_shader)
	{
		*this = std::move(CreateUniformSetterForShader(new_shader, m_usage_mode));
	}
}
