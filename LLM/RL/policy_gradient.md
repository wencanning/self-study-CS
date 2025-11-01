# Policy Gradient

目标，最大化total reward的期望：
$$
\bar{R}_{\theta} = \sum_{\tau} R(\tau) p_{\theta} (\tau) = \mathbb{E} _ {\tau \sim p _ {(\theta)}} R(\tau)
$$
对于最大化目标函数，可使用梯度上升算法：
$$
\theta = \theta + \eta \nabla \bar{R} (\theta)
$$
接下来我们展开来计算$\nabla \bar{R}(\theta)$:
$$
\begin{aligned}
\nabla \bar{R} (\theta) &= \nabla \sum R(\tau) p_{\theta} (\tau) \\
&= \sum R(\tau) \nabla p _ \theta(\tau) \\
&= \sum R(\tau) p_{\theta} \nabla \log p_{\theta}(\tau) \\
&= \mathbb{E} _ {\tau \sim p_{\theta}(\tau)} [R(\tau) \nabla \log p_\theta (\tau)] \\



\end{aligned}
$$
对于$p_{\theta}(\tau)$ ，我们可以展开计算：
$$
\begin{aligned}

p_{\theta}(\tau) &= p(s_1) p_{\theta}(a_1|s_1)p(s_2|s_1,a_1)p_{\theta}(a_2|s_2)p(s_3|s_2,a_2).... \\
&= p(s_1)\prod_{t=1}^{T} p_{\theta}(a_t|s_t)p(s_{t+1}|s_t,a_t) 

\end{aligned}
$$
强化学习有3个组成部分: 演员(actor)、环境和奖励函数。在这里要搞清楚，我们不能控制环境，环境的相关分布是和$\theta$无关的。同样的道理，$R(\tau)$和$\theta$也是无关的。

那么$\nabla\log p_{\theta}(\tau)$我们就能展开计算了。
$$
\begin{aligned}

\nabla \log p _ {\theta} (\tau) &= \nabla p(s_1) + \nabla\sum_{t=1}^T \log p_{\theta}(a_t|s_t) + \nabla \sum_{t=1}^T p(s_{t+1}|s_t,a_t) \\
&=\nabla \sum_{t=1}^T \log p_{\theta}(a_t|s_t) \\
&=\sum_{t=1}^T \nabla\log p_{\theta}(a_t|s_t)

\end{aligned}
$$
将上式带入目标函数的梯度中：
$$
\begin{aligned}
\nabla \bar{R}( \theta ) &= \mathbb{E}_{\tau \sim p _ {\theta} (\tau)}[R(\tau) \sum_{t=1}^T \nabla \log p_{\theta} (a_t|s_t)] \\
&\approx \frac{1}{N}\sum_{n=1}^N \sum_{t=1}^{T_n} R(\tau ^ n) \nabla \log p _ {\theta} (a_t^n|s_t^n)
\end{aligned}
$$


## 添加 baseline

如果都是$R(\tau)$正数，根据我们的梯度公式，前面的系数全部是正数。就代表每次更新参数会试图让所有的$p(a|s)$ 增加。这是我们可以考虑添加一个baseline：b。这样就可以让$R(\tau)-b$有正有负，此时公式变为：
$$
\nabla \bar{R}( \theta ) \approx \frac{1}{N}\sum_{n=1}^N \sum_{t=1}^{T_n} (R(\tau ^ n)-b)\nabla \log p _ {\theta} (a_t^n|s_t^n)
$$


## 分配合适的分数

所有的状态-动作对就使用同样的奖励项进行加权，这显然是不公平的。

