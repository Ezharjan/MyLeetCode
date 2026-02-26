# Mastering Large Scale Seat Reservations with Segment Trees [#2286]

Hello fellow coding enthusiasts!

Today I want to share a fascinating challenge I encountered that perfectly bridges the gap between managing simple 2D grids and handling massive scale data structures. It is a problem that asks us to build a ticketing system for a concert hall. While it sounds like a standard system design interview question, the constraints turn it into a rigorous algorithmic puzzle.

If you have ever wondered how to efficiently manage resources where you need to support both "contiguous blocks" and "fragmented bits" of data, this problem is a perfect case study. Let us dive in!

## The Challenge

Imagine you are managing a massive concert hall. This hall has `n` rows and `m` seats per row. We are talking potentially 50,000 rows and millions of seats. We need to support two specific types of booking requests:

1. **Gather:** A group of `k` people wants to sit together in the same row. They are picky! They want the smallest possible row number (closest to the stage). If there are multiple spots in that row, they want the lowest seat numbers. Also, they will only accept rows up to a specific limit, `maxRow`.
2. **Scatter:** A group of `k` people wants to watch the concert, but they do not mind sitting apart. As long as you can find `k` seats anywhere from row 0 up to `maxRow`, they are happy. Again, we want to fill seats starting from the lowest rows and lowest seat numbers to keep things organized.

The catch? We need to handle up to 50,000 of these requests efficiently.

## Why a Simple Loop is Not Enough

At first glance, you might want to create a big 2D array (or a vector of vectors) to represent the seats.

For a **Gather** request, you could iterate through every row from 0 to `maxRow`. inside each row, you check if there is enough space.
For a **Scatter** request, you could iterate through every row, summing up available seats until you reach `k`.

**The Problem with this approach:**
The constraints are huge. `n` is up to 50,000. The number of seats `m` can be up to 1 billion (so we cannot even allocate a full 2D grid in memory!). Even if we just store "used seats per row," the number of operations kills us.
If we have 50,000 queries, and for each query we scan 50,000 rows, that is roughly 2.5 billion operations. In C++, that is likely to result in a Time Limit Exceeded (TLE).

We need a way to query ranges of rows and update them much faster than checking them one by one. We need **Logarithmic Time Complexity**, or `O(log n)`.

## The Solution: Segment Trees

To achieve that sweet `O(log n)` speed, we can use a **Segment Tree**.

If you are new to Segment Trees, think of them as a way to summarize an array. The leaves of the tree represent our rows. The internal nodes represent a summary of a range of rows.

For this specific problem, we need to answer two different questions quickly, so we will actually maintain **two** pieces of information in our Segment Tree nodes:

### 1. The Min Tree (For Gather)

For the `gather` operation, we need to find a single row that has at least `k` seats available.
To find this quickly, each node in our Segment Tree will store the **minimum number of used seats** in its range.

* Why minimum used? Because if the minimum number of used seats in a range of rows is, say, 5, and the total capacity is 10, we know at least one row in that range has 5 seats free.
* Actually, it is often easier to store the **minimum used seats**. If we need `k` seats, we are looking for a row where `used + k <= m`.
* By querying this tree, we can ask: "What is the first row in the range `[0, maxRow]` where the used count is small enough to fit my group?"

### 2. The Sum Tree (For Scatter)

For the `scatter` operation, we do not care about contiguous space. We just need to know if the *total* number of empty seats in the range `[0, maxRow]` is greater than or equal to `k`.
To answer this, each node in our Segment Tree will store the **sum of used seats** in its range.

* If we know the total capacity of rows `0` to `maxRow` (which is simply `(maxRow + 1) * m`), and we know the total used seats from our tree, we can instantly calculate if there are enough empty seats.

## Designing the Data Structure

Let us structure our `BookMyShow` class. We will use a standard array based implementation for the Segment Tree.

### Variables

