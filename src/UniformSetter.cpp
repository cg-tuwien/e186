#include "UniformSetter.h"

namespace e186
{
	UniformSetter::UniformSetter(Shader& shader, const std::shared_ptr<MaterialData>& mat_data, UniformSetterFunc func)
		: m_shader(&shader),
		m_material_data(mat_data),
		m_func(func)
	{
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter::UniformSetter(UniformSetter&& other)
		: m_shader(std::move(other.m_shader)),
		m_material_data(std::move(other.m_material_data)),
		m_func(std::move(other.m_func))
	{
		m_shader->HandleUniformSetterMoved(&other, this);
	}

	UniformSetter::UniformSetter(const UniformSetter& other)
		: m_shader(other.m_shader),
		m_material_data(other.m_material_data),
		m_func(other.m_func)
	{
		m_shader->HandleUniformSetterCreated(this);
	}

	UniformSetter& UniformSetter::operator= (UniformSetter&& other)
	{
		m_shader = std::move(other.m_shader);
		m_material_data = std::move(other.m_material_data);
		m_func = std::move(other.m_func);
		m_shader->HandleUniformSetterMoved(&other, this);
		return *this;
	}

	UniformSetter& UniformSetter::operator= (const UniformSetter& other)
	{
		m_shader = other.m_shader;
		m_material_data = other.m_material_data;
		m_func = other.m_func;
		return *this;
	}

	UniformSetter::~UniformSetter()
	{
		m_shader->HandleUniformSetterDeleted(this);
	}

	void UniformSetter::operator()(const MaterialData&) const
	{

	}

	void UniformSetter::UpdateMaterialData(const std::shared_ptr<MaterialData>& mat_data)
	{
		*this = CreateUniformSetterForShader(*m_shader, mat_data);
	}

	void UniformSetter::ShaderUpdated(Shader& new_shader)
	{
		*this = CreateUniformSetterForShader(new_shader, m_material_data);
	}
}
