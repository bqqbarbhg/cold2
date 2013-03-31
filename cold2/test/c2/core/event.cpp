#include <c2/core/event.h>
#include <cstdio>

int main(int argc, char** argv)
{
	typedef c2::Event<bool(int)> TestEvent;
	
	TestEvent event;
	
	auto ha = event.add([](int i){ printf("a: %d\n", i); return false; }, 0);
	auto hb = event.add([](int i){ printf("b: %d\n", i); return false; }, 0);
	auto hc = event.add([](int i){ printf("c: %d\n", i); return false; }, 0);


	event(10);
	event(20);
	event(30);

	getchar();
}