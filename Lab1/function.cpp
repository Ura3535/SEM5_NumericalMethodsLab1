#include "function.h"
#include "exprtk.hpp"

using namespace function;

typedef T(*cmp_t) (T, T);

static T min_or_max(func_t func, segment_t seg, cmp_t cmp, int acc) {
	T	res = func(seg.second),
		tmp = 0;
	for (int i = 0; i != acc; ++i) {
		tmp = func(seg.first + (seg.second - seg.first) / acc * i);
		res = cmp(res, tmp);
	}
		
	return res;
}

static inline T min(func_t func, segment_t seg, int acc = 10000) {
	return min_or_max(func, seg, [](T a, T b) {return std::min(a, b); }, acc);
}

static inline T max(func_t func, segment_t seg, int acc = 10000) {
	return min_or_max(func, seg, [](T a, T b) {return std::max(a, b); }, acc);
}

std::vector<val_and_acc_t> function::find_by_relaxation(func_t func, func_t deriv, segment_t seg, T acc)
{
	T	a = min(deriv, seg),
		b = max(deriv, seg);
	if (a * b <= 0) {
		throw std::exception("The derivate has different signs on the segment");
	}
	if (a < 0) {
		std::swap(a, b);
	}
	
	const T
		m = std::abs(a),
		M = std::abs(b),
		t = 2 / (m + M),
		q = (M - m) / (M + m);

	if (q >= 1)
		throw std::exception("No roots?!");

	T	x = (seg.first + seg.second) / 2,
		curr_acc = x - seg.first;

	bool sign = deriv(x) > 0;
	std::vector<val_and_acc_t> res;
	while (acc < curr_acc) {
		res.push_back(std::make_pair(x, curr_acc));
		x += (t * func(x) * (sign ? -1 : 1));
		curr_acc *= q;
	}
	res.push_back(std::make_pair(x, curr_acc));
	
	return res;
}

std::vector<val_and_acc_t> function::find_by_Newton(func_t func, func_t deriv, func_t second_deriv, segment_t seg, T acc)
{
	if (func(seg.first) * func(seg.second) >= 0)
		throw std::exception("The function must have different signs at the ends of the segment");

	T	x = (seg.first + seg.second) / 2,
		curr_acc = x - seg.first,
		a = min(deriv, seg),
		b = max(second_deriv, seg);
	if (a < 0) {
		std::swap(a, b);
	}

	const T
		m = std::abs(a),
		M = std::abs(b),
		q = M * (x - seg.first) / (2 * m);

	if (q >= 1)
		throw std::exception("No roots?!");

	T q2n = q;
	std::vector<val_and_acc_t> res;
	while (acc < curr_acc) {
		res.push_back(std::make_pair(x, curr_acc));
		x -= func(x) / deriv(x);
		curr_acc *= q2n;
		q2n *= q2n;
	}
	res.push_back(std::make_pair(x, curr_acc));

	return res;
}
