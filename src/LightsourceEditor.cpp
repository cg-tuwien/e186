#include "LightsourceEditor.h"
#include <glad/glad.h>

namespace e186
{
	PointLightGpuData::PointLightGpuData(glm::vec4 pos, glm::vec4 col, glm::vec4 atten)
		: m_position(pos),
		m_light_color(col),
		m_attenuation(atten)
	{
		
	}

	PointLightGpuData::PointLightGpuData(glm::vec3 pos, glm::vec3 col, glm::vec3 atten)
		: m_position(pos, 0.0f),
		m_light_color(col, 0.0f),
		m_attenuation(atten, 0.0f)
	{

	}



	void TW_CALL LightsourceEditor::EnableAllCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_enabled(true);
		}
		thiz->m_has_changes = true;
	}

	void TW_CALL LightsourceEditor::DisableAllCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_enabled(false);
		}
		thiz->m_has_changes = true;
	}

	void TW_CALL LightsourceEditor::SetPositionCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_position(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetPositionCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = ptlt->position();
	}

	void TW_CALL LightsourceEditor::SetLightColorCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetLightColorCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = ptlt->light_color();
	}

	void TW_CALL LightsourceEditor::SetConstAttenuationCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_const_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetConstAttenuationCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->const_attenuation();
	}

	void TW_CALL LightsourceEditor::SetLinearAttenuationCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_linear_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetLinearAttenuationCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->linear_attenuation();
	}

	void TW_CALL LightsourceEditor::SetQuadraticAttenuationCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetQuadraticAttenuationCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->quadratic_attenuation();
	}

	void TW_CALL LightsourceEditor::SetEnabledCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_enabled(*reinterpret_cast<const bool*>(value));
	}

	void TW_CALL LightsourceEditor::GetEnabledCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<bool*>(value) = ptlt->enabled();
	}


	LightsourceEditor::LightsourceEditor() :
		m_gizmo_shader(),
		m_point_lights(),
		m_tweak_bar(Engine::current->tweak_bar_manager().create_new_tweak_bar("Lightsource Gizmos")),
		m_inner_transparency(.3f),
		m_outer_transparency(.07f),
		m_inner_radius(0.1f), // 10cm
		m_inner_color(1.0f, 1.0f, 0.5f),
		m_outer_color(1.0f, 1.0f, 0.85f),
		m_has_changes(true),
		m_uniform_buffer_handle(0)
	{
		m_sphere = Model::LoadFromFile("assets/models/sphere.obj", glm::mat4(1.0f), MOLF_triangulate | MOLF_smoothNormals);
		m_gizmo_shader.AddVertexShaderSourceFromFile("assets/shaders/translucent_gizmo.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/translucent_gizmo.frag", { std::make_tuple(0, "oFragColor") })
			.Build();
		m_point_lights.reserve(kMaxPointLights);

		TwAddButton(m_tweak_bar, "Enable all", EnableAllCallback, this, nullptr);
		TwAddButton(m_tweak_bar, "Disable all", DisableAllCallback, this, nullptr);
		TwAddVarRW(m_tweak_bar, "Transparency inner", TW_TYPE_FLOAT, &m_inner_transparency, " min=0.01 max=1.0 step=0.01 ");
		TwAddVarRW(m_tweak_bar, "Transparency outer", TW_TYPE_FLOAT, &m_outer_transparency, " min=0.01 max=1.0 step=0.01 ");
		TwAddVarRW(m_tweak_bar, "Inner radius", TW_TYPE_FLOAT, &m_inner_radius, " min=0.0 max=3.0 step=0.1 ");
		TwAddVarRW(m_tweak_bar, "Color inner", TW_TYPE_COLOR3F, glm::value_ptr(m_inner_color), nullptr);
		TwAddVarRW(m_tweak_bar, "Color outer", TW_TYPE_COLOR3F, glm::value_ptr(m_outer_color), nullptr);

		// create the uniform buffer right away
		glGenBuffers(1, &m_uniform_buffer_handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_uniform_buffer_handle);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	LightsourceEditor::~LightsourceEditor()
	{
	}

	void LightsourceEditor::Add(PointLight* point_light)
	{
		auto n = m_point_lights.size();
		if (n >= kMaxPointLights)
		{
			log_error("Max. number of supported point lights reached.");
			return;
		}

		m_point_lights.push_back(point_light);

		auto name = "PL #" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarCB(m_tweak_bar, (name + " enabled").c_str(), TW_TYPE_BOOLCPP, SetEnabledCallback, GetEnabledCallback, point_light, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " pos").c_str(), TW_TYPE_DIR3F, SetPositionCallback, GetPositionCallback, point_light, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " light-col").c_str(), TW_TYPE_COLOR3F, SetLightColorCallback, GetLightColorCallback, point_light, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " const-att").c_str(), TW_TYPE_FLOAT, SetConstAttenuationCallback, GetConstAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " lin-att").c_str(), TW_TYPE_FLOAT, SetLinearAttenuationCallback, GetLinearAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " quad-att").c_str(), TW_TYPE_FLOAT, SetQuadraticAttenuationCallback, GetQuadraticAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
	}

	void LightsourceEditor::Clear()
	{
		m_point_lights.clear();
	}

	void LightsourceEditor::RenderGizmos(Camera& camera)
	{
		auto pM = camera.projection_matrix();
		auto vM = camera.CalculateViewMatrix();

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_gizmo_shader.Use();
		auto mat_loc = m_gizmo_shader.GetUniformLocation("pvmtMatrix");
		auto col_loc = m_gizmo_shader.GetUniformLocation("uColor");
		auto& sphere_mesh = m_sphere->mesh_at(0);

		glm::vec4 inner_color(m_inner_color.r, m_inner_color.g, m_inner_color.b, m_inner_transparency);
		glm::vec4 outer_color(m_outer_color.r, m_outer_color.g, m_outer_color.b, m_outer_transparency);

		auto n = m_point_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			auto* ptlt = m_point_lights[i];

			if (!ptlt->enabled())
				continue;

			auto mM = glm::translate(ptlt->position()); // m_point_lights[i].get().transform().GetModelMatrix();
			auto tM = glm::scale(glm::vec3(m_inner_radius, m_inner_radius, m_inner_radius));
			m_gizmo_shader.SetUniform(mat_loc, pM * vM * mM * tM);
			m_gizmo_shader.SetUniform(col_loc, inner_color);
			RenderMesh(m_gizmo_shader, sphere_mesh);

			auto ca = ptlt->const_attenuation();
			auto la = ptlt->linear_attenuation();
			auto qa = ptlt->quadratic_attenuation();
			// Calculate the distance where the light contribution is as low as 0.025 a.k.a. 1/40
			auto faded_dist = qa < 0.001f || la < 0.001 ? 1.0f : (-la + sqrt(la * la - 4 * qa * (ca - 40))) / (2 * qa);
			tM = glm::scale(glm::vec3(faded_dist, faded_dist, faded_dist));
			m_gizmo_shader.SetUniform(mat_loc, pM * vM * mM * tM);
			m_gizmo_shader.SetUniform(col_loc, outer_color);
			RenderMesh(m_gizmo_shader, sphere_mesh);
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}

	void LightsourceEditor::Update(bool force_gpu_upload)
	{
		if (!m_has_changes && !force_gpu_upload)
			return;

		std::vector<PointLightGpuData> gpu_data;
		for (auto& cpu_data : m_point_lights)
		{
			if (cpu_data->enabled())
			{
				gpu_data.emplace_back(cpu_data->position(), cpu_data->light_color(), cpu_data->attenuation());
			}
		}

		auto n = gpu_data.size();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_uniform_buffer_handle);
		// fill the buffer, you do this anytime any of the lights change
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLightGpuData) * n, gpu_data.size() > 0 ? &gpu_data[0] : 0, GL_DYNAMIC_DRAW);
	}

	void LightsourceEditor::BindUniformBufferToShaderLocations()
	{
		// binding to index 0, the glsl code needs to specify this same
		// index using 'layout(binding=0)'
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_uniform_buffer_handle);
		// Credits: https://www.gamedev.net/forums/topic/658486-glsl-send-array-receive-struct/ (Happy Coder)
	}
}
