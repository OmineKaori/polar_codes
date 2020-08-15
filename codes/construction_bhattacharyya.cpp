#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include "polar_codes.hpp"
using namespace std;

template <typename Iter, typename Compare>
vector<int> argsort(Iter begin, Iter end, Compare comp)
{
	vector<pair<int, Iter>> pairList;
	vector<int> ret;

	int i = 0;
	for (auto it = begin; it < end; it++)
	{
		pair<int, Iter> pair(i, it);
		pairList.push_back(pair);
		i++;
	}

	stable_sort(pairList.begin(), pairList.end(),
		[comp](pair<int, Iter> prev, pair<int, Iter> next) -> bool
	{return comp(*prev.second, *next.second); }
	);

	for (auto i : pairList)
		ret.push_back(i.first);

	return ret;
}

vector<int> bhattacharyya_bounds (double S) {
    array<double, N> z;
    vector<int> indices;

    z[0] = exp(-S);

    for (int j = 1; j < n; j++) {
        int u = pow(2, j);

        for (int t = 0; t < (u/2 - 1); t++) {
            double T = z[t];
            z[t] = 2 * T - pow(T, 2);
            z[u/2 + t] = pow(T, 2);
        }
    }

    return argsort(z.begin(), z.end(), less<int>());
}
