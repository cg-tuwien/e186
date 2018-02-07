#pragma once
namespace e186
{
	enum struct VertexAttribData : uint32_t
	{
		Nothing			= 0x0000,
		Position		= 0x0001,
		Normal			= 0x0002,
		Color			= 0x0004,
		Bones			= 0x0008,
		Tex2D			= 0x0010,
		Tex2D_2			= 0x0020,
		Tex2D_3			= 0x0040,
		Tangents		= 0x0080,
		Bitangents		= 0x0100,
		Param0			= 0x0200,
		Param1			= 0x0400,
		Param2			= 0x0800,
		Param3			= 0x1000,
		Param4			= 0x2000,
		Param5			= 0x4000,
		//TangentSpace	= 0x0182
	};

	inline VertexAttribData operator| (VertexAttribData a, VertexAttribData b)
	{
		typedef std::underlying_type<VertexAttribData>::type EnumType;
		return static_cast<VertexAttribData>(static_cast<EnumType>(a) | static_cast<EnumType>(b));
	}

	inline VertexAttribData operator& (VertexAttribData a, VertexAttribData b)
	{
		typedef std::underlying_type<VertexAttribData>::type EnumType;
		return static_cast<VertexAttribData>(static_cast<EnumType>(a) & static_cast<EnumType>(b));
	}

	inline VertexAttribData& operator |= (VertexAttribData& a, VertexAttribData b)
	{
		return a = a | b;
	}

	inline VertexAttribData& operator &= (VertexAttribData& a, VertexAttribData b)
	{
		return a = a & b;
	}

}
