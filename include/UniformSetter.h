#pragma once

namespace e186
{
	class Shader;
	class MaterialData;
	
	class UniformSetter
	{
		friend class Shader;

	public:
		UniformSetter(Shader&, const std::shared_ptr<MaterialData>&, UniformSetterFunc);
		UniformSetter(UniformSetter&&);
		UniformSetter(const UniformSetter&);
		UniformSetter& operator= (UniformSetter&&);
		UniformSetter& operator= (const UniformSetter&);
		~UniformSetter();

		void operator()(const MaterialData&) const;

		void UpdateMaterialData(const std::shared_ptr<MaterialData>& mat_data);
		void ShaderUpdated(Shader& new_shader);

		Shader* shader() const { return m_shader; }

	private:
		Shader* m_shader;
		std::shared_ptr<MaterialData> m_material_data;
		UniformSetterFunc m_func;

		
	};

}