* `n`: Number of rows.
* `m`: Number of seats per row.
* `min_tree`: An array where index `1` is the root. Stores the minimum used seats for a range.
* `sum_tree`: An array storing the total used seats for a range.
* `start_row`: This is a special optimization pointer. It points to the first row that is not completely full.

### Why do we need `start_row`?

This is a crucial optimization for the `scatter` operation. `scatter` fills rows greedily from the top. Over time, rows 0, 1, 2, etc., will become completely full. We do not want to waste time traversing the Segment Tree for rows that we know are full. By keeping a `start_row` index, we can skip everything before it.

## The Logic Flow

### Initialization

We build a Segment Tree covering indices `0` to `n`. Initially, all rows have 0 used seats. So our trees are initialized to 0.

### Operation: Gather

**Goal:** Find the smallest row index `r` such that `r <= maxRow` and the row has `k` contiguous seats.

1. We start at the root of the Segment Tree.
2. We ask: "Does the left child (representing the first half of the rows) have a row with enough space?"
* We check this by looking at `min_tree[left_child]`. If `min_used <= m - k`, then yes, a candidate exists in the left half!


3. If the left child works (and is within our `maxRow` limit), we go left.
4. If the left child does not have space, we check the right child.
5. If we reach a leaf node, we have found our row! We record the row index.
6. **Update:** Once we find the row, we update its used count by adding `k`. We then update the values up the tree for both `min_tree` and `sum_tree`.
7. **Return:** We return `{row, initial_seat}`.

### Operation: Scatter

**Goal:** Book `k` seats across rows `0` to `maxRow`.

1. **Check Feasibility:** First, we query the `sum_tree` for the range `[0, maxRow]`. This gives us the total `used` seats. The total capacity is `(maxRow + 1) * m`.
* If `capacity - used < k`, we return `False` immediately.


2. **Book the Seats:** If it is possible, we need to fill the seats.
* We start iterating from our cached `start_row`.
* For each row `i`, we calculate how many seats we can take: `available = m - used[i]`.
* We take `min(k, available)` seats from this row.
* We update the Segment Tree for this specific row (Point Update).
* We decrease `k` by the amount we took.
* If the row becomes full, we increment `start_row` so we never look at it again.
* We stop when `k` becomes 0.


3. **Return:** `True`.

## Implementation Details

Here is the complete solution code written in C++. I have removed standard macros to make it readable and focused on the logic.

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// We use long long for the sum tree because the total seats can exceed integer limits.
typedef long long ll;

class BookMyShow {
    int n;
    int m;
    
    // Segment Tree arrays
    // min_tree: stores the minimum number of 'used' seats in a range
    // sum_tree: stores the total sum of 'used' seats in a range
    vector<int> min_tree;
    vector<ll> sum_tree;
    
    // Optimization pointer
    int start_row;

    // Standard Segment Tree Update: Point update
    // Updates the value at index `idx` to `val`
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            min_tree[node] = val;
            sum_tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        // Aggregate values after child update
        min_tree[node] = min(min_tree[2 * node], min_tree[2 * node + 1]);
        sum_tree[node] = sum_tree[2 * node] + sum_tree[2 * node + 1];
    }

    // Query for gather: Find smallest row index in [0, maxRow] with used <= m - k
    int queryGather(int node, int start, int end, int k, int maxRow) {
        // Pruning: If the minimum used seats in this range is too high, 
        // no row here can accommodate k seats.
        if (min_tree[node] > m - k) return -1;
        
        // If current range starts beyond maxRow, ignore
        if (start > maxRow) return -1;

        // Leaf node: we found a valid row!
        if (start == end) return start;

        int mid = (start + end) / 2;
        
        // Prefer left child (smaller row index)
        int res = queryGather(2 * node, start, mid, k, maxRow);
        if (res != -1) return res;
        
        // If not found in left, try right child (only if it overlaps with valid range)
        if (mid < maxRow) {
            return queryGather(2 * node + 1, mid + 1, end, k, maxRow);
        }
        
        return -1;
    }

    // Query for scatter: Sum of used seats in range [0, maxRow]
    ll querySum(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return sum_tree[node];
        int mid = (start + end) / 2;
        return querySum(2 * node, start, mid, l, r) + querySum(2 * node + 1, mid + 1, end, l, r);
    }
    
    // Helper to store current usage of each row 
    // This avoids querying the tree just to get a single row's current value
    vector<int> row_usage;

