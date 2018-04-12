#include "TexLoadingHelper.h"
#include "string_utils.h"


namespace e186
{
	TexLoadingHelper::TexLoadingHelper(std::string model_path)
		: m_base_dir(utils::ExtractBasePath(model_path)),
		m_general_tex_params(TexParams::AnisotropicFilteringMax)
	{

	}

	void TexLoadingHelper::AddTexParams(TexParams to_add)
	{
		m_general_tex_params |= to_add;
	}

	void TexLoadingHelper::RemoveTexParams(TexParams to_remove)
	{
		m_general_tex_params &= ~to_remove;
	}

	TexParams TexLoadingHelper::general_tex_params() const
	{
		return m_general_tex_params;
	}

	std::shared_ptr<Tex2D> TexLoadingHelper::GetOrLoadTex(std::string sub_path, TexParams texture_parameters, bool srgb)
	{
		std::string complete_path;
		assert(sub_path.length() > 1);
		const auto first_char = sub_path[0];
		if (first_char == utils::SEP_TO_USE || first_char == utils::SEP_NOT_TO_USE)
			complete_path = utils::CleanUpPath(m_base_dir + sub_path.substr(1));
		else
			complete_path = utils::CleanUpPath(m_base_dir + sub_path);

		// Is the texture already in our cache?
		auto search = m_tex_cache.find(complete_path);
		if (search != m_tex_cache.end())
		{
			return search->second;
		}
		else
		{
			auto tex = std::make_shared<Tex2D>();
			if (srgb)
				tex->FromFile(complete_path).UploadSRGBIfPossible().BindAndSetTextureParameters(texture_parameters).GenerateMipMaps();
			else
				tex->FromFile(complete_path).Upload().BindAndSetTextureParameters(texture_parameters).GenerateMipMaps();
			m_tex_cache[complete_path] = tex;
			return tex;
		}
	}
}
