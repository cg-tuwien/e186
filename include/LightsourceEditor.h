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

		/*! On first call, or if anything has changed, uploads the lightsource data to 
		 *	a Uniform Buffer on the GPU.
		 *	@param force_gpu_upload Force Uniform Buffer update
		 */
		void Update(bool force_gpu_upload);

		void BindUniformBufferToShaderLocations();

	private:
		static void TW_CALL SetLightColorForAllCallback(const void *value, void *clientData);
		static void TW_CALL SetConstAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL SetLinearAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL SetQuadraticAttenuationForAllCallback(const void *value, void *clientData);
		static void TW_CALL SetCubicAttenuationForAllCallback(const void *value, void *clientData);
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
		float m_inner_transparency;
		float m_outer_transparency;
		float m_inner_radius;
		glm::vec3 m_inner_color;
		glm::vec3 m_outer_color;
		bool m_has_changes;
		GLuint m_uniform_buffer_handle;
	};
}
