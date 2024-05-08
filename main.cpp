#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Base class for RNGs
class RNG {
public:
    virtual double generate() = 0;
    virtual ~RNG() {}
};

// Inversive Congruential Generator (ICG)
class ICG : public RNG {
private:
    unsigned long m, a, c;
    unsigned long x0;

public:
    ICG(unsigned long m, unsigned long a, unsigned long c, unsigned long x0) {
        this->m = m;
        this->a = a;
        this->c = c;
        this->x0 = x0;
    }

    double generate() override {
        x0 = (a * (x0 + c) % m) % m;
        return static_cast<double>(x0) / m;
    }
};

// Multiple Recursive Generator (MRG)
class MRG : public RNG {
private:
    vector<unsigned long> seeds;
    vector<unsigned long> a;
    unsigned long m;

public:
    MRG(vector<unsigned long> seeds, vector<unsigned long> a, unsigned long m) {
        this->seeds = seeds;
        this->a = a;
        this->m = m;
    }

    double generate() override {
        unsigned long x = 0;
        for (unsigned long i = 0; i < seeds.size(); i++) {
            x = (a[i] * seeds[i]) % m;
            seeds[i] = x;
        }
        return static_cast<double>(x) / m;
    }
};

// Lagged Fibonacci Generator (LFG)
class LFG : public RNG {
private:
    unsigned long m, a, b, n;
    vector<unsigned long> x;

public:
    LFG(unsigned long m, unsigned long a, unsigned long b, unsigned long n, vector<unsigned long> x) {
        this->m = m;
        this->a = a;
        this->b = b;
        this->n = n;
        this->x = x;
    }

    double generate() override {
        unsigned long x_new = (x[n - a] + x[n - b]) % m;
        x.push_back(x_new);
        x.erase(x.begin());
        return static_cast<double>(x_new) / m;
    }
};

// Mid-Square Method (MSM)
class MSM : public RNG {
private:
    unsigned long seed;
    unsigned long digits;

public:
    MSM(unsigned long seed, unsigned long digits) {
        this->seed = seed;
        this->digits = digits;
    }

    double generate() override {
        unsigned long squared = seed * seed;
        unsigned long len = static_cast<unsigned long>(log10(squared)) + 1;
        unsigned long start = (len - digits) / 2;
        unsigned long end = start + digits;
        seed = squared / static_cast<unsigned long>(pow(10, start));
        seed %= static_cast<unsigned long>(pow(10, digits));
        return static_cast<double>(seed) / pow(10, digits);
    }
};

// Kolmogorov-Smirnov (KS) Test
double ksTest(vector<double> data) {
    unsigned long n = data.size();
    double d_max = 0.0;

    sort(data.begin(), data.end());

    for (unsigned long i = 1; i <= n; i++) {
        double d1 = abs(data[i - 1] - (static_cast<double>(i - 1) / n));
        double d2 = abs(data[i - 1] - (static_cast<double>(i) / n));
        d_max = max(d_max, max(d1, d2));
    }

    return d_max;
}

int main() {
    unsigned long m, a, c, x0, n, seed, digits;
    vector<unsigned long> seeds, a_mrg, x_lfg;
    unsigned long b; // Declare b here

    cout << "Enter parameters for Inversive Congruential Generator (ICG):" << endl;
    cout << "m: "; cin >> m;
    cout << "a: "; cin >> a;
    cout << "c: "; cin >> c;
    cout << "x0: "; cin >> x0;

    ICG icg(m, a, c, x0);

    cout << "\nEnter parameters for Multiple Recursive Generator (MRG):" << endl;
    cout << "Number of seeds: "; unsigned long num_seeds; cin >> num_seeds;
    cout << "Enter seeds: ";
    for (unsigned long i = 0; i < num_seeds; i++) {
        unsigned long seed;
        cin >> seed;
        seeds.push_back(seed);
    }
    cout << "Enter coefficients: ";
    for (unsigned long i = 0; i < num_seeds; i++) {
        unsigned long coeff;
        cin >> coeff;
        a_mrg.push_back(coeff);
    }
    cout << "m: "; cin >> m;

    MRG mrg(seeds, a_mrg, m);

    cout << "\nEnter parameters for Lagged Fibonacci Generator (LFG):" << endl;
    cout << "m: "; cin >> m;
    cout << "a: "; cin >> a;
    cout << "b: "; cin >> b; // Use the declared variable b here
    cout << "n: "; cin >> n;
    cout << "Enter initial seeds: ";
    for (unsigned long i = 0; i < n; i++) {
        unsigned long seed;
        cin >> seed;
        x_lfg.push_back(seed);
    }

    LFG lfg(m, a, b, n, x_lfg);

    cout << "\nEnter parameters for Mid-Square Method (MSM):" << endl;
    cout << "Seed: "; cin >> seed;
    cout << "Number of digits: "; cin >> digits;

    MSM msm(seed, digits);

    vector<double> icg_data, mrg_data, lfg_data, msm_data;
    unsigned long num_samples = 10000;

    cout << "\nGenerating " << num_samples << " random numbers..." << endl;

    for (unsigned long i = 0; i < num_samples; i++) {
        icg_data.push_back(icg.generate());
        mrg_data.push_back(mrg.generate());
        lfg_data.push_back(lfg.generate());
        msm_data.push_back(msm.generate());
    }

    cout << "\nKolmogorov-Smirnov Test Results:" << endl;
    cout << "ICG: " << ksTest(icg_data) << endl;
    cout << "MRG: " << ksTest(mrg_data) << endl;
    cout << "LFG: " << ksTest(lfg_data) << endl;
    cout << "MSM: " << ksTest(msm_data) << endl;

    return 0;
}
