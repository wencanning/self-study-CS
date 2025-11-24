# verl 关键参数解析

The official document: https://verl.readthedocs.io/en/v0.5.x/examples/config.html#

- actor_rollout_ref前缀代表着**Common config for actor, rollout and reference model**

## verl 训练pipeline



![image-20251124190754468](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20251124190754468.png)

This figure illustrates how the configurations affect the training.

### Step 1 采样阶段

- **data.train_batch_size** : 选择多少个promt
- **actor_rollout_ref.rollout.n**:  每个prompt采样多少个回答(> 1 for grpo)

采样阶段一共要生成: `data.train_batch_size * rollout.n `条数据

### Step 2 计算Log Prob

- **ator_rollout_ref.rollout.log_prob_micro_batch_size_per_gpu**: 每次为多少条数据计算log prob，for actor

一共需要为`data.train_batch_size * rollout.n `条数据计算log prob，但显存一次性不能计算这么多条，故每次只计算`rollout.log_prob_micro_batch_size_per_gpu`条数据，循环多次算完

### Step 3 计算 ref Log Prob

- **ator_rollout_ref.ref.log_prob_micro_batch_size_per_gpu**: 每次为多少条数据计算log prob，for ref

### Step 4 模型更新(Update Actor)

- **actor_rollout_ref.actor.ppo_mini_batch_size**: 虽然选择了data.train_batch_size个问题来采用，但每次只选择其中ppo_mini_batch_size个问题来进行更新梯度
  - 即需要对 `actor.pop.mini_batch_size * rollout.n`条数据计算梯度
- **actor.ppo_micro_batch_size_per_gpu**: 梯度累计的微批次大小。
  - 需要对`actor.pop.mini_batch_size * rollout.n / 显卡数量`数据进行反向传播，但太大显存会爆炸，故可设置小的micro batch size来累计梯度。

故当显存比较小时,可将`actor_rollout_ref.actor.ppo_micro_batch_size_per_gpu`等**micro_batch_size**参数设置的小一点或者为1.

##  GRPO

有许多grpo训练脚本在`examples/grpo_trainer/`目录下

- **actor_rollout_ref.actor.use_kl_loss：** to use kl loss in actor. When used, we are not applying KL in the reward function.
  - 默认为False，对GRPO而言需要改为True。
- **actor_rollout_ref.actor.kl_loss_coef：** The coefficient of kl loss. Default is 0.001.

- **algorithm.use_kl_in_reward**:  默认为False 、
  - 对经典PPO，需要改为True。