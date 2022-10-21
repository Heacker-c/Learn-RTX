#include "Defines.h"
#include <iomanip>

using namespace Define;

int main()
{
	int N = 1;
	auto sum = 0.0f;
	for (int i = 0; i < N; i++)
	{
		auto x = pow(random(0.0f, 8.0f), 1.0f / 3.0f);
		sum += x * x / pdf_t(x);
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "I = " << sum / N << '\n';
}