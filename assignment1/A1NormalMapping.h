#pragma once
#include "QuakeCamera.h"
#include <LightsourceEditor.h>

using namespace e186;

namespace artr
{

	/*!	A scene showing a part of the sponza 3D model and a 
	 *	surrounding terrain. Some meshes of sponza and the 
	 *	terrain have materials which include normal maps.
	 *	They are shaded with a blinn-phong-normal-mapping 
	 *	shader. 
	 */
	class A1NormalMapping : public IScene
	{
	public:
		/*! Load and initialize all kinds of stuff in the constructor */
		A1NormalMapping();
		/*! Cleanup and destroy stuff in the destructor which is not
		 *	cleaned up or destroyed automatically. */
		virtual ~A1NormalMapping();

		/*! Invoked by the engine to indicate that this scene should be terminated. */
		void Terminate() override;
		/*! Invoked by the engine. By calling this method, it gains complete control
			over the program and must ensure to call some Engine-methods properly
			(namely, StartFrame and EndFrame) */
		void Run() override;

	private:
		/*!	Render all the meshes
		 */
		void Render(const glm::mat4& pM, const glm::mat4& vM);

		void SetUniformMatrices(Shader& shader, const glm::mat4& pM, const glm::mat4& vM, const glm::mat4& mM);

		/*! Create several point lights of different colors at positions across the scene.
		 */
		void CreateLotsOfLights();

		/*!	Changes the position of the first point light based on the elapsed time
		 */
		void AnimateFirstTwoPointLights(float elapsed_time);

		/*! Set to true when the framework tells this scene to terminate */
		bool m_should_terminate;
		/*! AntTweakBar handle for this scene's specific settings */
		AntTweakBarHandle m_scene_settings;
		/*! Whether or not LightsourceEditor should draw gizmos to indicate light positions */
		bool m_render_gizmos;

		/*! Whether or not the first/second point light should be animated or stay in place. */
		bool m_animate_first_two_point_lights;

		/*! Position of the first/second point light, or the rotation origin when animating its position. */
		static glm::vec3 kInitialPositionOfFirstPointLight, kInitialPositionOfSecondPointLight;

		/*! The shader used for rendering normal-mapped materials, blinn-phong shading */
		Shader m_blinnphong_nm_shader;
		
		/*! A camera which can be controlled via WASD+mouse to fly around */
		QuakeCamera m_camera;

		/*! (A part of) the sponza 3D model */
		std::unique_ptr<Model> m_sponza_model;
		/*! A terrain model which surrounds the sponza 3D model */
		std::unique_ptr<Model> m_terrain_model;
		/*! A parallelepiped for one of the bonus-tasks */
		std::unique_ptr<Model> m_parallelepiped;

		/*! Ambient light used in the shaders (can be edited via LightsourceEditor) */
		AmbientLight m_ambient_light;
		/*! Directional light used in the shaders (can be edited via LightsourceEditor) */
		DirectionalLight m_dir_light;
		/*! A vector of point lights which should finally all be passed to the shader (Task 2) */
		std::vector<PointLight> m_point_lights;
		/*! Used to edit light source properties and indicate the positions of point lights (gizmos) */
		LightsourceEditor m_lights_editor;

		/*! Material setters for the meshes of sponza */
		MeshUniformSettersForShader m_unisetters_sponza;
		/*! Material setters for the meshes of the terrain */
		MeshUniformSettersForShader m_unisetters_terr;
		/*! Material setters for the meshes of the parallelepiped */
		MeshUniformSettersForShader m_unisetters_parallelepiped;
		/*! VAOs for the meshes of sponza */
		MeshVaosForAttribConfig m_vaos_sponza;
		/*! VAOs for the meshes of the terrain */
		MeshVaosForAttribConfig m_vaos_terr;
		/*! VAOs for the meshes of the parallelepiped */
		MeshVaosForAttribConfig m_vaos_parallelepiped;
	};

}
