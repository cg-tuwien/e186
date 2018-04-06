#pragma once

namespace e186
{


	class LightsourceEditor
	{
		const static int kMaxPointLights = 512;
		const static int kMaxSpotLights = 512;
		
	public:
		LightsourceEditor();
		~LightsourceEditor();

		void Set(AmbientLight* ambient_light);
		void Set(DirectionalLight* directional_light);
		void Add(PointLight* point_light);
		void Add(PointLightWrapper point_light_wrapper);
		void Add(SpotLight* spot_light);
		void Add(SpotLightWrapper spot_light_wrapper);
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
		static void TW_CALL EnableAllPointLightsCallback(void *clientData);
		static void TW_CALL DisableAllPointLightsCallback(void *clientData);
		static void TW_CALL SetPosOffsetPlCB(const void *value, void *clientData);
		static void TW_CALL GetPosOffsetPlCB(void *value, void *clientData);
		static void TW_CALL SetAllLightColorPlCB(const void *value, void *clientData);
		static void TW_CALL GetAllLightColorPlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt0PlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt0PlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt1PlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt1PlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt2PlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt2PlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt3PlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt3PlCB(void *value, void *clientData);

		static void TW_CALL EnableAllSpotLightsCallback(void *clientData);
		static void TW_CALL DisableAllSpotLightsCallback(void *clientData);
		static void TW_CALL SetPosOffsetSlCB(const void *value, void *clientData);
		static void TW_CALL GetPosOffsetSlCB(void *value, void *clientData);
		static void TW_CALL SetDirOffsetSlCB(const void *value, void *clientData);
		static void TW_CALL GetDirOffsetSlCB(void *value, void *clientData);
		static void TW_CALL SetAllLightColorSlCB(const void *value, void *clientData);
		static void TW_CALL GetAllLightColorSlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt0SlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt0SlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt1SlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt1SlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt2SlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt2SlCB(void *value, void *clientData);
		static void TW_CALL SetAllAtt3SlCB(const void *value, void *clientData);
		static void TW_CALL GetAllAtt3SlCB(void *value, void *clientData);

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

		static void TW_CALL SetEnabledPlCB(const void *value, void *clientData);
		static void TW_CALL GetEnabledPlCB(void *value, void *clientData);
		static void TW_CALL SetPosPlCB(const void *value, void *clientData);
		static void TW_CALL GetPosPlCB(void *value, void *clientData);
		static void TW_CALL SetLightColorPlCB(const void *value, void *clientData);
		static void TW_CALL GetLightColorPlCB(void *value, void *clientData);
		static void TW_CALL SetAtt0PlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt0PlCB(void *value, void *clientData);
		static void TW_CALL SetAtt1PlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt1PlCB(void *value, void *clientData);
		static void TW_CALL SetAtt2PlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt2PlCB(void *value, void *clientData);
		static void TW_CALL SetAtt3PlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt3PlCB(void *value, void *clientData);

		static void TW_CALL SetEnabledSlCB(const void *value, void *clientData);
		static void TW_CALL GetEnabledSlCB(void *value, void *clientData);
		static void TW_CALL SetPosSlCB(const void *value, void *clientData);
		static void TW_CALL GetPosSlCB(void *value, void *clientData);
		static void TW_CALL SetDirSlCB(const void *value, void *clientData);
		static void TW_CALL GetDirSlCB(void *value, void *clientData);
		static void TW_CALL SetLightColorSlCB(const void *value, void *clientData);
		static void TW_CALL GetLightColorSlCB(void *value, void *clientData);
		static void TW_CALL SetAtt0SlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt0SlCB(void *value, void *clientData);
		static void TW_CALL SetAtt1SlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt1SlCB(void *value, void *clientData);
		static void TW_CALL SetAtt2SlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt2SlCB(void *value, void *clientData);
		static void TW_CALL SetAtt3SlCB(const void *value, void *clientData);
		static void TW_CALL GetAtt3SlCB(void *value, void *clientData);
		static void TW_CALL SetInnerAngleSlCB(const void *value, void *clientData);
		static void TW_CALL GetInnerAngleSlCB(void *value, void *clientData);
		static void TW_CALL SetOuterAngleSlCB(const void *value, void *clientData);
		static void TW_CALL GetOuterAngleSlCB(void *value, void *clientData);
		static void TW_CALL SetFalloffSlCB(const void *value, void *clientData);
		static void TW_CALL GetFalloffSlCB(void *value, void *clientData);

	private:
		std::unique_ptr<Model> m_sphere;
		std::unique_ptr<Model> m_cone;
		Shader m_gizmo_shader;
		DirectionalLight* m_directional_light;
		AmbientLight* m_ambient_light;
		std::vector<PointLightWrapper> m_point_lights;
		std::vector<glm::vec3> m_point_lights_orig_pos;
		std::vector<std::tuple<LightsourceEditor*, size_t>> m_point_lights_tw_index_helper;
		std::vector<SpotLightWrapper> m_spot_lights;
		std::vector<glm::vec3> m_spot_lights_orig_pos;
		std::vector<glm::vec3> m_spot_lights_orig_dir;
		std::vector<std::tuple<LightsourceEditor*, size_t>> m_spot_lights_tw_index_helper;
		AntTweakBarHandle m_tweak_bar;
		float m_transparency;
		float m_gizmo_scale;
		float m_gizmo_param;
		glm::vec3 m_uniform_position_offset_point_light;
		glm::vec3 m_uniform_position_offset_spot_light;
		glm::vec3 m_uniform_direction_offset_spot_light;
	};
}
