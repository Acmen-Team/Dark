#include <Dark.h>

class SandBox :public Dark::Application
{
public:
	SandBox()
	{

	}
	~SandBox()
	{

	}
};

Dark::Application* Dark::CreateApplication()
{
	return new SandBox();
}
