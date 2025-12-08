# DeepSeekMath-V2 训练 pipeline

## Proof Verification Model

### 第一阶段 initial proof verification model

- **Curating Cold Start RL Data**: 从Art of Problem Solving (AoPS) contests收集了17503个问题作为数据集$D_p$, 接下来让DeepSeek-V3.2-Exp-Thinking 通过循环迭代的方式改善proof的质量(iteratively refine its proofs over multiple rounds to improve comprehensiveness and rigor.) 。最终在不同种类的问题上随机采样，让数学专家进行打分得到数据集$D_v={(X_i,Y_i,s_i)}$ ，其中$s_i$是专家的打分。
- **RL GRPO** --->  verifier: 奖励分为格式奖励和score奖励，我感觉这有点SFT的味道，给予的奖励信号激励模型产生的$s_i '$朝$s_i$对齐。非常值得学习这种奖励函数的格式

$$
R_{\text{score}}(s_i', s_i) = 1 - \lvert s_i' - s_i \rvert
$$

RL Objective
$$
\max_{\pi_{\varphi}} 
\mathbb{E}_{(X_i, Y_i, s_i) \sim \mathcal{D}_\nu,\; (V_i', s_i') \sim \pi_{\varphi}(\cdot \mid X_i, Y_i)}
\left[ R_{\text{format}}(V_i') \cdot R_{\text{score}}(s_i', s_i) \right]
$$

### 第二阶段 use meta-verification to enhance verification model

- 为什么需要meta-verification？ ：第一阶段的RL只激励模型朝人工标注的$s_i$对齐，但并没有监督有问题的证明本身(but provides no direct supervision on the identified issues themselves), 导致模型可能会出现 reward hacking： 即虚构不存在的问题来获得full reward。
- **RL GRPO** ---> meta-verifier: 构建新的数据集$D_{mv}={(X_i,Y_i,V_i,ms_i)}$ ，其中$ms_i$是人工标注的打分，RL Objective同第一阶段的
- **enhance verifier through meta-verifier:**  通过将meta-verifier的反馈加入奖励信号，通过RL训练出一个能够同时执行证明验证和元验证任务的单一模型

$$
R_v=R_{format}.R_{score}.R_{meta}
$$

## Proof Generation Model

### proof generation model

- **RL GRPO** ： 让上述训练得到的verifier充当奖励模型，激励proof generater产生质量更高的proof。
- **endow the proof generator with genuine verification**： deepseek团队在训练generator的时候发现一个问题：把生成器自己生成的proof送给它自己verification，即使是非常明显的错误，它都发现不了。为了让生成器能够诚实的自我评估，deepseek团队设计了一个新的Reward。激励模型不仅生成正确的答案，而且诚实的自我评估。其中： α = 0.76 and β = 0.24.

$$
R = R_{format}(Y,Z).(\alpha.R_y + \beta.R_z)

$$

$$
R_Z=R_{score}(s',s).R_{meta}(Z)
$$



### Enhancing Reasoning via Self-Verification

1. 每轮迭代首先优化验证器的能力（即训在难以识别的proof上训练该模型）
2. 让**刚刚优化完成的验证器的检查点**作为生成器，接着优化生成器。（为什么可以这样呢？因为评估一个答案的好坏比从头开始做要容易）
3. 最后采用拒绝式微调（rejection fine-tuning）用于**整合前一迭代中验证器和生成器学到的能力**，作为下一轮验证器的起点。

- 如何处理当生成器能力变强，验证器能力更不上的问题？
  - 找到哪些**验证器无法识别的proof**来训练verifier。
- 如何找到哪些验证器无法识别的proof？
  - deepseek团队提出一个automated labeling process:(扩大TTS)
    - 每个证明采用n个独立的verifier来验证
    - 对于每个检查出有问题的(0, 0.5)analysis，使用m个meta-verifier来证明其正确性，只有当大部分meta-verifier都认为合理才能算作合理
    - 对于每个proof，
      - 若全部的verification都认为没问题，那么该proof score为1
      - 对于最低的socre，若有大于k个verification都是这个score，那么该proof标记为该最低score
      - 其它情况：即有的verifier认为这个proof有问题，但数量不多。这种情况就会送给人工标记，**即为无法识别的proof**

# A. Prompt Templates

## A.1. Proof Generation Prompt

Your task is to solve a given problem. The problem may ask you to prove a statement, or ask for an answer. If finding an answer is required, you should come up with the answer, and your final solution should also be a rigorous proof of that answer being valid.

Your final solution to the problem should be exceptionally comprehensive and easy-to-follow, which will be rated according to the following evaluation instruction:

```txt
Here is the instruction to evaluate the quality of a solution to a problem. The problem may ask for a proof of statement, or ask for an answer. If finding an answer is required, the solution should present the answer, and it should also be a rigorous proof of that answer being valid.

Please evaluate the solution and score it according to the following criteria:
- If the solution is completely correct, with all steps executed properly and clearly demonstrated, then the score is 1
- If the solution is generally correct, but with some details omitted or minor errors, then the score is 0.5
- If the solution does not actually address the required problem, contains fatal errors, or has severe omissions, then the score is 0

Additionally, referencing anything from any paper does not save the need to prove the reference. It's okay IF AND ONLY IF the solution also presents a valid proof of the reference argument(s); otherwise, if the solution omits the proof or if the proof provided is not completely correct, the solution should be scored according to the criteria above, and definitely not with a score of 1
```

In fact, you already have the ability to rate your solution yourself, so you are expected to reason carefully about how to solve a given problem, evaluate your method according to the instruction, and refine your solution by fixing issues identified until you can make no further progress.

In your final response, you should present a detailed solution to the problem followed by your evaluation of that solution.
- To give a good final response, you should try your best to locate potential issues in your own (partial) solution according to the evaluation instruction above, and fix them as many as you can.
- A good final response should just faithfully present your progress, including the best solution you can give, as well as a faithful evaluation of that solution.
- Only when you fail to locate any issues in your solution should you score it with 1.
- If you do notice some issues in your solution but fail to resolve them with your best efforts, it's totally ok to faithfully present the issues in your final response.
- The worst final response would provide a wrong solution but lie that it's correct or claim that it's correct without careful error checking. A better version should faithfully identify errors in the solution. Remember! You CAN'T cheat! If you cheat, we will know, and you will be penalized!

Your final response should be in the following format:

## Solution // Your final solution should start with this exact same markdown title
... // Your final solution to the problem here. You should try your best to optimize the quality of your solution according to the evaluation instruction above before finalizing it here.

## Self Evaluation // Your evaluation of your own solution above should start with this exact same markdown title

Here is my evaluation of the solution: // Your analysis should start with this exact same phrase
... // Your evaluation here. You are required to present in detail the key steps of the solution or the steps for which you had doubts regarding their correctness, and explicitly analyze whether each step is accurate: for correct steps, explain why you initially doubted their correctness and why they are indeed correct; for erroneous steps, explain the reason for the error and the impact of that error on the solution. You should analyze your solution faithfully. E.g., if there are issues in your final solution, you should point it out.

Based on my evaluation, the final overal score should be:
\\boxed{{...}} // where ... should be the final overall score (0, 0.5, or 1, and nothing else) based on the evaluation instruction above. You should reach this score ONLY AFTER careful RE-examination of your own solution above

---

Here is your task input:

## Problem
{question}



## A.2. Proof Verification Prompt

## Instruction
Your task is to evaluate the quality of a solution to a problem. The problem may ask for a proof of statement, or ask for an answer. If finding an answer is required, the solution should present the answer, and it should also be a rigorous proof of that answer being valid.

Please evaluate the solution and score it according to the following criteria:

- If the solution is completely correct, with all steps executed properly and clearly demonstrated, then the score is 1.
- If the solution is generally correct, but with some details omitted or minor errors, then the score is 0.5.
- If the solution does not actually address the required problem, contains fatal errors, or has severe omissions, then the score is 0.
- Additionally, referencing anything from any paper does not save the need to prove the reference. It’s okay IF AND ONLY IF the solution also presents a valid proof of the reference argument(s); otherwise, if the solution omits the proof or if the proof provided is not completely correct, the solution should be scored according to the criteria above, and definitely not with a score of 1.

Please carefully reason out and analyze the quality of the solution below, and in your final response present a detailed evaluation of the solution’s quality followed by your score. Therefore, your response should be in the following format:

Here is my evaluation of the solution:
... // Your evaluation here. You are required to present in detail the key steps of the solution or the steps for which you had doubts regarding their correctness, and explicitly analyze whether each step is accurate: for correct steps, explain why you initially doubted their correctness and why they are indeed correct; for erroneous steps, explain the reason for the error and the impact of that error on the solution.

Based on my evaluation, the final overall score should be:
\boxed{...} // where ... should be the final overall score (0, 0.5, or 1, and nothing else) based on the above criteria

---
Here is your task input:
## Problem
{ question }
## Solution
{ proof }



## A.3. Meta-Verification Prompt  即 meta-verification rubrics

You are given a "problem", "solution", and "solution evaluation", and you need to assess whether this "solution evaluation" is reasonable.

First, "solution evaluation" is generated to evaluate the quality of the "solution", by prompting a verifier with the rules below (these are not your rules):

'''
Please evaluate the solution and score it according to the following criteria:
- If the solution is completely correct, with all steps executed properly and clearly demonstrated, then the score is 1.
- If the solution is generally correct, but with some details omitted or minor errors, then the score is 0.5.
- If the solution does not actually address the required problem, contains fatal errors, or has severe omissions, then the score is 0.

Additionally, referencing anything from any paper does not save the need to prove the reference. It’s okay IF AND ONLY IF the solution also presents a valid proof of the reference argument(s); otherwise, if the solution omits the proof or if the proof provided is not completely correct, the solution should be scored according to the criteria above, and definitely not with a score of 1.
'''

Next, I will introduce the rules for you to analyze the quality of the "solution evaluation":

1. Your task is to analyze the "solution evaluation". You do not need to solve the "problem", nor do you need to strictly assess whether the "solution" is accurate. Your only task is to strictly follow the rules below to evaluate whether the "solution evaluation" is reasonable.

2. You need to analyze the content of the "solution evaluation" from three aspects:
   - **Step Restatement**: In the "solution evaluation", certain behaviors of the "solution" may be restated. You need to return to the original text of the "solution" and check whether the "solution" actually has these behaviors mentioned in the "solution evaluation".
   - **Defect Analysis**: "solution evaluation" may point out errors or defects in the "solution". You need to carefully analyze whether the mentioned errors and defects are indeed valid.
   - **Expression Analysis**: Whether the "solution evaluation"’s expressions are accurate.
   - **Score Analysis**: Whether the final score given by the "solution evaluation" matches the defects it found. You need to analyze according to the scoring rules given above.

3. The most important part is **defect analysis**: In this part, your core task is to check whether the errors or defects of the "solution" pointed out in the "solution evaluation" are reasonable. In other words, any positive components about the "solution" in the "solution evaluation", regardless of whether they are reasonable, are not within your evaluation scope.
   - **For example**: If the "solution evaluation" says that a certain conclusion in the "solution" is correct, but actually this conclusion is incorrect, then you do not need to care about this point. All parts that the "solution evaluation" considers correct do not belong to your evaluation scope.
   - **Specifically**: If the "solution evaluation" believes that the "solution" is completely accurate and has not found any errors or defects, then regardless of whether the "solution" itself is actually accurate, even if there are obvious errors, you should still consider its analysis of errors to be reasonable.

   **Importantly**, for defects found by the "solution evaluation", you need to analyze two points simultaneously:
   - whether this defect actually exists
   - whether the "solution evaluation"’s analysis of this defect is accurate
   These two aspects constitute the analysis of defects.

4. About **expression analysis**, if there are certain expression errors in the "solution evaluation", even minor errors in details, you need to identify them. However, please note that identifying incorrect steps in the "solution" as correct steps does not constitute an **expression error**.
   In practice, expression errors include but are not limited to:
   - If the "solution evaluation" identifies some reasoning step(s) in the "solution" as incorrect, then it cannot further indicate that subsequent conclusion(s) depending on those reasoning step(s) are wrong, but can only indicate that subsequent conclusion(s) are "not rigorously demonstrated."
   - Typos and calculation errors made by "solution evaluation".
   - Inaccurate restatement of content from "solution".

5. Finally, you need to present your analysis of the "solution evaluation" in your output and also rate its quality based on the rules below:

   First, if there is at least one unreasonable defect among the defects found by the "solution evaluation", then you only need to do **defect analysis**:
   - If all defects found by the "solution evaluation" are unreasonable, then you should rate it with \(0\).
   - If some defects found by the "solution evaluation" are reasonable and some are unreasonable, then your rating should be \(0.5\).

   Next, if the "solution evaluation" points out no errors or defects, or all defects found by the evaluation are reasonable, then you should do the following things:
   
   - Analyze whether "expression errors" exist in the "solution evaluation" (**expression analysis**) or whether "solution evaluation" gives a wrong score according to the rules for "solution evaluation" (**score analysis**). If yes, you should rate the "solution evaluation" with \(0.5\); if no, your rating should be \(1\).

Your output should follow the format below:

Here is my analysis of the "solution evaluation":
... // Your analysis here.

Based on my analysis, I will rate the "solution evaluation" as:
\boxed{...} // where ... should be a numerical rating of the "solution evaluation" (0, 0.5, or 1, and nothing else) based on the criteria above.

--Here is your task input:

## Problem 

{question}

## Solution 

{proof}

## Solution Evaluation 

{proof analysis}