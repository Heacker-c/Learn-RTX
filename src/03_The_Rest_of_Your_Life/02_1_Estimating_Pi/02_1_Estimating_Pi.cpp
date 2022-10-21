#include "Defines.h"
#include <iomanip>

using namespace Define;

int main() 
{
	int N = 150;
	int inside_circle = 0;
	for (int i = 0; i < N; i++) 
	{
		auto x = random(-1.0f, 1.0f);
		auto y = random(-1.0f, 1.0f);
		if (x * x + y * y < 1.0f)
			++inside_circle;
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "Estimate of Pi = " << 4.0f * inside_circle / N << '\n';
}
