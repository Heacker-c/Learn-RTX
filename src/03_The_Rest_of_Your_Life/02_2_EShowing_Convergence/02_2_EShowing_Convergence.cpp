#include "Defines.h"
#include <iomanip>

using namespace Define;

int main() {
	int inside_circle = 0;
	int runs = 0;
	std::cout << std::fixed << std::setprecision(12);
	while (true) 
	{
		runs++;
		auto x = random(-1, 1);
		auto y = random(-1, 1);
		if (x * x + y * y < 1)
			inside_circle++;

		if (runs % 100000 == 0)
			std::cout << "Estimate of Pi = " << 4.0 * inside_circle / runs << std::endl;
	}
}

