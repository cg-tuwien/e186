#pragma once
namespace e186
{
	
	class ModelExplorerScene : public IScene
	{
	public:
		ModelExplorerScene(std::string model_to_load_path, glm::mat4 transformation_matrix, unsigned int model_loader_flags = MOLF_default);
		virtual ~ModelExplorerScene();
		void Terminate() override;
		void Run() override;

	private:
		bool m_termination_requested;
		std::unique_ptr<Model> m_model;
	};

}
