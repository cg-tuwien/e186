#pragma once
namespace e186
{
	enum VertexAttribData : unsigned int
	{
		VertexAttribData_Nothing		= 0x0000,
		VertexAttribData_Position		= 0x0001,
		VertexAttribData_Normal			= 0x0002,
		VertexAttribData_Color			= 0x0004,
		VertexAttribData_Bones			= 0x0008,
		VertexAttribData_Tex2D			= 0x0010,
		VertexAttribData_Tex2D_2		= 0x0020,
		VertexAttribData_Tex2D_3		= 0x0040,
		VertexAttribData_TangentSpace	= 0x0080,
		VertexAttribData_Param0			= 0x0100,
		VertexAttribData_Param1			= 0x0200,
		VertexAttribData_Param2			= 0x0400,
		VertexAttribData_Param3			= 0x0800,
		VertexAttribData_Param4			= 0x1000,
		VertexAttribData_Param5			= 0x2000
	};
}
