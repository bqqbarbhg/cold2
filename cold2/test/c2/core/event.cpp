#include <c2/core/event.h>
#include <cstdio>

int main(int argc, char** argv)
{
	
	typedef c2::Event<void(int)> TestEvent;
	TestEvent event;
	{
		auto ea = event.add([&](int i){ printf("a: %d\n", i); });
		auto eb = event.add([&](int i){ printf("b: %d\n", i); });

		event(10);
		event(20);
		event(30);

		event.remove(ea);

		event(10);
		event(20);
		event(30);
	}
	event(10);
	event(20);
	event(30);

	getchar();
}