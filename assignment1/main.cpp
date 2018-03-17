#include "A1NormalMapping.h"

int main()
{
	e186::Engine::StartWindowedWithRootScene(1600, 900, []()
	{
		return std::make_unique<artr::A1NormalMapping>();
	});
}
