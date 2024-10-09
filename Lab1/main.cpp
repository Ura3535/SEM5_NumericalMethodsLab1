#include<iostream>
#include<cmath>
#include<numbers>
#include<iomanip>
#include"function.h"

const auto PI = std::numbers::pi;

using namespace function;

namespace test1 {
	T F(T x) {
		return x * x * x - 10 * x * x + 44 * x +29;
	}
	T dF(T x) {
		return 3 * x * x - 20 * x + 44;
	}
	T ddF(T x) {
		return 6 * x - 20;
	}
	const auto seg = std::make_pair(-1, -0.5);
}

namespace test2 {
	T F(T x) {
		return 3 * x * x - std::cos(PI * x) * std::cos(PI * x);
	}
	T dF(T x) {
		return 6 * x + 2 * PI * std::cos(PI * x) * std::sin(PI * x);
	}
	T ddF(T x) {
		return 6 - 2 * PI * PI * std::sin(PI * x) * std::sin(PI * x) + 2 * PI * PI * std::cos(PI * x) * std::cos(PI * x);
	}
	//const auto seg = std::make_pair(0.2, 0.4);
	const auto seg = std::make_pair(-0.4, -0.2);
}

using namespace test2;

int main() {
	std::cerr << "Xn\t\tf(Xn)\t\tacc\n";
	std::cout << std::fixed << std::setprecision(8);
	std::cerr << std::fixed << std::setprecision(8);
	//auto res = find_by_relaxation(F, dF, seg, 0.00001);
	auto res = find_by_Newton(F, dF, ddF, seg, 0.00001);
	for (const auto& x : res) {
		std::cout << x.first << '\t' << F(x.first) << '\t' << x.second << '\n';
	}
}