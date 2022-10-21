#include "Defines.h"
#include <iomanip>

using namespace Define;

int main() 
{
	int N = 1000000;
	auto sum = 0.0f;
	for (int i = 0; i < N; i++) 
	{
		RayVec3 d = random_unit_vector();
		auto cosine_squared = d.z * d.z;
		sum += cosine_squared / pdf(d);
	}
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "I = " << sum / N << '\n';
}