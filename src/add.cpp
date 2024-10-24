#include <algorithm>
#include <complex>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

#define DATA_POINTER(vec) (&(*(vec.begin())))

double 
mean(std::vector<double>::iterator begin, std::vector<double>::iterator end) 
{
    double res = 0;
    size_t N = 0;
    while(begin != end)
    {
        res += *begin;
        N ++;
        begin ++;
    }
    return res / N;
}


void 
add(std::complex<double> *v1
    , std::complex<double> *v2 
    , std::complex<double> *res 
    , size_t L)
{
#pragma GCC ivdep
    for(size_t i = 0; i < L; i++)
    {
            res[i] = v1[i] + v2[i];
    }
}

int
main(int argc, char ** argv)
{
    size_t L = 8*8*16*16 * 4 * 3 * 4;
    size_t n_trials = 100;

    std::vector<std::complex<double>> v1(L, 1);
    std::vector<std::complex<double>> v2(L, 3);
    std::vector<std::complex<double>> res(L);

    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()};
    std::uniform_real_distribution<double> dist{-1, 1};
    auto gen = [&](){
                   return std::complex<double>(dist(mersenne_engine), dist(mersenne_engine));
               };
    

    std::cout << "Benchmarking using " << n_trials << " trials" << std::endl;
    std::vector<double> results(n_trials, 0);
    for(size_t i = 0; i < n_trials; i++)
    {
        std::generate(v1.begin(), v1.end(), gen);
        std::generate(v2.begin(), v2.end(), gen);
        auto start = std::chrono::high_resolution_clock::now();
        add(DATA_POINTER(v1), DATA_POINTER(v2), DATA_POINTER(res), L);
        auto stop = std::chrono::high_resolution_clock::now();
        auto elapsed = stop - start;
        results[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count() / 1000.0 / 1000.0 / 1000.0;
    }
    std::cout << "Results:" << std::endl;

    std::sort(results.begin(), results.end());

    // Size of data (LOAD + STORE)
    auto data_size = v1.size() * sizeof(std::complex<double>) * 3;


    double mean_top_20 = mean(results.begin(), results.begin() + 20);

    auto peak_troughput = data_size / results[0] / 1024 / 1024 / 1024;
    auto mean_troughput = data_size / mean_top_20 / 1024 / 1024 / 1024;

    std::cout << "Data size    [MiB]: " << data_size / 1024 / 1024 << std::endl;
    std::cout << "Best time     [ms]: " << results[0] * 1000 << std::endl;
    std::cout << "Mean time (20)[ms]: " << mean_top_20 * 1000 << std::endl;
    std::cout << "Max thrput [GiB/s]: " << peak_troughput << std::endl;
    std::cout << "Mean thrpt [GiB/s]: " << mean_troughput << std::endl;

    return 0;
}
