#pragma once

namespace e186
{
	enum struct ShaderType
	{
		None			= 0x0000,
		Vertex			= 0x0001,
		TessControl		= 0x0002,
		TessEval		= 0x0004,
		Geometry		= 0x0008,
		Fragment		= 0x0010,
		Compute			= 0x0020
	};
	
	inline ShaderType operator| (ShaderType a, ShaderType b)
	{
		typedef std::underlying_type<ShaderType>::type EnumType;
		return static_cast<ShaderType>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline ShaderType operator& (ShaderType a, ShaderType b)
	{
		typedef std::underlying_type<ShaderType>::type EnumType;
		return static_cast<ShaderType>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}

	inline ShaderType& operator |= (ShaderType& a, ShaderType b)
	{
		return a = a | b;
	}

	inline ShaderType& operator &= (ShaderType& a, ShaderType b)
	{
		return a = a & b;
	}
}