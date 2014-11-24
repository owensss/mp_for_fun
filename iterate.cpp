#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>
#include <functional>

using namespace std;

template <class F, class ... T>
void invoke_and_advance(F f, T& ... t) {
	f(*(t++)...);
}

template <int idx>
struct iterate_unpack_impl {
	template <class F, class Tp, class ... Unpack>
	void operator() (F f, Tp& tp, Unpack& ... args) {
		iterate_unpack_impl<idx-1>()(f, tp, get<idx>(tp), args...);
	}
};

template <>
struct iterate_unpack_impl<0> {
	template <class F, class Tp, class ... Unpack>
	void operator() (F f, Tp& tp, Unpack& ... args) {
		invoke_and_advance(f, get<0>(tp), args...);
	}
};

template <class F, class Tp>
void iterate_unpack(F f, Tp& tp) {
	iterate_unpack_impl<tuple_size<Tp>::value-1>()(f, tp);
}

template <class F, class CF, class ... C>
void iterate(F f, CF&& first, C&& ... c) {
	auto ifirst = begin(first);
	auto tp = make_tuple<decltype(std::begin(first)), decltype(begin(c))...>(begin(first), begin(c)...); // reference

	while (ifirst++ != end(first)) {
		iterate_unpack(f, tp);
	}
}

void foo(int a, int b) {
	cout << a << "\t" << b << endl;
}

int main() {
	vector<int> a = {1, 2, 3, 4, 5};
	vector<int> b = {2, 4, 6, 8, 10};

	iterate(foo, a, b);

	int c[5] = {1, 2, 3, 4, 5};
	int d[5] = {2, 4, 6, 8, 10};

	iterate(foo, c, d);
}
