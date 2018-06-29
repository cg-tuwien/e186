#pragma once

namespace e186
{
	enum struct ShaderSourceInfo : uint32_t
	{
		Nothing				= 0x0000,
		FromFile			= 0x0001,
		FromMemory			= 0x0002,
		AppendNewline		= 0x0004,
	};
	
	inline ShaderSourceInfo operator| (ShaderSourceInfo a, ShaderSourceInfo b)
	{
		typedef std::underlying_type<ShaderSourceInfo>::type EnumType;
		return static_cast<ShaderSourceInfo>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline ShaderSourceInfo operator& (ShaderSourceInfo a, ShaderSourceInfo b)
	{
		typedef std::underlying_type<ShaderSourceInfo>::type EnumType;
		return static_cast<ShaderSourceInfo>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}

	inline ShaderSourceInfo& operator |= (ShaderSourceInfo& a, ShaderSourceInfo b)
	{
		return a = a | b;
	}

	inline ShaderSourceInfo& operator &= (ShaderSourceInfo& a, ShaderSourceInfo b)
	{
		return a = a & b;
	}
}