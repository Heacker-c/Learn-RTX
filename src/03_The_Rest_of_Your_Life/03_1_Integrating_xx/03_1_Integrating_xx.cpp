#include "Defines.h"
#include <iomanip>

using namespace Define;

int main() 
{
	int N = 1000000;
	auto sum = 0.0f;
	for (int i = 0; i < N; i++) 
	{
		auto x = random(0.0, 2.0);
		sum += x * x;
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "I = " << 2.0 * sum / N << '\n';
}