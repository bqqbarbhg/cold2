#ifndef _C2_CORE_ENABLED_EVENT_H
#define _C2_CORE_ENABLED_EVENT_H

#include <c2/core/event.h>

namespace c2 {
template <typename F, typename E>
class EnabledEvent : public Event<F>
{
public:
	EnabledEvent(E e)
		: handle_count(0), e(e), enabled(false)
	{
	}
	~EnabledEvent()
	{
		if (enabled)
			e(false);
	}

	Handler add(const std::function<F>& target, unsigned int priority=0)
	{
		if (handle_count == 0)
			e(enabled = true);
		handle_count++;

		Event<F>::add(target, priority);
	}

	void remove(const Handler& handler)
	{
		auto it = events->find(handler.id, [](impl::EventHandlerBind<F> bind, impl::EventHandlerId id) { return bind.id < id; });
		if (it != events->end() && it->id == handler.id)
			it->alive = false;
	}
	template <typename Arg0>
	int operator()(Arg0 arg)
	{
		handle_count = Event<F>::operator()(arg);
		if (handle_count == 0)
			e(enabled = false);
	}
private:
	unsigned int handle_count;
	bool enabled;
	E e;
};
}

#endif