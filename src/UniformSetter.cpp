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

	void UniformSetter::NotifyShaderAboutCreation()
	{
		if (nullptr != m_shader)
		{
			m_shader->HandleUniformSetterCreated(this);
		}
	}

	void UniformSetter::NotifyShaderAboutDeletion()
	{
		if (nullptr != m_shader)
		{
			m_shader->HandleUniformSetterDeleted(this);
		}
	}

	UniformSetter::UniformSetter() noexcept
		: m_shader(nullptr),
		m_func(),
		m_usage_mode(UniformSetterUsageMode::Static)
	{
		NotifyShaderAboutCreation();
	}

	UniformSetter::UniformSetter(Shader& shader, UniformSetterFunc func, UniformSetterUsageMode usage_mode) noexcept
		: m_shader(&shader),
		m_func(std::move(func)),
		m_usage_mode(std::move(usage_mode))
	{
		// inform the shader about the new UniformSetter
		NotifyShaderAboutCreation();
	}

	UniformSetter::UniformSetter(UniformSetter&& other) noexcept
		: m_shader(std::move(other.m_shader)),
		m_func(std::move(other.m_func)),
		m_usage_mode(std::move(other.m_usage_mode))
	{
		// cleanup other and invalidate its shader-pointer to indicate it has been moved from
		other.NotifyShaderAboutDeletion();
		other.m_shader = nullptr;
		// inform the shader about the new UniformSetter
		NotifyShaderAboutCreation();
	}

	UniformSetter::UniformSetter(const UniformSetter& other) noexcept
		: m_shader(other.m_shader),
		m_func(other.m_func),
		m_usage_mode(other.m_usage_mode)
	{
		// inform the shader about the new UniformSetter (copy)
		NotifyShaderAboutCreation();
	}

	UniformSetter& UniformSetter::operator= (UniformSetter&& other) noexcept
	{
		// possibly clean myself up
		NotifyShaderAboutDeletion();

		// move data
		m_shader = std::move(other.m_shader);
		m_func = std::move(other.m_func);
		m_usage_mode = std::move(other.m_usage_mode);

		// clean up other and invalidate its shader-pointer to indicate it has been moved from
		other.NotifyShaderAboutDeletion();
		other.m_shader = nullptr;

		// inform the shader about the new UniformSetter
		NotifyShaderAboutCreation();
		return *this;
	}

	UniformSetter& UniformSetter::operator= (const UniformSetter& other) noexcept
	{
		// possibly clean myself up
		NotifyShaderAboutDeletion();

		// copy data
		m_shader = other.m_shader;
		m_func = other.m_func;
		m_usage_mode = other.m_usage_mode;
		
		// inform the shader about the new UniformSetter
		NotifyShaderAboutCreation();
		return *this;
	}

	UniformSetter::~UniformSetter()
	{
		NotifyShaderAboutDeletion();
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
