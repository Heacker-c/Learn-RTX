#include "Defines.h"
#include <iomanip>

using namespace Define;

#define Random_Directions_Relative 0
#define Hemisphere1 0
#define Hemisphere2 1

#if Random_Directions_Relative
int main() {
	for (int i = 0; i < 200; i++) {
		auto r1 = random_float();
		auto r2 = random_float();
		auto x = cos(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
		auto y = sin(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
		auto z = 1 - 2 * r2;
		std::cout << x << " " << y << " " << z << '\n';
	}
}
#endif 

#if Hemisphere1
int main() {
	int N = 1000000;
	auto sum = 0.0f;
	for (int i = 0; i < N; i++) {
		auto r1 = random_float();
		auto r2 = random_float();
		auto x = cos(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
		auto y = sin(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
		auto z = 1 - r2;
		sum += z * z * z / (1.0f / (2.0f * pi));
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "Pi/2     = " << pi / 2 << '\n';
	std::cout << "Estimate = " << sum / N << '\n';
}
#endif 

#if Hemisphere2
int main() {
	int N = 1000000;

	auto sum = 0.0f;
	for (int i = 0; i < N; i++) {
		auto v = random_cosine_direction();
		sum += v.z * v.z * v.z / (v.z / pi);
	}

	std::cout << std::fixed << std::setprecision(12);
	std::cout << "Pi/2     = " << pi / 2 << '\n';
	std::cout << "Estimate = " << sum / N << '\n';
}
#endif 