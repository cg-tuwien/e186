#include "UniformSetter.h"

namespace e186
{

	UniformSubSetter::UniformSubSetter() : m_func()
	{
	}

	UniformSubSetter::UniformSubSetter(UniformSubSetterFunc func) : m_func(std::move(func))
	{
	}

	UniformSubSetter::UniformSubSetter(UniformSubSetter&& other) : m_func(std::move(other.m_func))
	{
	}

	UniformSubSetter::UniformSubSetter(const UniformSubSetter& other) : m_func(other.m_func)
	{
	}

	UniformSubSetter& UniformSubSetter::operator= (UniformSubSetter&& other)
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

	UniformSetter::UniformSetter() 
		: m_shader(nullptr),
		m_func()
	{
	}

	UniformSetter::UniformSetter(Shader& shader, UniformSetterFunc func)
		: m_shader(&shader),
		m_func(std::move(func))
	{
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter::UniformSetter(UniformSetter&& other)
		: m_shader(std::move(other.m_shader)),
		m_func(std::move(other.m_func))
	{
		other.m_shader = nullptr;
		m_shader->HandleUniformSetterMoved(&other, this);
	}

	UniformSetter::UniformSetter(const UniformSetter& other)
		: m_shader(other.m_shader),
		m_func(other.m_func)
	{
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter& UniformSetter::operator= (UniformSetter&& other)
	{
		other.m_shader = nullptr;
		m_shader = std::move(other.m_shader);
		m_func = std::move(other.m_func);
		m_shader->HandleUniformSetterMoved(&other, this);
		return *this;
	}

	UniformSetter& UniformSetter::operator= (const UniformSetter& other)
	{
		m_shader = other.m_shader;
		m_func = other.m_func;
		return *this;
	}

	UniformSetter::~UniformSetter()
	{
		if (nullptr != m_shader)
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
		*this = CreateUniformSetterForShader(new_shader); // TODO: uniform setter mode static/dynamic
	}
}
