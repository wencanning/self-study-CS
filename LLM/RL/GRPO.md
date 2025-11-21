# GRPO

- 了解DeepSeek R1训练pipeline
- 了解GRPO训练过程及其公式
  - 手写GRPO公式
  - 一步一步debug GRPO trainer

## 1 DeepSeek R1 training pipeline

![image-20251106195803825](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20251106195803825.png)

### 1.1 Cold Start（冷启动）

deepseek团队尝试先让model先具备基础推理能力来避免base model在RL训练早期出现的不稳定冷启动现象，同时加速RL阶段的收敛过程和更好的推理性能。

### 1.2 **Reasoning-oriented Reinforcement Learning**

在cold start data sft之后，接着对模型进行和r1-zero一样的大规模强化学习。在追求推准确性的同时，在奖励函数上添加了语言一致性性的限制，防止在CoT出现多种语言混用的情况

### 1.3 **Rejection Sampling and Supervised Fine-Tuning**

- **Reasoning data** ：通过上一阶段训练的RL model，设计reasoning prompts通过拒绝采样来收集trajetories。
- **Non-Reasoning data**： 部分数据沿用 DeepSeek-V3 dataset。其它数据：对于复杂任务，让v3-base model先生成CoT再收集答案，对于简单任务则直接生成答案。

### 1.4 **Reinforcement Learning for all Scenario**

在这个阶段我们将得到最终的DeepSeek R1。

首先对V3-base model在1.3收集到的数据集进行2个epoch的SFT，再在包含有多种推理数据的data上进行RL训练，得到最终的R1.

### 1.5 Distillation: Empower Small Models with Reasoning Capabilit

让小模型在1.3收集到的数据直接SFT。研究发现，这种直接蒸馏方法能显著提升小型模型的推理能力

## 2 GRPO

###2.1 GRPO in TRL

官方文档: https://huggingface.co/docs/trl/grpo_trainer

注意几个点

- reward function 的 input 会根据 dataset 的format有所区别, 注意区分 standard format 还是 conversational format.
- vllm 加速推理有两种形式