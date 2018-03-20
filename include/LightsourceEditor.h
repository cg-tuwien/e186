#pragma once

namespace e186
{


	class LightsourceEditor
	{
		const static int kMaxPointLights = 1024;
		
	public:
		LightsourceEditor();
		~LightsourceEditor();

		void Set(AmbientLight* ambient_light);
		void Set(DirectionalLight* directional_light);
		void Add(PointLight* point_light);
		void Add(PointLightWrapper point_light_wrapper);
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
		static void TW_CALL SetUniformPositionOffsetCallback(const void *value, void *clientData);
		static void TW_CALL GetUniformPositionOffsetCallback(void *value, void *clientData);

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

		static void TW_CALL SetDLDirectionCallback(const void *value, void *clientData);
		static void TW_CALL GetDLDirectionCallback(void *value, void *clientData);
		static void TW_CALL SetDLLightColorCallback(const void *value, void *clientData);
		static void TW_CALL GetDLLightColorCallback(void *value, void *clientData);
		static void TW_CALL SetDLEnabledCallback(const void *value, void *clientData);
		static void TW_CALL GetDLEnabledCallback(void *value, void *clientData);

		static void TW_CALL SetAmbientLightColorCallback(const void *value, void *clientData);
		static void TW_CALL GetAmbientLightColorCallback(void *value, void *clientData);
		static void TW_CALL SetAmbientEnabledCallback(const void *value, void *clientData);
		static void TW_CALL GetAmbientEnabledCallback(void *value, void *clientData);

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
		DirectionalLight* m_directional_light;
		AmbientLight* m_ambient_light;
		std::vector<PointLightWrapper> m_point_lights;
		std::vector<glm::vec3> m_point_lights_orig_pos;
		std::vector<std::tuple<LightsourceEditor*, size_t>> m_point_lights_tw_index_helper;
		AntTweakBarHandle m_tweak_bar;
		float m_transparency;
		float m_gizmo_scale;
		float m_gizmo_param;
		glm::vec3 m_uniform_position_offset;
	};
}
