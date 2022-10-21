#include "Defines.h"
#include <iomanip>

using namespace Define;

int main()
{
    int N = 1000000;
    auto sum = 0.0f;
    for (int i = 0; i < N; i++)
    {
        auto x = sqrt(random(0.0f, 4.0f));
        sum += x * x / pdf_t(x);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}
