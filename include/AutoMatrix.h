#pragma once
namespace e186
{
	enum AutoMatrix : unsigned int
	{
		AutoMatrix_Nothing				= 0x0000,
		AutoMatrix_TransformMatrix		= 0x0001,
		AutoMatrix_ModelMatrix			= 0x0002,
		AutoMatrix_ViewMatrix			= 0x0004,
		AutoMatrix_ProjectionMatrix		= 0x0008,
		AutoMatrix_IsNormalMatrix		= 0x0010,
		AutoMatrix_IsOptional			= 0x0020,
		AutoMatrix_IsMandatory			= 0x0040,
	};
}
