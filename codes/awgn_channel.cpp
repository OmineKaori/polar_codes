#include <iostream>
#include <cmath>
#include <limits>
#include <random>
#include "polar_codes.hpp"
using namespace std;

double generate_noise (double sigma) {
    random_device seed_gen;
    default_random_engine engine(seed_gen());
    normal_distribution<> dist(0, sigma);

    return dist(engine);
}

void generate_received_word (int *x, int *y, double sigma) {
    int d[N];
    double w[N];

    for ( int i = 0; i < N; i++ ) {
        d[i] = (*x == 0) ? 1 : -1;
        x++;
    }

    for ( int i = 0; i < N; i++ ) {
        w[i] = d[i] + generate_noise(sigma);
    }

    for ( int i = 0; i < N; i++ ) {
        *y = ( w[i] > 0 )? 1 : 0;
        y++;
    }
}
