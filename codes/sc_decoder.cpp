#include <iostream>
#include <cmath>
#include <vector>
#include "polar_codes.hpp"
using namespace std;

void recursively_calc_llr (int lambda, int phi, vector<vector<double>>& LLR, vector<vector<vector<int>>> C ) {
    if (lambda == 0) { return; }

    int psi = phi / 2;

    if (phi % 2 == 0) { recursively_calc_llr(lambda - 1, psi, LLR, C); }

    for (int beta = 0; beta < pow(2, n - lambda); beta++) {
        if (phi % 2 == 0) {
            if ( 40.0 >  max(abs(LLR[lambda-1][2*beta]), abs(LLR[lambda-1][2*beta+1])) ) {
                LLR[lambda][beta] = log((exp(LLR[lambda-1][2*beta] + LLR[lambda-1][2*beta+1])+1) / (exp(LLR[lambda-1][2*beta])+exp(LLR[lambda-1][2*beta+1])) );
            } else {
                LLR[lambda][beta] =  double( (LLR[lambda-1][2*beta] < 0 ? -1 : 1) *
                    (LLR[lambda-1][2*beta+1] < 0 ? -1 : 1) *
                    min(abs(LLR[lambda-1][2*beta]), abs(LLR[lambda-1][2*beta+1])));
            }
        } else {
            double up = C[lambda][beta][0];
            LLR[lambda][beta] = (1-2*up) * LLR[lambda-1][2*beta] + LLR[lambda-1][2*beta+1];
        }
    }
}

void recursively_update_c (int lambda, int phi, vector<vector<vector<int>>>& C) {
    int psi = phi / 2;

    for (int beta = 0; beta < pow(2, n - lambda); beta ++) {
		C[lambda - 1][2 * beta][psi % 2] = C[lambda][beta][0] ^ C[lambda][beta][1];
		C[lambda - 1][2 * beta + 1][psi % 2] = C[lambda][beta][1];
    }

    if (psi % 2 == 1) {
        recursively_update_c(lambda - 1, psi, C);
    }
}

void sc_decoder (int *decoded_codeword, vector<int> frozen_bit_indices, vector<double> input_llr) {
    vector<vector<double>> LLR(n+1, vector<double>(N));
    vector<vector<vector<int>>>  B(n+1, vector<vector<int>>(N, vector<int>(2)));

    // 復号
    for (int i = 0; i < N; i++) {
        LLR[0][i] = input_llr[i];
    }

    for (int phi = 0; phi < N; phi++) {
        recursively_calc_llr(n, phi, LLR, B);

        if (find(frozen_bit_indices.begin(), frozen_bit_indices.end(), phi) != frozen_bit_indices.end()) {
            B[n][0][phi % 2] = 0;
        } else {
            if (LLR[n][0] > 0) {
                B[n][0][phi % 2] = 0;
            } else {
                B[n][0][phi % 2] = 1;
            }
        }

        if (phi%2 == 1) {
            recursively_update_c(n, phi, B);
        }
    }

    for (int i = 0; i < N; i++) {
        *decoded_codeword = B[0][i][0];
        decoded_codeword++;
    }
}
