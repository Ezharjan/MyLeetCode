# Dynamic Programming as the Discrete Engine of ML Optimization

[Blog Post Link](https://leetcode.com/discuss/post/7539872/dynamic-programming-the-discrete-engine-o9ywa/)



If you strip away the LeetCode branding, Dynamic Programming (DP) is simply **optimization under constraints**.

In Machine Learning, we obsess over finding the global minimum of a loss function $L(\theta)$ or the global maximum of a reward function $J(\pi)$. We usually do this in continuous spaces using Calculus (Gradient Descent). DP is the exact same discipline, but applied to **discrete** or **sequential** spaces.

When you solve *Knapsack* or *Edit Distance*, you aren't just "finding a path." You are manually implementing an optimization solver. You are defining a loss landscape, identifying the optimal substructure, and efficiently computing the global optimum without searching the entire parameter space.

Here is how DP directly maps to the optimization engines powering modern ML.

---

## 1. Backpropagation is just DP on a Computational Graph

This is the most critical realization for any ML engineer. We think of **Backpropagation** as "calculus," but algorithmically, it is **Dynamic Programming**.

Consider a neural network as a Directed Acyclic Graph (DAG) of operations. To compute gradients for optimization, we use the Chain Rule. Naively applying the Chain Rule would involve re-calculating the same derivatives repeatedly (exponential complexity).

Instead, we perform a **top-down DP pass** (reverse-mode differentiation). We compute the gradient of the loss with respect to the output, memoize it, and propagate it backward.

* **DP State:** Gradient at node $i$, denoted $\frac{\partial L}{\partial x_i}$
* **Transition (Chain Rule):**

$$
\frac{\partial L}{\partial x_i}
=
\sum_{j \in \text{children}(i)}
\frac{\partial L}{\partial x_j}
\cdot
\frac{\partial x_j}{\partial x_i}
$$

### Pseudo-code: Backprop as DP

```python
# A simplified view of Autograd engines (e.g., PyTorch)
def backward_pass(node):
    # Base case: memoization
    if node.grad is not None:
        return node.grad

    current_grad = 0
    for parent in node.parents:
        parent_grad = backward_pass(parent)
        local_grad = parent_grad * local_derivative(parent, node)
        current_grad += local_grad

    node.grad = current_grad
    return current_grad
```

If you understand **Longest Path in a DAG**, you understand the engine training modern large language models.

---

## 2. The Viterbi Algorithm: Maximum Likelihood Estimation

In sequence modeling (NLP, speech), we often want the **most likely output sequence**:

$$
\hat{y} = \arg\max_y P(y \mid x)
$$

A naive search has complexity:

$$
O(|V|^T)
$$

where $|V|$ is vocabulary size and $T$ is sequence length — completely infeasible.

We solve this using the **Viterbi Algorithm**, which is DP on a lattice. It is used in HMMs, CTC (speech recognition), and probabilistic sequence models.

We define a value function:

$$
dp[t][s]
$$

representing the **maximum log-probability** of ending in state $s$ at time $t$.

### Pseudo-code: Viterbi Optimization

We work in log-space to convert products into sums and avoid numerical underflow.

```python
def viterbi_optimization(obs, states, start_p, trans_p, emit_p):
    T = len(obs)
    S = len(states)

    # DP table in log-space
    dp = [[-float('inf')] * S for _ in range(T)]

    # Base case (t = 0)
    for s in range(S):
        dp[0][s] = log(start_p[s]) + log(emit_p[s][obs[0]])

    # DP transitions
    for t in range(1, T):
        for s in range(S):
            best_prev = -float('inf')
            for prev_s in range(S):
                score = dp[t - 1][prev_s] + log(trans_p[prev_s][s])
                best_prev = max(best_prev, score)
            dp[t][s] = best_prev + log(emit_p[s][obs[t]])

    return max(dp[T - 1])
```

This is functionally identical to finding a **maximum-weight path in a DAG**.

---

## 3. Reinforcement Learning: From Discrete DP to Continuous Control

In Reinforcement Learning, optimization is defined via the **Bellman Optimality Equation**:

$$
V^*(s)

=

\max_a
\left(
R(s,a)
+
\gamma
\sum_{s'}
P(s' \mid s,a)
V^*(s')
\right)
$$

* In classic LeetCode DP, transitions are deterministic
* In RL, transitions are **stochastic**
* Small state spaces → **Value Iteration (exact DP)**
* Large state spaces → **function approximation (neural networks)**

We approximate the DP table with:

$$
Q(s,a;\theta) \approx Q^*(s,a)
$$

### Pseudo-code: Q-Learning (Approximate DP)

Deep Q-Learning minimizes the **Bellman error** — the violation of the DP recurrence.

```python
def compute_q_learning_loss(batch, model, target_model, gamma):
    states, actions, rewards, next_states, dones = batch

    # Left-hand side of Bellman equation
    current_q = model(states).gather(1, actions)

    # Right-hand side (DP target)
    with torch.no_grad():
        next_q_max = target_model(next_states).max(dim=1)[0]

    target_q = rewards + gamma * next_q_max * (1 - dones)

    loss = MSE(current_q, target_q)
    return loss
```

---

## Summary

When you practice Dynamic Programming, you are not just learning array manipulation — you are learning **sequential optimization**.

* **Memoization** → caching activations and gradients
* **Recurrence relations** → Bellman equations and loss functions
* **State compression** → latent representations

Every time you optimize a DP solution from $O(2^N)$ to $O(N^2)$, you are sharpening the exact same instinct required to optimize Transformer inference, training loops, and reinforcement learning agents.
