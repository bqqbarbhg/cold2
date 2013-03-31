#ifndef _C2_CORE_EVENT_H
#define _C2_CORE_EVENT_H

#include <c2/core/flat_set.h>
#include <functional>
#include <memory>
#include <cstdint>

namespace c2 {

namespace impl {
typedef std::uint_fast32_t EventHandlerId;

template <typename T>
class EventHandlerBind
{
public:
	EventHandlerBind(EventHandlerId i, std::function<F> f)
		: id(i), target(f)
	{
	}
	EventHandlerId id;
	std::function<F> target;

	bool operator<(const EventHandlerBind& rhs) const;
};
}

template <typename F>
class Event
{
public:
	class Handler
	{
	public:
		~Handler()
		{
			if (!listref.expired())
			{
				auto it = listref->find(id, [](impl::EventHandlerBind bind, impl::EventHandlerId id) { return bind.id < id; });
				if (it->id == id)
					listref->erase(it);
			}
		}
	private:
		Handler(impl::EventHandlerId i, std::weak_ptr<FlatSet<impl::EventHandlerBind<F>>> l)
			: id(i), listref(l)
		{
		}
		impl::EventHandlerId id;
		std::weak_ptr<FlatSet<impl::EventHandlerBind<F>>> listref;
	};

	Handler add(std::function<F> target, unsigned int priority)
	{
		impl::EventHandlerId id = counter++ | priority << 16;
		counter &= 0xFFFF;
		events->insert(EventHandlerBind(id, target));
		return Handler(id, events);
	}

	template <typename Args...>
	void operator()(Args... args)
	{
		for (auto it = events->begin(); it != events->end(); ++it)
		{
			if (it->target(args...))
				break;
		}
	}
private:
	impl::EventHandlerId counter;
	std::shared_ptr<FlatSet<impl::EventHandlerBind<F>>> events;
};

}

#endif