#include "MaxFpsTimer.h"
#include "QuakeCamera.h"
#include <glad/glad.h>
#include "Stopwatch.h"

#include "Voxelizer.h"

#pragma once
namespace e186
{

    class VoxelizationTestScene : public IScene
	{
	public:
		VoxelizationTestScene();
		virtual ~VoxelizationTestScene();
		void Terminate() override;
		void Run() override;

	private:

		bool m_termination_requested;

		std::unique_ptr<Model> m_model_for_voxelization;
		std::unique_ptr<Model> m_model_sponza;

		std::unique_ptr<Voxelizer> m_voxelizer;
	};

}
