#pragma once

namespace e186
{
	class LightsourceEditor
	{
		const static int kMaxPointLights = 1024;
		
	public:
		LightsourceEditor();
		~LightsourceEditor();

		void Add(PointLight* point_light);
		void Clear();

		template<typename T>
		void AddAll(T collection)
		{
			for (auto& x : collection)
			{
				Add(&x);
			}
		}

		void RenderGizmos(Camera& camera);

	private:
		static void TW_CALL EnableAllCallback(void *clientData);
		static void TW_CALL DisableAllCallback(void *clientData);
		static void TW_CALL SetPositionCallback(const void *value, void *clientData);
		static void TW_CALL GetPositionCallback(void *value, void *clientData);
		static void TW_CALL SetDiffuseColorCallback(const void *value, void *clientData);
		static void TW_CALL GetDiffuseColorCallback(void *value, void *clientData);
		static void TW_CALL SetSpecularColorCallback(const void *value, void *clientData);
		static void TW_CALL GetSpecularColorCallback(void *value, void *clientData);
		static void TW_CALL SetConstAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetConstAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetLinearAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetLinearAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetQuadraticAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetQuadraticAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetEnabledCallback(const void *value, void *clientData);
		static void TW_CALL GetEnabledCallback(void *value, void *clientData);

	private:
		std::unique_ptr<Model> m_sphere;
		Shader m_gizmo_shader;
		std::vector<PointLight*> m_point_lights;
		AntTweakBarHandle m_tweak_bar;
		float m_inner_transparency;
		float m_outer_transparency;
		float m_inner_radius;
		glm::vec3 m_inner_color;
		glm::vec3 m_outer_color;
	};
}
