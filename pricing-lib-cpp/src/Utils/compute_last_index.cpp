#include "compute_last_index.hpp"

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
