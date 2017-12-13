#include "string_utils.h"

namespace e186
{
	namespace utils
	{
		std::string ExtractBasePath(const std::string& path)
		{
			auto cleaned_path = CleanUpPath(path);
			auto last_sep_idx = cleaned_path.find_last_of(SEP_TO_USE);
			if (std::string::npos == last_sep_idx)
			{
				return cleaned_path;
			}
			return cleaned_path.substr(0, last_sep_idx + 1);
		}

		std::string CleanUpPath(const std::string& path)
		{
			auto cleaned_up = path;
			int consecutive_sep_cnt = 0;
			for (int i = 0; i < cleaned_up.size(); ++i)
			{
				if (cleaned_up[i] == SEP_NOT_TO_USE)
				{
					cleaned_up[i] = SEP_TO_USE;
				}
				if (cleaned_up[i] == SEP_TO_USE)
				{
					consecutive_sep_cnt += 1;
				}
				else
				{
					consecutive_sep_cnt = 0;
				}
				if (consecutive_sep_cnt > 1)
				{
					cleaned_up = cleaned_up.substr(0, i) + (i < cleaned_up.size() - 1 ? cleaned_up.substr(i + 1) : "");
					consecutive_sep_cnt -= 1;
					--i;
				}
			}
			return cleaned_up;
		}
	}
}
