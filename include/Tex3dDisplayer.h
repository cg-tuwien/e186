#pragma once

#include "Tex3D.h"

namespace e186
{

	class Tex3dDisplayer
	{
	public:
		Tex3dDisplayer(Tex3D& tex3d);

	private:
		Tex3D& m_tex3d;
		GLuint m_b1, m_b2, m_b3, m_vao;
	};

}
