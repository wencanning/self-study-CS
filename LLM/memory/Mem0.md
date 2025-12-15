# Mem0总结

## Mem0 pipeline(Mem0 如何处理新到来的数据?）

<img src="D:\workplace\self-study-CS\LLM\memory\asset\mem0.png" alt="image-20251215183619050" style="zoom:50%;" />

- new message pair：$(m_{t-1}, m_t)$
  - 通常$m_{t-1}$为prompt，$m_t$为complement

### extraction phase

To establish appropriate context for memory extraction, the system employs **two complementary sources**:

- **a conversation summary $S$**: encapsulates the semantic **content of the entire conversation history**
  - To support context-aware memory extraction, we implement an **asynchronous** summary generation module that periodically refreshes the conversation summary. This component operates independently of the main processing pipeline.

- **a sequence of recent messages**$\{m_{t-m},m_{t-m+1},...,m_{t-2}\}$:
  - mem0把参数$m$设置为了10

最后将$S$和$\{m_{t-m},m_{t-m+1},...,m_{t-2}\}$打包送给LLM，提取出候选记忆: $\Omega=\{\omega_1,\omega_2,...,\omega_n\}$

## update phase

为每个候选记忆，利用vector embeddings，在数据库中筛选出前$s$个相似的记忆。接下来将这些记忆和其候选记忆通过‘tool call’送给LLM，让LLM决定执行什么动作。mem0把参数$s$设置为了10

- **ADD** for creation of new memories when no semantically equivalent memory exists

- **UPDATE** for augmentation of **existing memories** with complementary information

- **DELETE** for removal of memories contradicted by new information
- **NOOP** when the candidate fact requires no modification to the knowledge base

## Mem0 Graph pipeline