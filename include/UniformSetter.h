#pragma once

namespace e186
{
	class Shader;
	class MaterialData;
	
	class UniformSubSetter
	{
	public:
		UniformSubSetter();
		UniformSubSetter(UniformSubSetterFunc);
		UniformSubSetter(UniformSubSetter&&);
		UniformSubSetter(const UniformSubSetter&);
		UniformSubSetter& operator= (UniformSubSetter&&);
		UniformSubSetter& operator= (const UniformSubSetter&);
		~UniformSubSetter();
		void operator()(const Shader&, const MaterialData&);
		void set_func(UniformSubSetterFunc);
	private:
		UniformSubSetterFunc m_func;
	};

	class UniformSetter
	{
		friend class Shader;

	public:
		UniformSetter();
		UniformSetter(Shader&, UniformSetterFunc);
		UniformSetter(UniformSetter&&);
		UniformSetter(const UniformSetter&);
		UniformSetter& operator= (UniformSetter&&);
		UniformSetter& operator= (const UniformSetter&);
		~UniformSetter();

		void operator()(const MaterialData&);
		void operator()(const Shader&, const MaterialData&);

		void ShaderUpdated(Shader& new_shader);

		Shader* shader() const { return m_shader; }

	private:
		Shader* m_shader;
		UniformSetterFunc m_func;
	};

	struct MeshUniformSettersForShader
	{
		GLuint m_shader_handle;
		std::vector<std::tuple<MeshRef, UniformSetter>> m_mesh_uniform_setters;
		MeshUniformSettersForShader() : m_shader_handle{ 0 }, m_mesh_uniform_setters{} {}
		auto num_meshes() const { return m_mesh_uniform_setters.size(); }
	};
}
