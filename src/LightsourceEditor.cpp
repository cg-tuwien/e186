#include "LightsourceEditor.h"
#include <glad/glad.h>
#include "math_utils.h"

namespace e186
{
	void TW_CALL LightsourceEditor::GetAllLightColorPlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<glm::vec3*>(value) = thiz->m_point_lights[0].light_color();
		}
		else
		{
			*reinterpret_cast<glm::vec3*>(value) = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt0PlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0].const_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt1PlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0].linear_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt2PlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0].quadratic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt3PlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_point_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_point_lights[0].cubic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::GetPosOffsetPlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_uniform_position_offset_point_light;
	}

	void TW_CALL LightsourceEditor::SetPosOffsetPlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		thiz->m_uniform_position_offset_point_light = *reinterpret_cast<const glm::vec3*>(value);
		// Update all lights' positions
		auto n = thiz->m_point_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			thiz->m_point_lights[i].set_position(thiz->m_point_lights_orig_pos[i] + thiz->m_uniform_position_offset_point_light);
		}
	}

	void TW_CALL LightsourceEditor::SetAllLightColorPlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_light_color(*reinterpret_cast<const glm::vec3*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt0PlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_const_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt1PlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_linear_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt2PlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt3PlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_cubic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::EnableAllPointLightsCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_enabled(true);
		}
	}

	void TW_CALL LightsourceEditor::DisableAllPointLightsCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& ptlt : thiz->m_point_lights)
		{
			ptlt.set_enabled(false);
		}
	}



	void TW_CALL LightsourceEditor::EnableAllSpotLightsCallback(void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_enabled(true);
		}
	}

	void TW_CALL LightsourceEditor::DisableAllSpotLightsCallback(void *clientData) 
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_enabled(false);
		}
	}

	void TW_CALL LightsourceEditor::SetPosOffsetSlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		thiz->m_uniform_position_offset_spot_light = *reinterpret_cast<const glm::vec3*>(value);
		// Update all lights' positions
		auto n = thiz->m_spot_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			thiz->m_spot_lights[i].set_position(thiz->m_spot_lights_orig_pos[i] + thiz->m_uniform_position_offset_spot_light);
		}
	}

	void TW_CALL LightsourceEditor::GetPosOffsetSlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_uniform_position_offset_point_light;
	}

	void TW_CALL LightsourceEditor::SetDirOffsetSlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		thiz->m_uniform_direction_offset_spot_light = *reinterpret_cast<const glm::vec3*>(value);
		auto rotation = rotate_vector_a_to_vector_b(glm::vec3(0, 0, 1), glm::normalize(thiz->m_uniform_direction_offset_spot_light));
		// Update all lights' directions
		auto n = thiz->m_spot_lights.size();
		for (auto i = 0; i < n; ++i)
		{
			thiz->m_spot_lights[i].set_direction(glm::mat3(rotation) * thiz->m_spot_lights_orig_dir[i]);
		}
	}

	void TW_CALL LightsourceEditor::GetDirOffsetSlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_uniform_direction_offset_spot_light;
	}

	void TW_CALL LightsourceEditor::SetAllLightColorSlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_light_color(*reinterpret_cast<const glm::vec3*>(value));
		}
	}

	void TW_CALL LightsourceEditor::GetAllLightColorSlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_spot_lights.size() > 0)
		{
			*reinterpret_cast<glm::vec3*>(value) = thiz->m_spot_lights[0].light_color();
		}
		else
		{
			*reinterpret_cast<glm::vec3*>(value) = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt0SlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_const_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt0SlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_spot_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_spot_lights[0].const_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt1SlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_linear_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt1SlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_spot_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_spot_lights[0].linear_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt2SlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt2SlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_spot_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_spot_lights[0].quadratic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}

	void TW_CALL LightsourceEditor::SetAllAtt3SlCB(const void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		for (auto& sptl : thiz->m_spot_lights)
		{
			sptl.set_cubic_attenuation(*reinterpret_cast<const float*>(value));
		}
	}

	void TW_CALL LightsourceEditor::GetAllAtt3SlCB(void *value, void *clientData)
	{
		auto* thiz = reinterpret_cast<LightsourceEditor*>(clientData);
		if (thiz->m_spot_lights.size() > 0)
		{
			*reinterpret_cast<float*>(value) = thiz->m_spot_lights[0].cubic_attenuation();
		}
		else
		{
			*reinterpret_cast<float*>(value) = 0.0f;
		}
	}


	void TW_CALL LightsourceEditor::SetDLDirectionCallback(const void * value, void * clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		dlt->set_light_direction(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetDLDirectionCallback(void * value, void * clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = dlt->light_direction();
	}

	void TW_CALL LightsourceEditor::SetDLLightColorCallback(const void *value, void *clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		dlt->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetDLLightColorCallback(void *value, void *clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = dlt->light_color();
	}

	void TW_CALL LightsourceEditor::SetDLEnabledCallback(const void *value, void *clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		dlt->set_enabled(*reinterpret_cast<const bool*>(value));
	}

	void TW_CALL LightsourceEditor::GetDLEnabledCallback(void *value, void *clientData)
	{
		auto* dlt = reinterpret_cast<DirectionalLight*>(clientData);
		*reinterpret_cast<bool*>(value) = dlt->enabled();
	}


	void TW_CALL LightsourceEditor::SetAmbientLightColorCallback(const void *value, void *clientData)
	{
		auto* alt = reinterpret_cast<AmbientLight*>(clientData);
		alt->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetAmbientLightColorCallback(void *value, void *clientData)
	{
		auto* alt = reinterpret_cast<AmbientLight*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = alt->light_color();
	}

	void TW_CALL LightsourceEditor::SetAmbientEnabledCallback(const void *value, void *clientData)
	{
		auto* alt = reinterpret_cast<AmbientLight*>(clientData);
		alt->set_enabled(*reinterpret_cast<const bool*>(value));
	}

	void TW_CALL LightsourceEditor::GetAmbientEnabledCallback(void *value, void *clientData)
	{
		auto* alt = reinterpret_cast<AmbientLight*>(clientData);
		*reinterpret_cast<bool*>(value) = alt->enabled();
	}



	void TW_CALL LightsourceEditor::SetPosPlCB(const void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		thiz->m_point_lights[index].set_position(*reinterpret_cast<const glm::vec3*>(value));
		thiz->m_point_lights_orig_pos[index] = thiz->m_point_lights[index].position();
	}

	void TW_CALL LightsourceEditor::GetPosPlCB(void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_point_lights[index].position();
	}

	void TW_CALL LightsourceEditor::SetLightColorPlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetLightColorPlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = ptlt->light_color();
	}

	void TW_CALL LightsourceEditor::SetAtt0PlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_const_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt0PlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->const_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt1PlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_linear_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt1PlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->linear_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt2PlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt2PlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->quadratic_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt3PlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_cubic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt3PlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = ptlt->cubic_attenuation();
	}

	void TW_CALL LightsourceEditor::SetEnabledPlCB(const void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		ptlt->set_enabled(*reinterpret_cast<const bool*>(value));
	}

	void TW_CALL LightsourceEditor::GetEnabledPlCB(void *value, void *clientData)
	{
		auto* ptlt = reinterpret_cast<PointLightWrapper*>(clientData);
		*reinterpret_cast<bool*>(value) = ptlt->enabled();
	}

	void TW_CALL LightsourceEditor::SetEnabledSlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_enabled(*reinterpret_cast<const bool*>(value));
	}

	void TW_CALL LightsourceEditor::GetEnabledSlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<bool*>(value) = sptl->enabled();
	}

	void TW_CALL LightsourceEditor::SetPosSlCB(const void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		thiz->m_spot_lights[index].set_position(*reinterpret_cast<const glm::vec3*>(value));
		thiz->m_spot_lights_orig_pos[index] = thiz->m_spot_lights[index].position();
	}

	void TW_CALL LightsourceEditor::GetPosSlCB(void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_spot_lights[index].position();
	}

	void TW_CALL LightsourceEditor::SetDirSlCB(const void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		thiz->m_spot_lights[index].set_direction(*reinterpret_cast<const glm::vec3*>(value));
		thiz->m_spot_lights_orig_dir[index] = thiz->m_spot_lights[index].direction();
	}

	void TW_CALL LightsourceEditor::GetDirSlCB(void *value, void *clientData)
	{
		auto* tpl = reinterpret_cast<std::tuple<LightsourceEditor*, size_t>*>(clientData);
		auto* thiz = reinterpret_cast<LightsourceEditor*>(std::get<0>(*tpl));
		auto index = std::get<1>(*tpl);
		*reinterpret_cast<glm::vec3*>(value) = thiz->m_spot_lights[index].direction();
	}

	void TW_CALL LightsourceEditor::SetLightColorSlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_light_color(*reinterpret_cast<const glm::vec3*>(value));
	}

	void TW_CALL LightsourceEditor::GetLightColorSlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<glm::vec3*>(value) = sptl->light_color();
	}

	void TW_CALL LightsourceEditor::SetAtt0SlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_const_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt0SlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->const_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt1SlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_linear_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt1SlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->linear_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt2SlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_quadratic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt2SlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->quadratic_attenuation();
	}

	void TW_CALL LightsourceEditor::SetAtt3SlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_cubic_attenuation(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetAtt3SlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->cubic_attenuation();
	}

	void TW_CALL LightsourceEditor::SetInnerAngleSlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_inner_angle(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetInnerAngleSlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->inner_angle();
	}

	void TW_CALL LightsourceEditor::SetOuterAngleSlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_outer_angle(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetOuterAngleSlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->outer_angle();
	}

	void TW_CALL LightsourceEditor::SetFalloffSlCB(const void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		sptl->set_falloff(*reinterpret_cast<const float*>(value));
	}

	void TW_CALL LightsourceEditor::GetFalloffSlCB(void *value, void *clientData)
	{
		auto* sptl = reinterpret_cast<SpotLightWrapper*>(clientData);
		*reinterpret_cast<float*>(value) = sptl->falloff();
	}

	LightsourceEditor::LightsourceEditor() :
		m_gizmo_shader(),
		m_directional_light(nullptr),
		m_ambient_light(nullptr),
		m_point_lights(),
		m_point_lights_orig_pos(),
		m_point_lights_tw_index_helper(),
		m_spot_lights(),
		m_spot_lights_orig_pos(),
		m_spot_lights_orig_dir(),
		m_spot_lights_tw_index_helper(),
		m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Lightsources")),
		m_transparency(.3f),
		m_gizmo_scale_ptl(8.0f),
		m_gizmo_scale_sptl(0.7f),
		m_gizmo_param_ptl(3.0f),
		m_gizmo_param_sptl(1.5f),
		m_uniform_position_offset_point_light(0.0f, 0.0f, 0.0f),
		m_uniform_position_offset_spot_light(0.0f, 0.0f, 0.0f),
		m_uniform_direction_offset_spot_light(0.0f, 0.0f, 0.0f)
	{
		m_sphere = Model::LoadFromFile("assets/models/sphere.obj", glm::mat4(1.0f), MOLF_triangulate | MOLF_smoothNormals);
		m_sphere->CreateAndUploadGpuData();

		m_cone = Model::LoadFromFile("assets/models/cone.obj", glm::mat4(1.0f), MOLF_triangulate | MOLF_smoothNormals);
		m_cone->CreateAndUploadGpuData();

		m_gizmo_shader.AddVertexShaderSourceFromFile("assets/shaders/translucent_gizmo.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/translucent_gizmo.frag", { std::make_tuple(0, "oFragColor") })
			.Build();

		m_point_lights.reserve(kMaxPointLights);
		m_point_lights_orig_pos.reserve(kMaxPointLights);
		m_point_lights_tw_index_helper.reserve(kMaxPointLights);

		m_spot_lights.reserve(kMaxSpotLights);
		m_spot_lights_orig_pos.reserve(kMaxSpotLights);
		m_spot_lights_orig_dir.reserve(kMaxSpotLights);
		m_spot_lights_tw_index_helper.reserve(kMaxSpotLights);

		TwDefine(" Lightsources color='65 65 65' text=light position='16 400' ");
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
		TwAddButton(m_tweak_bar, "Enable all point lights", EnableAllPointLightsCallback, this, nullptr);
		TwAddButton(m_tweak_bar, "Disable all point lights", DisableAllPointLightsCallback, this, nullptr);
		TwAddVarCB(m_tweak_bar, "Point lights: Pos. Offset", TW_TYPE_DIR3F, SetPosOffsetPlCB, GetPosOffsetPlCB, this, nullptr);
		TwAddVarCB(m_tweak_bar, "All point lights: light color", TW_TYPE_COLOR3F, SetAllLightColorPlCB, GetAllLightColorPlCB, this, nullptr);
		TwAddVarCB(m_tweak_bar, "All point lights: const-atten", TW_TYPE_FLOAT, SetAllAtt0PlCB, GetAllAtt0PlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All point lights: lin-atten", TW_TYPE_FLOAT, SetAllAtt1PlCB, GetAllAtt1PlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All point lights: quad-atten", TW_TYPE_FLOAT, SetAllAtt2PlCB, GetAllAtt2PlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All point lights: cub-atten", TW_TYPE_FLOAT, SetAllAtt3PlCB, GetAllAtt3PlCB, this, " min=0.0 step=0.01 ");
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
		TwAddButton(m_tweak_bar, "Enable all spot lights", EnableAllSpotLightsCallback, this, nullptr);
		TwAddButton(m_tweak_bar, "Disable all spot lights", DisableAllSpotLightsCallback, this, nullptr);
		TwAddVarCB(m_tweak_bar, "Spot lights: Pos. Offset", TW_TYPE_DIR3F, SetPosOffsetSlCB, GetPosOffsetSlCB, this, nullptr);
		//TwAddVarCB(m_tweak_bar, "Spot lights: Dir. Offset", TW_TYPE_DIR3F, SetDirOffsetSlCB, GetDirOffsetSlCB, this, nullptr);
		TwAddVarCB(m_tweak_bar, "All spot lights: light color", TW_TYPE_COLOR3F, SetAllLightColorSlCB, GetAllLightColorSlCB, this, nullptr);
		TwAddVarCB(m_tweak_bar, "All spot lights: const-atten", TW_TYPE_FLOAT, SetAllAtt0SlCB, GetAllAtt0SlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All spot lights: lin-atten", TW_TYPE_FLOAT, SetAllAtt1SlCB, GetAllAtt1SlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All spot lights: quad-atten", TW_TYPE_FLOAT, SetAllAtt2SlCB, GetAllAtt2SlCB, this, " min=0.0 step=0.01 ");
		TwAddVarCB(m_tweak_bar, "All spot lights: cub-atten", TW_TYPE_FLOAT, SetAllAtt3SlCB, GetAllAtt3SlCB, this, " min=0.0 step=0.01 ");
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
		TwAddVarRW(m_tweak_bar, "Gizmo Transparency", TW_TYPE_FLOAT, &m_transparency, " min=0.01 max=1.0 step=0.01 ");
		TwAddVarRW(m_tweak_bar, "PL Gizmo Scale", TW_TYPE_FLOAT, &m_gizmo_scale_ptl, " min=0.0 max=100.0 step=0.1 ");
		TwAddVarRW(m_tweak_bar, "PL Gizmo Param", TW_TYPE_FLOAT, &m_gizmo_param_ptl, " min=0.0 max=100.0 step=0.1 ");
		TwAddVarRW(m_tweak_bar, "SL Gizmo Scale", TW_TYPE_FLOAT, &m_gizmo_scale_sptl, " min=0.0 max=100.0 step=0.1 ");
		TwAddVarRW(m_tweak_bar, "SL Gizmo Param", TW_TYPE_FLOAT, &m_gizmo_param_sptl, " min=0.0 max=100.0 step=0.1 ");
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
	}

	LightsourceEditor::~LightsourceEditor()
	{
	}

	void LightsourceEditor::Set(AmbientLight* ambient_light)
	{
		m_ambient_light = ambient_light;
		const auto groupAssignment = " group='Ambient Light' ";
		TwAddVarCB(m_tweak_bar, "Amb. enabled", TW_TYPE_BOOLCPP, SetAmbientEnabledCallback, GetAmbientEnabledCallback, ambient_light, groupAssignment);
		TwAddVarCB(m_tweak_bar, "Amb. light-col", TW_TYPE_COLOR3F, SetAmbientLightColorCallback, GetAmbientLightColorCallback, ambient_light, groupAssignment);
		int opened = 0;
		TwSetParam(m_tweak_bar, "Ambient Light", "opened", TW_PARAM_INT32, 1, &opened);
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
	}

	void LightsourceEditor::Set(DirectionalLight* directional_light)
	{
		m_directional_light = directional_light;
		const auto groupAssignment = " group='Directional Light' ";
		TwAddVarCB(m_tweak_bar, "Dir. enabled", TW_TYPE_BOOLCPP, SetDLEnabledCallback, GetDLEnabledCallback, directional_light, groupAssignment);
		TwAddVarCB(m_tweak_bar, "Dir. direction", TW_TYPE_DIR3F, SetDLDirectionCallback, GetDLDirectionCallback, directional_light, groupAssignment);
		TwAddVarCB(m_tweak_bar, "Dir. light-col", TW_TYPE_COLOR3F, SetDLLightColorCallback, GetDLLightColorCallback, directional_light, groupAssignment);
		int opened = 0;
		TwSetParam(m_tweak_bar, "Directional Light", "opened", TW_PARAM_INT32, 1, &opened);
		TwAddSeparator(m_tweak_bar, nullptr, nullptr);
	}

	void LightsourceEditor::Add(PointLight* point_light)
	{
		Add(PointLightWrapper(*point_light, nullptr));
	}

	void LightsourceEditor::Add(PointLightWrapper point_light_wrapper)
	{
		auto n = m_point_lights.size();
		if (n >= kMaxPointLights)
		{
			log_error("Max. number of supported point lights reached.");
			return;
		}

		auto insert_index = n;
		m_point_lights.push_back(std::move(point_light_wrapper));
		auto* point_light_ptr = &m_point_lights[insert_index];
		m_point_lights_orig_pos.push_back(point_light_ptr->position());
		m_point_lights[insert_index].set_position(m_point_lights_orig_pos[insert_index] + m_uniform_position_offset_point_light);
		m_point_lights_tw_index_helper.push_back(std::make_tuple(this, insert_index));
		assert(m_point_lights.size() == m_point_lights_orig_pos.size());
		assert(m_point_lights.size() == m_point_lights_tw_index_helper.size());
		assert(m_point_lights.size() == insert_index + 1);

		auto name = "PL#" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarCB(m_tweak_bar, (name + " enabled").c_str(), TW_TYPE_BOOLCPP, SetEnabledPlCB, GetEnabledPlCB, point_light_ptr, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " pos").c_str(), TW_TYPE_DIR3F, SetPosPlCB, GetPosPlCB, &m_point_lights_tw_index_helper[insert_index], groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " light-col").c_str(), TW_TYPE_COLOR3F, SetLightColorPlCB, GetLightColorPlCB, point_light_ptr, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " const-att").c_str(), TW_TYPE_FLOAT, SetAtt0PlCB, GetAtt0PlCB, point_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " linear-att").c_str(), TW_TYPE_FLOAT, SetAtt1PlCB, GetAtt1PlCB, point_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " quadr-att").c_str(), TW_TYPE_FLOAT, SetAtt2PlCB, GetAtt2PlCB, point_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " cubic-att").c_str(), TW_TYPE_FLOAT, SetAtt3PlCB, GetAtt3PlCB, point_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		int opened = 0;
		auto plGroup = "Point Lights";
		TwSetParam(m_tweak_bar, name.c_str(), "opened", TW_PARAM_INT32,   1, &opened);
		TwSetParam(m_tweak_bar, name.c_str(), "group",  TW_PARAM_CSTRING, 1, plGroup);
		TwSetParam(m_tweak_bar, plGroup     , "opened", TW_PARAM_INT32,   1, &opened);
	}

	void LightsourceEditor::Add(SpotLight* spot_light)
	{
		Add(SpotLightWrapper(*spot_light, nullptr));
	}

	void LightsourceEditor::Add(SpotLightWrapper spot_light_wrapper)
	{
		auto n = m_spot_lights.size();
		if (n >= kMaxSpotLights)
		{
			log_error("Max. number of supported spot lights reached.");
			return;
		}

		auto insert_index = n;
		m_spot_lights.push_back(std::move(spot_light_wrapper));
		auto* spot_light_ptr = &m_spot_lights[insert_index];
		m_spot_lights_orig_pos.push_back(spot_light_ptr->position());
		m_spot_lights_orig_dir.push_back(spot_light_ptr->direction());
		m_spot_lights[insert_index].set_position(m_spot_lights_orig_pos[insert_index] + m_uniform_position_offset_spot_light);
		//m_spot_lights[insert_index].set_direction(m_spot_lights_orig_dir[insert_index] + m_uniform_direction_offset_spot_light);
		m_spot_lights_tw_index_helper.push_back(std::make_tuple(this, insert_index));
		assert(m_spot_lights.size() == m_spot_lights_orig_pos.size());
		assert(m_spot_lights.size() == m_spot_lights_tw_index_helper.size());
		assert(m_spot_lights.size() == insert_index + 1);

		auto name = "SL#" + std::to_string(n);
		auto groupAssignment = " group='" + name + "' ";
		TwAddVarCB(m_tweak_bar, (name + " enabled").c_str(), TW_TYPE_BOOLCPP, SetEnabledSlCB, GetEnabledSlCB, spot_light_ptr, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " pos").c_str(), TW_TYPE_DIR3F, SetPosSlCB, GetPosSlCB, &m_spot_lights_tw_index_helper[insert_index], groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " dir").c_str(), TW_TYPE_DIR3F, SetDirSlCB, GetDirSlCB, &m_spot_lights_tw_index_helper[insert_index], groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " light-col").c_str(), TW_TYPE_COLOR3F, SetLightColorSlCB, GetLightColorSlCB, spot_light_ptr, groupAssignment.c_str());
		TwAddVarCB(m_tweak_bar, (name + " const-att").c_str(), TW_TYPE_FLOAT,  SetAtt0SlCB, GetAtt0SlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " linear-att").c_str(), TW_TYPE_FLOAT, SetAtt1SlCB, GetAtt1SlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " quadr-att").c_str(), TW_TYPE_FLOAT,  SetAtt2SlCB, GetAtt2SlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " cubic-att").c_str(), TW_TYPE_FLOAT, SetAtt3SlCB, GetAtt3SlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " inner angle").c_str(), TW_TYPE_FLOAT, SetInnerAngleSlCB, GetInnerAngleSlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " outer angle").c_str(), TW_TYPE_FLOAT, SetOuterAngleSlCB, GetOuterAngleSlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		TwAddVarCB(m_tweak_bar, (name + " falloff").c_str(), TW_TYPE_FLOAT, SetFalloffSlCB, GetFalloffSlCB, spot_light_ptr, (" min=0.0 step=0.01 " + groupAssignment).c_str());
		int opened = 0;
		auto slGroup = "Spot Lights";
		TwSetParam(m_tweak_bar, name.c_str(), "opened", TW_PARAM_INT32, 1, &opened);
		TwSetParam(m_tweak_bar, name.c_str(), "group", TW_PARAM_CSTRING, 1, slGroup);
		TwSetParam(m_tweak_bar, slGroup, "opened", TW_PARAM_INT32, 1, &opened);
	}

	void LightsourceEditor::Clear()
	{
		m_point_lights.clear();
		m_point_lights_orig_pos.clear();
		m_point_lights_tw_index_helper.clear();
		m_spot_lights.clear();
		m_spot_lights_orig_pos.clear();
		m_spot_lights_orig_dir.clear();
		m_spot_lights_tw_index_helper.clear();
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

		{
			auto& sphere_mesh = m_sphere->mesh_at(0);
			auto n = m_point_lights.size();
			for (auto i = 0; i < n; ++i)
			{
				auto& ptlt = m_point_lights[i];

				if (!ptlt.enabled())
					continue;

				auto a = ptlt.attenuation();
				auto d = m_gizmo_param_ptl;
				auto s = m_gizmo_scale_ptl / (a[0] + a[1] * d + a[2] * d * d + a[3] * d * d * d);

				auto translation = glm::translate(ptlt.position()); // m_point_lights[i].get().transform().GetModelMatrix();
				auto scale = glm::scale(glm::vec3(s, s, s));
				m_gizmo_shader.SetUniform(mat_loc, pM * vM * translation * scale);
				m_gizmo_shader.SetUniform(col_loc, glm::vec4(ptlt.light_color(), m_transparency));
				RenderMesh(m_gizmo_shader, sphere_mesh);
			}
		}

		{
			auto& cone_mesh = m_cone->mesh_at(0);
			auto n = m_spot_lights.size();
			for (auto i = 0; i < n; ++i)
			{
				auto& sptl = m_spot_lights[i];

				if (!sptl.enabled())
					continue;

				auto a = sptl.attenuation();
				auto d = m_gizmo_param_sptl;
				auto s = m_gizmo_scale_sptl / (a[0] + a[1] * d + a[2] * d * d + a[3] * d * d * d);

				auto angle_scale = glm::tan(sptl.outer_angle() / 2.0f);

				auto translation = glm::translate(sptl.position()); // m_point_lights[i].get().transform().GetModelMatrix();
				auto rotation = rotate_vector_a_to_vector_b(glm::vec3(0, 0, 1), sptl.direction());
				auto scale = glm::scale(glm::vec3(angle_scale * s, angle_scale * s, s));
				m_gizmo_shader.SetUniform(mat_loc, pM * vM * translation * rotation * scale);
				m_gizmo_shader.SetUniform(col_loc, glm::vec4(sptl.light_color(), m_transparency));
				RenderMesh(m_gizmo_shader, cone_mesh);
			}
		}

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}

}
