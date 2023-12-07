//#pragma GCC optimize("O0")
#include <iostream>
#include <chrono>
#include <string>
#include <random>
#include <thread>
#include <execution>
#include <algorithm>

using namespace std;

class Timer {
private:
    string name;

    using ClockType = chrono::high_resolution_clock;
    using DurationType = chrono::duration<double, milli>;
    chrono::time_point<ClockType> start;

    inline chrono::time_point<ClockType> now() const {
        return ClockType::now();
    }
public:
    Timer(string name) : start(now()), name(name) {}

    ~Timer() {
        auto end = now();
        DurationType duration = end - start;
        cout << name << " took " << duration.count() << "ms to calculate" << "\n";
    }
};


vector<int> generateRandomVector(int n) {
    vector<int> v(n);
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, 1000000000);
    for (int i = 0; i < n; ++i) {
        v[i] = dist(rng) % 1000000;
    }
    return v;
}

void scalar_mul_without_policy(vector<int>& v, vector<int>& u) {
    Timer t("scalar_mul_without_policy");
    transform_reduce(v.begin(), v.end(), u.begin(), 0, plus<>(), multiplies<>());
}


//void scalar_mul_with_policy_par(vector<int>& v, int threshold, int newValue) {
//    Timer t("scalar_mul_with_policy_par");
//    scalar_mul(execution::par, v.begin(), v.end(), [threshold](int x) { return x > threshold; }, newValue);
//}
//
//void scalar_mul_with_policy_seq(vector<int>& v, int threshold, int newValue) {
//    Timer t("scalar_mul_with_policy_seq");
//    scalar_mul(execution::seq, v.begin(), v.end(), [threshold](int x) { return x > threshold; }, newValue);
//}

void scalar_mul_custom(vector<int>& v, vector<int>& u) {
    Timer t("scalar_mul_custom");
    int result = 0;
    for (int i = 0; i < v.size(); ++i) {
        result += v[i] * u[i];
    }
}

void scalar_mul_custom_multithreaded(vector<int>& v, vector<int>& u, int threads) {
    Timer t("scalar_mul_custom_multithreaded threads: " + to_string(threads));
    vector<thread> workers;
    int chunkSize = v.size() / threads;
    vector<int> results(threads);
    for (int i = 0; i < threads; ++i) {
        workers.emplace_back([i, chunkSize, &v, u, &results]() {
            int start = i * chunkSize;
            int end = std::min((int)v.size(), (i + 1) * chunkSize);
            int result = 0;
            for (int j = start; j < end; ++j) {
                result += v[j] * u[j];
            }
            results[i] = result;
        });
    }
    int result = 0;
    for (auto& worker : workers) {
        worker.join();
    }

    for (int i = 0; i < threads; ++i) {
        result += results[i];
    }

}


int main() {
    int n = 10000000;

    vector<int> v = generateRandomVector(n);
    vector<int> v2 = generateRandomVector(n);
    scalar_mul_without_policy(v, v2);
//    scalar_mul_with_policy_par(v2, threshold, newValue);
//    scalar_mul_with_policy_seq(v3, threshold, newValue);
    scalar_mul_custom(v, v2);
    scalar_mul_custom_multithreaded(v, v2, 1);
    scalar_mul_custom_multithreaded(v, v2, 2);
    scalar_mul_custom_multithreaded(v, v2, 4);
    scalar_mul_custom_multithreaded(v, v2, thread::hardware_concurrency());

    return 0;
}
