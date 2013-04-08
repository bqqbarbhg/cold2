#ifndef _C2_CORE_EVENT_H
#define _C2_CORE_EVENT_H

#include <c2/core/flat_set.h>
#include <functional>
#include <memory>
#include <cstdint>

namespace c2 {

namespace impl {
typedef std::uint_fast32_t EventHandlerId;

template <typename F>
class EventHandlerBind
{
public:
	EventHandlerBind(EventHandlerId i, std::function<F> f)
		: id(i), target(f), alive(true)
	{
	}
	EventHandlerId id;
	bool alive;
	std::function<F> target;

	bool operator<(const EventHandlerBind& rhs) const
	{
		return id < rhs.id;
	}
};
}

template <typename F>
class Event
{
public:
	class Handler
	{
	public:
		Handler(Handler&& h)
			: id(h.id), listref(h.listref)
		{
			h.listref.reset();
		}
		Handler(impl::EventHandlerId i, std::weak_ptr<FlatSet<impl::EventHandlerBind<F>>> l)
			: id(i), listref(l)
		{
		}
		~Handler()
		{
			if (!listref.expired())
			{
				auto it = listref.lock()->find(id, [](impl::EventHandlerBind<F> bind, impl::EventHandlerId id) { return bind.id < id; });
				if (it != listref.lock()->end() && it->id == id)
					it->alive = false;
			}
		}
		
	private:
		friend class Event<F>;
		Handler(const Handler& h);
		Handler& operator=(const Handler& h);
		impl::EventHandlerId id;
		std::weak_ptr<FlatSet<impl::EventHandlerBind<F>>> listref;
	};

	Event()
		: counter(0), events(std::make_shared<FlatSet<impl::EventHandlerBind<F>>>())
	{
	}

	Handler add(const std::function<F>& target, unsigned int priority=0)
	{
		impl::EventHandlerId id = counter++ | priority << 16;
		counter &= 0xFFFF;
		events->insert(impl::EventHandlerBind<F>(id, target));
		return Handler(id, events);
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
		typename FlatSet<impl::EventHandlerBind<F>>::iterator it, res, end;
		for (res = it = events->begin(), end = events->end(); it != end; ++it)
		{
			if (it->alive)
			{
				if (res != it)
					*res = std::move(*it);
				res++;
				it->target(arg);
			}
		}
		int c = res - events->begin();
		events->erase(res, events->end());
		return c;
	}
private:
	impl::EventHandlerId counter;
	std::shared_ptr<FlatSet<impl::EventHandlerBind<F>>> events;
};

}

#endif