#pragma once
namespace e186
{
	enum struct AutoMatrix : uint32_t
	{
		Nothing				= 0x0000,
		TransformMatrix		= 0x0001,
		ModelMatrix			= 0x0002,
		ViewMatrix			= 0x0004,
		ProjectionMatrix	= 0x0008,
		IsNormalMatrix		= 0x0010,
		IsOptional			= 0x0020,
		IsMandatory			= 0x0040,
		DoTranspose			= 0x0080,
		DoInvert			= 0x0100,
	};

	inline AutoMatrix operator| (AutoMatrix a, AutoMatrix b)
	{
		typedef std::underlying_type<AutoMatrix>::type EnumType;
		return static_cast<AutoMatrix>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline AutoMatrix operator& (AutoMatrix a, AutoMatrix b)
	{
		typedef std::underlying_type<AutoMatrix>::type EnumType;
		return static_cast<AutoMatrix>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}
}
