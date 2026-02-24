class Solution {
    // Function to calculate the total number of set bits from 1 to N
    long long count_bits(long long N) {
        long long cnt = 0;
        for (int i = 0; i < 60; ++i) {
            long long block = 1LL << (i + 1);
            long long full = N / block;
            cnt += full * (1LL << i);
            long long rem = N % block;
            if (rem >= (1LL << i)) {
                cnt += rem - (1LL << i) + 1;
            }
        }
        return cnt;
    }

    // Function to calculate the sum of exponents (bit indices) from 1 to N
    long long sum_exponents(long long N) {
        long long sum = 0;
        for (long long i = 0; i < 60; ++i) {
            long long block = 1LL << (i + 1);
            long long full = N / block;
            long long cnt = full * (1LL << i);
            long long rem = N % block;
            if (rem >= (1LL << i)) {
                cnt += rem - (1LL << i) + 1;
            }
            sum += cnt * i;
        }
        return sum;
    }

    // Function to get the sum of exponents for the first X elements in big_nums
    long long get_f(long long X) {
        if (X == 0) return 0;
        
        long long low = 0, high = 1e15, N = 0;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (count_bits(mid) <= X) {
                N = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        
        long long sum = sum_exponents(N);
        long long R = X - count_bits(N);
        long long num = N + 1;
        
        for (int i = 0; i < 60 && R > 0; ++i) {
            if ((num >> i) & 1) {
                sum += i;
                R--;
            }
        }
        return sum;
    }

    // Fixed modular exponentiation to handle mod == 1
    long long power(long long base, long long exp, long long mod) {
        long long res = 1 % mod; // Fix: 1 % 1 returns 0 appropriately
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = res * base % mod;
            base = base * base % mod;
            exp /= 2;
        }
        return res;
    }

public:
    vector<int> findProductsOfElements(vector<vector<long long>>& queries) {
        vector<int> ans;
        for (auto& q : queries) {
            long long from = q[0], to = q[1], mod = q[2];
            
            // Get total exponent sum in the range [from, to]
            long long total_exponent = get_f(to + 1) - get_f(from);
            
            // Calculate (2^total_exponent) % mod
            ans.push_back(power(2, total_exponent, mod));
        }
        return ans;
    }
};