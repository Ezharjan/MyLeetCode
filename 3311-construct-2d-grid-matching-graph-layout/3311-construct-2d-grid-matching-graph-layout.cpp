#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> constructGridLayout(int n, vector<vector<int>>& edges) {
        // Step 1: Build the adjacency list graph
        vector<vector<int>> adj(n);
        for(auto& e : edges){
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        // Find the minimum degree to identify corner configurations
        int min_deg = n;
        for(int i = 0; i < n; ++i){
            min_deg = min(min_deg, (int)adj[i].size());
        }
        
        vector<int> corners;
        for(int i = 0; i < n; ++i){
            if(adj[i].size() == min_deg){
                corners.push_back(i);
            }
        }
        
        // Step 2: Use BFS from a chosen corner to find distances and the shortest path map
        int c = corners[0];
        vector<int> dist(n, -1);
        vector<int> parent(n, -1);
        queue<int> q;
        
        q.push(c);
        dist[c] = 0;
        
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int v : adj[u]){
                if(dist[v] == -1){
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        
        int target_corner = -1;
        if(min_deg == 1){
            target_corner = corners[1]; // 1D grid layout, only exactly two corners exist
        } else {
            // Find the closest other corner resolving to form a boundary row
            int min_d = 1e9;
            for(int x : corners){
                if(x != c && dist[x] < min_d){
                    min_d = dist[x];
                    target_corner = x;
                }
            }
        }
        
        // Trace back the shortest path resolving exclusively to the boundary layer mapped row
        vector<int> first_row;
        int curr = target_corner;
        while(curr != -1){
            first_row.push_back(curr);
            curr = parent[curr];
        }
        reverse(first_row.begin(), first_row.end());
        
        vector<vector<int>> grid;
        grid.push_back(first_row);
        
        vector<bool> visited(n, false);
        for(int x : first_row){
            visited[x] = true;
        }
        
        // Step 3: Populate and construct subsequent rows downward
        while(true){
            vector<int> last_row = grid.back();
            vector<int> new_row;
            bool has_next = false;
            
            for(int u : last_row){
                int next_u = -1;
                for(int v : adj[u]){
                    if(!visited[v]){
                        next_u = v;  // The next respective downwards vertical neighbor
                        break;
                    }
                }
                if(next_u != -1){
                    has_next = true;
                }
                new_row.push_back(next_u);
            }
            
            // If the last tracked row nodes don't have available adjacent neighbors, the grid bottom is reached 
            if(!has_next) break;
            
            for(int x : new_row){
                if(x != -1) visited[x] = true;
            }
            grid.push_back(new_row);
        }
        
        return grid;
    }
};