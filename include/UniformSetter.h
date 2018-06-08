#pragma once

namespace e186
{
	class Shader;
	class MaterialData;
	
	/*! Indicate how UniformSetters are handling changing material properties. 
	 *	'Static' means that UniformSetters will only upload the material 
	 *		properties which were available during UniformSetter creation.
	 *		I.e., e.g.: if a texture is added during run-time, 'Static' 
	 *		mode might not upload the new texture. Changes in color values
	 *		and suchlike, however, will be uploaded correctly.
	 *	'Dynamic' means that UniformSetters will handle all material property
	 *		updates whatsoever, at the cost of more if statements at run-time.
	 */
	enum struct UniformSetterUsageMode : uint32_t
	{
		Static,
		Dynamic
	};

	/*! Actual worker function of a UniformSetter which uploads one 
	 *	specific material property to the shader. A reference to *this
	 *	is passed to the UniformSubSetterFunc in order to enable self-updates
	 *	of the worker functions (this is used in UniformSetterUsageMode::Static
	 *	to replace the function with an optimized one).
	 */
	class UniformSubSetter
	{
	public:
		UniformSubSetter() noexcept;
		UniformSubSetter(UniformSubSetterFunc) noexcept;
		UniformSubSetter(UniformSubSetter&&) noexcept;
		UniformSubSetter(const UniformSubSetter&) noexcept;
		UniformSubSetter& operator= (UniformSubSetter&&) noexcept;
		UniformSubSetter& operator= (const UniformSubSetter&);
		~UniformSubSetter();
		void operator()(const Shader&, const MaterialData&);
		void set_func(UniformSubSetterFunc);
	private:
		UniformSubSetterFunc m_func;
	};

	/*! Uploads material data to a shader. */
	class UniformSetter
	{
		friend class Shader;

	public:
		UniformSetter() noexcept;
		UniformSetter(Shader&, UniformSetterFunc, UniformSetterUsageMode) noexcept;
		UniformSetter(UniformSetter&&) noexcept;
		UniformSetter(const UniformSetter&) noexcept;
		UniformSetter& operator= (UniformSetter&&) noexcept;
		UniformSetter& operator= (const UniformSetter&) noexcept;
		~UniformSetter();

		void operator()(const MaterialData&);
		void operator()(const Shader&, const MaterialData&);

		void ShaderUpdated(Shader& new_shader);

		Shader* shader() const { return m_shader; }

		bool is_valid() const { return nullptr != m_shader; }

	private:
		Shader* m_shader;
		UniformSetterFunc m_func;
		UniformSetterUsageMode m_usage_mode;
	};

	/*! Basically, a collection of tuples of Meshes and associated UniformSetters. */
	struct MeshUniformSettersForShader
	{
		GLuint m_shader_handle;
		std::vector<std::tuple<MeshRef, UniformSetter>> m_mesh_uniform_setters;
		MeshUniformSettersForShader() : m_shader_handle{ 0 }, m_mesh_uniform_setters{} {}
		auto num_meshes() const { return m_mesh_uniform_setters.size(); }
	};
}
