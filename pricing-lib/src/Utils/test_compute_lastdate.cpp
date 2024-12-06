#include <cmath>
#include <cassert>

double EPS = 1E-10;

int compute_last_index(double t, double T, int N)
{
    double dt = T / N;
    int nearest_index = std::round(t / dt);
    if (std::fabs(nearest_index * dt - t) < EPS)
    {
        return nearest_index;
    }
    else
    {
        return int(t / dt);
    }
}

int main()
{
    assert(compute_last_index(0.0, 1., 100) == 0);
    assert(compute_last_index(0.5, 1., 100) == 50);
    assert(compute_last_index(0.5 - EPS / 10, 1., 100) == 50);
    assert(compute_last_index(0.5 + EPS / 10, 1., 100) == 50);
    assert(compute_last_index(0.5 + 10 * EPS, 1., 100) == 50);
    assert(compute_last_index(0.5 - 10 * EPS, 1., 100) == 49);
    return 0;
}