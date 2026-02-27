#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <algorithm>

using namespace std;

class Solution {
public:
    double minTime(int n, int k, int m, vector<int>& time, vector<double>& mul) {
        // Precalculate sizes (popcount) and max crossing times for all 2^n sub-groups
        vector<int> max_t(1 << n, 0);
        vector<int> popc(1 << n, 0);
        for(int i = 0; i < (1 << n); ++i) {
            popc[i] = __builtin_popcount(i);
            int mx = 0;
            for(int j = 0; j < n; ++j) {
                if((i >> j) & 1) mx = max(mx, time[j]);
            }
            max_t[i] = mx;
        }
        
        // Initialize distances array to "infinity"
        vector<vector<vector<double>>> dist(1 << n, vector<vector<double>>(2, vector<double>(m, 1e18)));
        dist[0][0][0] = 0.0; // Starting locally at base camp
        
        // Priority Queue elements format: {current_time, mask, boat_position, current_stage}
        using State = tuple<double, int, int, int>;
        priority_queue<State, vector<State>, greater<State>> pq;
        pq.emplace(0.0, 0, 0, 0);
        
        int target_mask = (1 << n) - 1;
        
        while(!pq.empty()) {
            auto [d, mask, boat, stage] = pq.top();
            pq.pop();
            
            // If all individuals safely crossed, we arrived at the globally optimal answer
            if (mask == target_mask) return d;
            
            // Skip processing an outdated queue element
            if (d > dist[mask][boat][stage]) continue;
            
            if (boat == 0) { // Boat at base camp: Sending a group to destination
                int avail = target_mask ^ mask;
                // Iterate through all sub-groups available at the base camp
                for (int sub = avail; sub > 0; sub = (sub - 1) & avail) {
                    if (popc[sub] <= k) {
                        double trip_time = max_t[sub] * mul[stage];
                        int steps = floor(trip_time + 1e-9);
                        int next_stage = (stage + steps) % m;
                        int next_mask = mask | sub;
                        double next_d = d + trip_time;
                        
                        if (next_d < dist[next_mask][1][next_stage]) {
                            dist[next_mask][1][next_stage] = next_d;
                            pq.emplace(next_d, next_mask, 1, next_stage);
                        }
                    }
                }
            } else {         // Boat at destination: Individual has to return
                for (int i = 0; i < n; ++i) {
                    if ((mask >> i) & 1) { // They must logically be present at the destination
                        double trip_time = time[i] * mul[stage];
                        int steps = floor(trip_time + 1e-9);
                        int next_stage = (stage + steps) % m;
                        int next_mask = mask ^ (1 << i);
                        double next_d = d + trip_time;
                        
                        if (next_d < dist[next_mask][0][next_stage]) {
                            dist[next_mask][0][next_stage] = next_d;
                            pq.emplace(next_d, next_mask, 0, next_stage);
                        }
                    }
                }
            }
        }
        
        return -1.0; // It's impossible to transport all individuals
    }
};