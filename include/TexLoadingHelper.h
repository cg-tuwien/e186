#pragma once
#include "Tex2D.h"

namespace e186
{
	
	class TexLoadingHelper
	{
	public:
		TexLoadingHelper(std::string model_path);
		std::shared_ptr<Tex2D> GetOrLoadTex(std::string sub_path, TexParams texture_parameters);
		void AddTexParams(TexParams to_add);
		void RemoveTexParams(TexParams to_remove);
		TexParams general_tex_params() const;

	private:
		TexParams m_general_tex_params;
		std::string m_base_dir;
		std::unordered_map<std::string, std::shared_ptr<Tex2D>> m_tex_cache;
	};

}
