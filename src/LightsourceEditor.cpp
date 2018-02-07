#include "LightsourceEditor.h"
#include <glad/glad.h>

namespace e186
{
	void TW_CALL LightsourceEditor::GetLightColorForAllCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<glm::vec3*>(value) = thiz->m_point_lights[0]->light_color();
		}
		else
		{
			*reinterpret_cast<glm::vec3*>(value) = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	void TW_CALL LightsourceEditor::GetConstAttenuationForAllCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0]->const_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetLinearAttenuationForAllCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0]->linear_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetQuadraticAttenuationForAllCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0]->quadratic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetCubicAttenuationForAllCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0]->cubic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetUniformPositionOffsetCallback(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_uniform_position_offset;
	}

	void TW_CALL LightsourceEditor::SetUniformPositionOffsetCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		thiz->m_uniform_position_offset = *reinterpret_cast<const glm::vec3*>(value);
		// Update all lights' positions
		auto n = thiz->m_point_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			thiz->m_point_lights[i]->set_position(thiz->m_point_lights_orig_pos[i] + thiz->m_uniform_position_offset);
		}
	}

	void TW_CALL LightsourceEditor::SetLightColorForAllCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetConstAttenuationForAllCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_const_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetLinearAttenuationForAllCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_linear_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetQuadraticAttenuationForAllCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetCubicAttenuationForAllCallback(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_cubic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::EnableAllCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_enabled(true);
		}
	}

	void TW_CALL LightsourceEditor::DisableAllCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto* ptlt : thiz->m_point_lights)
		{
			ptlt->set_enabled(false);
		}
	}

	void TW_CALL LightsourceEditor::SetPositionCallback(const void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		thiz->m_point_lights[index]->set_position(*reinterpret_cast<const glm::vec3*>(value));
		thiz->m_point_lights_orig_pos[index] = thiz->m_point_lights[index]->position();
	}

	void TW_CALL LightsourceEditor::GetPositionCallback(void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_point_lights[index]->position();
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

	void TW_CALL LightsourceEditor::SetCubicAttenuationCallback(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		ptlt->set_cubic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetCubicAttenuationCallback(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLight*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->cubic_attenuation();
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
		m_point_lights_orig_pos(),
		m_point_lights_tw_index_helper(),
		m_tweak_bar(Engine::current->tweak_bar_manager().create_new_tweak_bar("Lightsource Gizmos")),
		m_transparency(.3f),
		m_gizmo_scale(40.0f),
		m_gizmo_param(4.0f),
		m_uniform_position_offset(0.0f, 0.0f, 0.0f)
	{
		m_sphere = Model::LoadFromFile("assets/models/sphere.obj", glm::mat4(1.0f), MOLF_triangulate | MOLF_smoothNormals);
		m_gizmo_shader.AddVertexShaderSourceFromFile("assets/shaders/translucent_gizmo.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/translucent_gizmo.frag", { std::make_tuple(0, "oFragColor") })
			.Build();
		m_point_lights.reserve(kMaxPointLights);
		m_point_lights_orig_pos.reserve(kMaxPointLights);
		m_point_lights_tw_index_helper.reserve(kMaxPointLights);

		TwAddButton(m_tweak_bar, "Enable all", EnableAllCallback, this, nullptr);
		TwAddButton(m_tweak_bar, "Disable all", DisableAllCallback, this, nullptr);
		TwAddVarCB(m_tweak_bar, "Offset", TW_TYPE_DIR3F, SetUniformPositionOffsetCallback, GetUniformPositionOffsetCallback, this, nullptr);
		TwAddVarCB(m_tweak_bar, "Set light color for all", TW_TYPE_COLOR3F, SetLightColorForAllCallback, GetLightColorForAllCallback, this, nullptr);
		TwAddVarCB(m_tweak_bar, "Set const-atten for all", TW_TYPE_FLOAT, SetConstAttenuationForAllCallback, GetConstAttenuationForAllCallback, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "Set lin-atten for all", TW_TYPE_FLOAT, SetLinearAttenuationForAllCallback, GetLinearAttenuationForAllCallback, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "Set quad-atten for all", TW_TYPE_FLOAT, SetQuadraticAttenuationForAllCallback, GetQuadraticAttenuationForAllCallback, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "Set cub-atten for all", TW_TYPE_FLOAT, SetCubicAttenuationForAllCallback, GetCubicAttenuationForAllCallback, this, " min=0.0 step=0.01 ");
		TwAddVarRW(m_tweak_bar, "Gizmo Transparency", TW_TYPE_FLOAT, &m_transparency, " min=0.01 max=1.0 step=0.01 ");
		TwAddVarRW(m_tweak_bar, "Gizmo Scale", TW_TYPE_FLOAT, &m_gizmo_scale, " min=0.0 max=100.0 step=0.1 ");
		TwAddVarRW(m_tweak_bar, "Gizmo Param", TW_TYPE_FLOAT, &m_gizmo_param, " min=0.0 max=100.0 step=0.1 ");
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

		auto insert_index = n;
		m_point_lights.push_back(point_light);
		m_point_lights_orig_pos.push_back(point_light->position());
		m_point_lights[insert_index]->set_position(m_point_lights_orig_pos[insert_index] + m_uniform_position_offset);
		m_point_lights_tw_index_helper.push_back(std::make_tuple(this, insert_index));
		assert(m_point_lights.size() == m_point_lights_orig_pos.size());
		assert(m_point_lights.size() == m_point_lights_tw_index_helper.size());
		assert(m_point_lights.size() == insert_index + 1);

		auto name = "PL #" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarCB(m_tweak_bar, (name + " enabled").c_str(), TW_TYPE_BOOLCPP, SetEnabledCallback, GetEnabledCallback, point_light, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " pos").c_str(), TW_TYPE_DIR3F, SetPositionCallback, GetPositionCallback, &m_point_lights_tw_index_helper[insert_index], groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " light-col").c_str(), TW_TYPE_COLOR3F, SetLightColorCallback, GetLightColorCallback, point_light, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " const-att").c_str(), TW_TYPE_FLOAT, SetConstAttenuationCallback, GetConstAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " linear-att").c_str(), TW_TYPE_FLOAT, SetLinearAttenuationCallback, GetLinearAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " quadr-att").c_str(), TW_TYPE_FLOAT, SetQuadraticAttenuationCallback, GetQuadraticAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " cubic-att").c_str(), TW_TYPE_FLOAT, SetCubicAttenuationCallback, GetCubicAttenuationCallback, point_light, (" min=0.0 step=0.01 " + groupAssignment).c_str());
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

		auto n = m_point_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			auto* ptlt = m_point_lights[i];

			if (!ptlt->enabled())
				continue;

			auto a = ptlt->attenuation();
			auto d = m_gizmo_param;
			auto s = m_gizmo_scale / (a[0] + a[1] * d + a[2] * d * d + a[3] * d * d * d);

			auto mM = glm::translate(ptlt->position()); // m_point_lights[i].get().transform().GetModelMatrix();
			auto tM = glm::scale(glm::vec3(s, s, s));
			m_gizmo_shader.SetUniform(mat_loc, pM * vM * mM * tM);
			m_gizmo_shader.SetUniform(col_loc, glm::vec4(ptlt->light_color(), m_transparency));
			RenderMesh(m_gizmo_shader, sphere_mesh);
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}

}
