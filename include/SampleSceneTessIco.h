#pragma once
namespace e186
{
	class SampleSceneTessIco : public IScene
	{
	public:
		SampleSceneTessIco();
		virtual ~SampleSceneTessIco();
		void Terminate() override;
		void Run() override;

	private:
		bool m_termination_requested;
	};
}
