
#include <vector>
#include <algorithm>

namespace c2 {

template <typename T, typename C=std::less<T>>
class FlatSet
{
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	iterator insert(const T& t)
	{
		return vec.insert(std::lower_bound(vec.begin(), vec.end(), t), t);
	}
	iterator insert(T&& t)
	{
		return vec.insert(std::lower_bound(vec.begin(), vec.end(), t), std::move(t));
	}

	iterator find(const T& t)
	{
		return std::lower_bound(vec.begin(), vec.end(), t);
	}
	const_iterator find(const T& t) const
	{
		return std::lower_bound(vec.cbegin(), vec.cend(), t);
	}

	template <typename E, typename C>
	iterator find(const E& t, C c)
	{
		return std::lower_bound(vec.begin(), vec.end(), t, c);
	}
	template <typename E, typename C>
	const_iterator find(const E& t, C c) const
	{
		return std::lower_bound(vec.cbegin(), vec.cend(), t, c);
	}

	iterator erase(const_iterator it)
	{
		return vec.erase(it);
	}
	iterator erase(const_iterator begin, const_iterator end)
	{
		return vec.erase(begin, end);
	}

	iterator begin()
	{
		return vec.begin();
	}
	iterator end()
	{
		return vec.end();
	}
	const_iterator cbegin() const
	{
		return vec.cbegin();
	}
	const_iterator cend() const
	{
		return vec.cend();
	}

private:
	std::vector<T> vec;
};

}