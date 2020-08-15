#include <iostream>
#include <cmath>
#include <vector>
#include "polar_codes.hpp"
#include "construction_bhattacharyya.cpp"
#include "awgn_channel.cpp"
#include "sc_decoder.cpp"
using namespace std;

int I = 10000;
double R = 0.5;
int K = N * R;
double n = log2(N);

int x[N] = {}; // 符号語
int y[N]; // 受信語

int main () {
    double EbN0 = 0; // 0~4

    for (double EbN0 = 0; EbN0 < 5; EbN0 = EbN0 + 0.5) {
        double S = pow(10, EbN0*R/10);
        double sigma = sqrt( pow( 10, -EbN0/10 ) / (2 * R) );
        int count = 0;
        double BER;

        vector<double> input_llr(N);
        int decoded_codeword[N];

        for (int i = 0; i < I; i++) {
            vector<int> reliability_sequence = bhattacharyya_bounds(S);
            vector<int> frozen_bit_indices(reliability_sequence.begin(), reliability_sequence.begin() + (N - K));
            vector<int> information_bit_indices(reliability_sequence.begin() + (N - K), reliability_sequence.end());

            // AWGN通信路に通す
            generate_received_word(x, y, sigma);

            // 復号
            for (int i = 0; i < N; i++) { input_llr[i] = 2 * y[i] / sigma; }
            sc_decoder(decoded_codeword, frozen_bit_indices, input_llr);

            // BERの計算
            for (int i = 0; i < N; i++) { if (x[i] != decoded_codeword[i]) {count++;} }
        }

        BER = double(count) / double(I * N);
        printf("Eb/N0 = %.1f, BER = %.20f\n", EbN0, BER);
    }

    return 0;
}
