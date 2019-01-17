#include "Tex3dDisplayer.h"
#include "Model.h"
#include "Shader.h"

#define MODERN_VAO 0

namespace e186
{
	Tex3dDisplayer::Tex3dDisplayer(Tex3D& tex3d) 
		: m_tex3d(tex3d)
		, m_element_pos(0.0f, 0.0f, 0.0f)
	    , m_element_scale(0.05f)
	    , m_element_rotation_axis(1.0f, 1.0f, 1.0f)
	    , m_element_rotation_angle(0.0f)
		, m_twbar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("3D-Texture Displayer"))
	{
		m_shader
			.AddVertexShaderSourceFromFile("assets/shaders/tex3d_element.vert")
			.AddFragmentShaderSourceFromFile("assets/shaders/tex3d_element.frag")
			.Build();

		GLfloat side2 = 1.0 / 2.0f;

		// cube

		std::vector<GLfloat> p = {
			// Front
		   -side2, -side2, side2, side2, -side2, side2, side2,  side2, side2,  -side2,  side2, side2,
		   // Right
			side2, -side2, side2, side2, -side2, -side2, side2,  side2, -side2, side2,  side2, side2,
			// Back
			-side2, -side2, -side2, -side2,  side2, -side2, side2,  side2, -side2, side2, -side2, -side2,
			// Left
			-side2, -side2, side2, -side2,  side2, side2, -side2,  side2, -side2, -side2, -side2, -side2,
			// Bottom
			-side2, -side2, side2, -side2, -side2, -side2, side2, -side2, -side2, side2, -side2, side2,
			// Top
			-side2,  side2, side2, side2,  side2, side2, side2,  side2, -side2, -side2,  side2, -side2
		};

		std::vector<GLfloat> n = {
			// Front
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			// Right
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			// Back
			0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
			// Left
			-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			// Bottom
			0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			// Top
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
		};

		std::vector<GLfloat> tex = {
			// Front
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			// Right
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			// Back
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			// Left
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			// Bottom
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			// Top
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::vector<GLuint> el = {
			0,1,2,0,2,3,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
			12,13,14,12,14,15,
			16,17,18,16,18,19,
			20,21,22,20,22,23
		};

		std::vector<glm::vec3> insti_positions = {
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(1.0, 0.0, 0.0),
			glm::vec3(2.0, 0.0, 0.0),
			glm::vec3(2.0, 0.0, 1.0),
			glm::vec3(2.0, 0.0, 2.0),
		};

		m_indices_len = el.size();

		// Generate the buffers
		glCreateBuffers(1, &m_posBuffer);
		assert(m_posBuffer > 0);
		glCreateBuffers(1, &m_nrmBuffer);
		assert(m_nrmBuffer > 0);
		glCreateBuffers(1, &m_elBuffer);
		assert(m_elBuffer > 0);
		//glCreateBuffers(1, &m_instPosBuffer);
		//assert(m_instPosBuffer > 0);

		// Allocate space for all buffers
		glNamedBufferData(m_posBuffer, p.size() * sizeof(GLfloat), &p[0], GL_STATIC_DRAW);
		glNamedBufferData(m_nrmBuffer, n.size() * sizeof(GLfloat), &n[0], GL_STATIC_DRAW);
		glNamedBufferData(m_elBuffer, el.size() * sizeof(GLuint), &el[0], GL_STATIC_DRAW);
		//glNamedBufferData(m_instPosBuffer, insti_positions.size() * 3 * sizeof(float), &insti_positions[0], GL_STATIC_DRAW);

		// Create vertex arrays for each set of buffers
		glGenVertexArrays(1, &m_vao);
		assert(m_vao > 0);
		glBindVertexArray(m_vao);
#if MODERN_VAO
		// New way to specify VAO-attribute arrays (as described in GLSL cookbook, chapter 3)
		//   location = 0: positions
		glEnableVertexArrayAttrib(m_vao, 0);
		glVertexArrayVertexBuffer(m_vao, 0, m_posBuffer, 0, 0);
		glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_vao, 0, 0);
		//   location = 1: normals
		glEnableVertexArrayAttrib(m_vao, 1);
		glVertexArrayVertexBuffer(m_vao, 1, m_nrmBuffer, 0, 0);
		glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_vao, 1, 1);
		//    element array
		glVertexArrayElementBuffer(m_vao, m_elBuffer);
#else
		//   location = 0: positions
		glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
		//   location = 1: normals
		glBindBuffer(GL_ARRAY_BUFFER, m_nrmBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		////    location = 2: element positions
		//glBindBuffer(GL_ARRAY_BUFFER, m_instPosBuffer);
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		//glEnableVertexAttribArray(2);
		//glVertexAttribDivisor(2, 1); // advance after each object
		//    element array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elBuffer);
#endif
		glBindVertexArray(0);

		// ============= set up tweak bar ====================

		TwAddVarRW(m_twbar, "Pos", TW_TYPE_DIR3F, glm::value_ptr(m_element_pos), nullptr);
		TwAddVarRW(m_twbar, "Scale", TW_TYPE_FLOAT, &m_element_scale, "min=0 step=0.01");
		TwAddVarRW(m_twbar, "Rot Axis", TW_TYPE_DIR3F, glm::value_ptr(m_element_rotation_axis), nullptr);
		TwAddVarRW(m_twbar, "Rot Angle", TW_TYPE_FLOAT, &m_element_rotation_angle, "step=0.01");


	}

	void Tex3dDisplayer::Render(const glm::mat4& mM, const glm::mat4& vM, const glm::mat4& pM)
	{
		m_shader.Use();
		m_shader.SetUniform("pvmMatrix", pM * vM * mM);
		m_shader.SetUniform("uColor", glm::vec4(0.5f, 0.2f, 0.5f, 1.0f));
		m_shader.SetFirstSampler("uSampler", m_tex3d);
		m_shader.SetUniform("uTexDimensions", glm::ivec3(m_tex3d.width(), m_tex3d.height(), m_tex3d.depth()));

		glBindVertexArray(m_vao);
		glEnableVertexArrayAttrib(m_vao, 0);
		glEnableVertexArrayAttrib(m_vao, 1);
		glEnableVertexArrayAttrib(m_vao, 2);
		glVertexArrayElementBuffer(m_vao, m_elBuffer);
		//glDrawElements(GL_TRIANGLES, m_indices_len, GL_UNSIGNED_INT, nullptr);

		// draw a cube for each voxel, instance positions are determined in shader using gl_InstanceID and 3D texture
		glDrawElementsInstanced(GL_TRIANGLES, m_indices_len, GL_UNSIGNED_INT, nullptr, 
		                        m_tex3d.width() * m_tex3d.height() * m_tex3d.depth());
	}

	void Tex3dDisplayer::Render(const glm::mat4 &vM, const glm::mat4 &pM)
	{
		glm::mat4 centerTranslationMat = glm::translate(glm::vec3(-m_tex3d.width()/2, -m_tex3d.height()/2, -m_tex3d.depth()/2));
		glm::mat4 scaleMat = glm::scale(glm::vec3(m_element_scale));
		glm::mat4 rotationMat = glm::rotate(m_element_rotation_angle, m_element_rotation_axis);
		glm::mat4 translationMat = glm::translate(m_element_pos);
		glm::mat4 mMTweaked = translationMat * rotationMat * scaleMat * centerTranslationMat;

		Render(mMTweaked, vM, pM);
	}
}
