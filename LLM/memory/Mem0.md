# Mem0总结

## Mem0 pipeline(Mem0 如何处理新到来的数据?）

<img src=".\asset\mem0.png" alt="image-20251215183619050" style="zoom: 67%;" />

- new message pair：$(m_{t-1}, m_t)$
  - 通常$m_{t-1}$为prompt，$m_t$为complement

### extraction phase

To establish appropriate context for memory extraction, the system employs **two complementary sources**:

- **a conversation summary $S$**: encapsulates the semantic **content of the entire conversation history**
  - To support context-aware memory extraction, we implement an **asynchronous** summary generation module that periodically refreshes the conversation summary. This component operates independently of the main processing pipeline.

- **a sequence of recent messages**$\{m_{t-m},m_{t-m+1},...,m_{t-2}\}$:
  - mem0把参数$m$设置为了10

最后将$S$和$\{m_{t-m},m_{t-m+1},...,m_{t-2}\}$打包送给LLM，提取出候选记忆: $\Omega=\{\omega_1,\omega_2,...,\omega_n\}$

### update phase

为每个候选记忆，利用vector embeddings，在数据库中筛选出前$s$个相似的记忆。接下来将这些记忆和其候选记忆通过‘tool call’送给LLM，让LLM决定执行什么动作。mem0把参数$s$设置为了10

- **ADD** for creation of new memories when no semantically equivalent memory exists

- **UPDATE** for augmentation of **existing memories** with complementary information

- **DELETE** for removal of memories contradicted by new information
- **NOOP** when the candidate fact requires no modification to the knowledge base

## Mem0 Graph pipeline

<img src=".\asset\mem0_graph.png" alt="image-20251216161556255" style="zoom:67%;" />

### extraction phase(two-stage)

Relationships in our system are structured as triplets in the form: $(v_s,r,v_d)$

- **entity extractor:** an entity extractor module processes the **input text** to identify a set of entities along with their corresponding types.

- **relationship generator**: a relationship generator component derives meaningful connections **between these entities**, establishing a set of relationship triplets that capture the semantic structure of the information.

### update Phase

- for each **new relationship triple**: we compute embeddings for both source and destination entities, then search for existing nodes with semantic similarity **above a defined threshold ‘t’.**Based on node existence
  - create both nodes
  - create only one node
  - use existing nodes
- **conflict detection**: identifies potentially conflicting existing relationships when new information arrives.
- **update resolver**: determines if certain relationships should be **obsolete**, **marking them as invalid** rather than physically removing them to enable temporal reasoning.

### how to retrieval ? a dual-approach

会retrieval到两种信息

- **entity-centric method:** 首先分析query中的核心entity, 接下来根据语义相似度找到知识图谱上相识的结点, 分析它们的出边和入边然后提取出一个子图.

- **semantic triplet approach**: 
  - encoding the entire query as a dense embedding vector
  - matched against textual encodings of each relationship triplet in the knowledge graph
  - calculates fine-grained similarity scores between the query and all available triplets, returning only those that exceed a configurable  relevance threshold, ranked in order of decreasing similarity.

mem0使用 Neo4j 作为底层的图数据库

