#pragma once

namespace e186
{
	class IScene
	{
	public:
		virtual ~IScene() {}

		// Instruct a Scene to end the loop and return from the Run() method, 
		// i.e. Run is expected to return soon after calling Terminate() on a scene.
		virtual void Terminate() = 0;

		// Run in a loop until Terminate is called
		virtual void Run() = 0;
	};
}