public:
    BookMyShow(int n, int m) : n(n), m(m), start_row(0) {
        // 4 * n is the standard safe size for a segment tree array
        min_tree.resize(4 * n, 0);
        sum_tree.resize(4 * n, 0);
        row_usage.resize(n, 0);
    }
    
    vector<int> gather(int k, int maxRow) {
        // Find the first row that can fit k contiguous seats
        int r = queryGather(1, 0, n - 1, k, maxRow);
        
        if (r == -1) return {}; // No such row found
        
        int seat = row_usage[r];
        
        // Update state
        row_usage[r] += k;
        update(1, 0, n - 1, r, row_usage[r]);
        
        return {r, seat};
    }
    
    bool scatter(int k, int maxRow) {
        // 1. Check Feasibility using Sum Tree
        
        // Total capacity possible in the allowed rows
        ll capacity = (ll)(maxRow + 1) * m;
        
        // Total already used seats in the allowed rows
        ll used = querySum(1, 0, n - 1, 0, maxRow);
        
        // If we do not have enough total space, fail early.
        if (capacity - used < k) return false; 
        
        // 2. Perform Booking
        // Start from start_row (optimization to skip fully filled rows)
        for (int i = start_row; i <= maxRow && k > 0; ++i) {
            int current_used = row_usage[i];
            
            // If row is full, verify start_row pointer and continue
            if (current_used == m) {
                if (i == start_row) start_row++;
                continue;
            }
            
            // Calculate how many seats we can take from this row
            int available = m - current_used;
            int take = min(k, available);
            
            // Update the usage
            row_usage[i] += take;
            k -= take;
            
            // Update Segment Tree
            update(1, 0, n - 1, i, row_usage[i]);
            
            // If we just filled this row completely and it was the start_row,
            // move the pointer forward for future efficiency.
            if (row_usage[i] == m && i == start_row) {
                start_row++;
            }
        }
        
        return true;
    }
};

```

## Complexity Analysis

This is the part that proves why this solution works where others fail.

### Time Complexity

1. **Gather:**
* We perform a search down the Segment Tree. The height of the tree is `log n`.
* In the worst case, we visit nodes proportional to the height of the tree.
* Complexity: **O(log n)** per call.


2. **Scatter:**
* The feasibility check (`querySum`) takes **O(log n)**.
* The booking loop is interesting. Inside the loop, we perform an `update`, which is `O(log n)`. It looks like this could be `O(n log n)` in the worst case if `k` is huge.
* **However**, notice the `start_row` optimization. Each row is filled to capacity (`used == m`) exactly **once**. Once a row is full, `start_row` moves past it, and we never update that row again for the rest of the program's life.
* Therefore, across all calls to `scatter`, each row is updated a limited number of times. The amortized complexity is very low.
* Complexity: **O(log n)** amortized per call.



### Space Complexity

* We store the Segment Tree arrays which are size `4 * n`.
* We store the `row_usage` array which is size `n`.
* Total Space: **O(n)**.

## Final Thoughts

This problem is a fantastic example of how picking the right data structure simplifies complex requirements. By realizing that "contiguous availability" maps to a "minimum" query and "total availability" maps to a "sum" query, we turned a daunting simulation problem into a classic Segment Tree application.

The addition of the `start_row` pointer is a subtle but powerful technique often called "amortized analysis" or "two pointer optimization," ensuring that our solution doesn't just work theoretically but runs blazing fast in practice.

I hope this walkthrough helps you understand the power of Segment Trees in resource allocation problems. Happy coding!
