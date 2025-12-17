# GAM总结

## GAM pipeline

![image-20251217114005698](.\asset\GAM_pipeline.png)

### Memorizer

- **Memorizing**:　当新的session到来时, 会将$s_i$和$m_i$一起送给LLM生成新的memo $u_i$. 最后会用$u_i$来更新$m_i$得到$m_{i+1}$

  $Memorizer.memorize(s_i,m_i)->u_i; m_i+{u_i}->m_{i+1}$

- **Paging:** 首先根据$s_i$和$m_i$创建一个header, 用来修饰这个new page. 随后将下header和session组成一个new page,放到page-store后面.

  $Memorizer.page(s_i, m_i) → h_i; {header : h_i, content : s_i} → p_i; p.append(p_i).$

### Researcher

**注意这是Researcher而不是之前mem0和A-mem的updater!**

