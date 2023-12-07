#pragma GCC optimize("O0")
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
        v[i] = dist(rng);
    }
    return v;
}

void replace_if_without_policy(vector<int>& v, int threshold, int newValue) {
    Timer t("replace_if_without_policy");
    replace_if(v.begin(), v.end(), [threshold](int x) { return x > threshold; }, newValue);
}

//void replace_if_with_policy_par(vector<int>& v, int threshold, int newValue) {
//    Timer t("replace_if_with_policy_par");
//    replace_if(execution::par, v.begin(), v.end(), [threshold](int x) { return x > threshold; }, newValue);
//}
//
//void replace_if_with_policy_seq(vector<int>& v, int threshold, int newValue) {
//    Timer t("replace_if_with_policy_seq");
//    replace_if(execution::seq, v.begin(), v.end(), [threshold](int x) { return x > threshold; }, newValue);
//}

void replace_if_custom(vector<int>& v, int threshold, int newValue) {
    Timer t("replace_if_custom");
    for (int& x : v) {
        if (x > threshold) {
            x = newValue;
        }
    }
}

void replace_if_custom_multithreaded(vector<int>& v, int threshold, int newValue, int threads) {
    Timer t("replace_if_custom_multithreaded threads: " + to_string(threads));
    vector<thread> workers;
    int chunkSize = v.size() / threads;
    for (int i = 0; i < threads; ++i) {
        workers.emplace_back([i, chunkSize, &v, threshold, newValue]() {
            int start = i * chunkSize;
            int end = (i + 1) * chunkSize;
            for (int j = start; j < end; ++j) {
                if (v[j] > threshold) {
                    v[j] = newValue;
                }
            }
        });
    }
    for (thread& t : workers) {
        t.join();
    }
}


int main() {
    int n = 10000000;
    int threshold = 500000;
    int newValue = 0;

    vector<int> v = generateRandomVector(n);
    replace_if_without_policy(v, threshold, newValue);
//    replace_if_with_policy_par(v2, threshold, newValue);
//    replace_if_with_policy_seq(v3, threshold, newValue);
    replace_if_custom(v, threshold, newValue);
    replace_if_custom_multithreaded(v, threshold, newValue, 1);
    replace_if_custom_multithreaded(v, threshold, newValue, 2);
    replace_if_custom_multithreaded(v, threshold, newValue, 4);
    replace_if_custom_multithreaded(v, threshold, newValue, thread::hardware_concurrency());

    return 0;
}
