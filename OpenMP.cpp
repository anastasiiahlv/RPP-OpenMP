#include <iostream>
#include <vector>
#include <limits>
#include <omp.h>

using namespace std;

void generate_data(vector<unsigned>& v, unsigned n) {
    unsigned mul = 1664525, add = 1013904223, cur = 123456789;
    for (unsigned i = 0; i < n; i++) {
        v[i] = (cur = cur * mul + add) % 2000000001;
    }
}

void find_min_max(const vector<unsigned>& v, unsigned& global_min, unsigned& global_max) {
    global_min = numeric_limits<unsigned>::max();
    global_max = numeric_limits<unsigned>::min();

#pragma omp parallel
    {
        unsigned local_min = numeric_limits<unsigned>::max();
        unsigned local_max = numeric_limits<unsigned>::min();

#pragma omp for nowait
        for (int i = 0; i < static_cast<int>(v.size()); ++i) {
            if (v[i] < local_min) local_min = v[i];
            if (v[i] > local_max) local_max = v[i];
        }

#pragma omp critical
        {
            if (local_min < global_min) global_min = local_min;
            if (local_max > global_max) global_max = local_max;
        }
    }
}

int main() {
    unsigned n, num_threads;

    cout << "Enter array size: ";
    cin >> n;

    cout << "Enter number of threads: ";
    cin >> num_threads;

    omp_set_num_threads(num_threads);

    vector<unsigned> v(n);

    double start_gen_time = omp_get_wtime();
    generate_data(v, n);
    double end_gen_time = omp_get_wtime();
    cerr << (end_gen_time - start_gen_time) << " s - finished data generation\n";

    unsigned global_min, global_max;
    double start_time = omp_get_wtime();
    find_min_max(v, global_min, global_max);
    double end_time = omp_get_wtime();

    cout << "Global minimum: " << global_min << endl;
    cout << "Global maximum: " << global_max << endl;
    cerr << (end_time - start_time) << " s - finished computation\n";

    return 0;
}
