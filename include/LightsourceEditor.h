#pragma once

namespace e186
{
	struct PointLightGpuData
	{
		glm::vec4 m_position;
		glm::vec4 m_light_color;
		glm::vec4 m_attenuation;
	};

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
		static void TW_CALL SetLightColorForAllCallback(const void *value, void *clientData);
		static void TW_CALL GetLightColorForAllCallback(void *value, void *clientData);
		static void TW_CALL SetConstAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL GetConstAttenuationForAllCallback(void *value, void *clientData);
		static void TW_CALL SetLinearAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL GetLinearAttenuationForAllCallback(void *value, void *clientData);
		static void TW_CALL SetQuadraticAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL GetQuadraticAttenuationForAllCallback(void *value, void *clientData);
		static void TW_CALL SetCubicAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL GetCubicAttenuationForAllCallback(void *value, void *clientData);
		static void TW_CALL EnableAllCallback(void *clientData);
		static void TW_CALL DisableAllCallback(void *clientData);

		static void TW_CALL SetPositionCallback(const void *value, void *clientData);
		static void TW_CALL GetPositionCallback(void *value, void *clientData);
		static void TW_CALL SetLightColorCallback(const void *value, void *clientData);
		static void TW_CALL GetLightColorCallback(void *value, void *clientData);
		static void TW_CALL SetConstAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetConstAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetLinearAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetLinearAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetQuadraticAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetQuadraticAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetCubicAttenuationCallback(const void *value, void *clientData);
		static void TW_CALL GetCubicAttenuationCallback(void *value, void *clientData);
		static void TW_CALL SetEnabledCallback(const void *value, void *clientData);
		static void TW_CALL GetEnabledCallback(void *value, void *clientData);

	private:
		std::unique_ptr<Model> m_sphere;
		Shader m_gizmo_shader;
		std::vector<PointLight*> m_point_lights;
		AntTweakBarHandle m_tweak_bar;
		float m_transparency;
		float m_gizmo_scale;
		float m_gizmo_param;
	};
}
