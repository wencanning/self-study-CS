# Answer Generation Prompt

Your task is to execute a precise text classification analysis. Based on the provided interview excerpt, characterize the nature of the interviewee's response to the specified question. You are required to assign the response to exactly one of the nine categories listed below, strictly adhering to the definitions to distinguish between nuanced labels.

- Explicit: The information requested is explicitly stated (in the requested form)

- Implicit: The information requested is given, but without being explicitly stated (not in the requested form)

- Dodging: Ignoring the question altogether

- Deflection: Starts on topic but shifts the focus and makes a different point than what is asked

- Partial/half-answer: Offers only a specific component of the requested information.

- General: The information provided is too general/lacks the requested specificity.

- Declining to answer: Acknowledge the question but directly or indirectly refusing to answer at the moment

- Claims ignorance: The answerer claims/admits not to know the answer themselves.

- Clarification: Does not provide the requested information and asks for clarification.

In your final response, you are required to substantiate your classification with a detailed analysis that explicitly links the interviewee's wording to the category definition.

To ensure the highest accuracy, you should critically evaluate your initial classification against the provided definitions to rule out potential misinterpretations or edge cases. Your final response should present your most refined conclusion, offering a definitive category supported by a faithful and transparent justification of your reasoning.

Your final response should be in the following format:

## Analysis//Your final analysis should start with this exact same markdown title

Here is my analysis of the text classification task: //Your analysis should start with this exact same phrase.

...//Your analysis here.

Based on my analysis , the final label should be: ... //where ... should be the final label based on your analysis.

---

Here is your task input:

## Part of the interview

{}

{}

## Question

{}





# Verification Prompt

Your task is to critically evaluate the accuracy and reasoning of a text classification assignment. The "Part of the interview"  and "Question" consists of an interview question and an answer. The "Solution" is a proposed classification of that answer into specific categories (e.g., Explicit, Dodging, Deflection, etc.), accompanied by a rationale.

You must act as a strict adjudicator and determine whether the solution is **Correct** or **Incorrect** based on the following strict criteria:

- **Correct:** The classification label is indisputably the best fit according to the standard definitions. AND, the accompanying rationale provides robust textual evidence, explicitly linking the interviewee's specific wording to the category's criteria. It must successfully rule out other similar categories (e.g., distinguishing 'Dodging' from 'Deflection').
- **Incorrect:** The solution falls into ANY of the following failure modes:
    1. The label is objectively wrong.
    2. The label is plausible, but the reasoning is weak, generic, or fails to cite specific evidence.
    3. The analysis hallucinates information not present in the text.
    4. A broader category (e.g., 'Implicit') is chosen when a more specific one (e.g., 'Deflection') would be more accurate.

**Important Constraints:**

- Do not accept generic justifications. The solution must reference specific phrases or logic within the text.
- If the solution cites "Context" or "Intent," it must be grounded in the provided text, not assumed external knowledge.

Please carefully scrutinize the quality of the classification below. In your final response, present a detailed critique followed by your final verdict.

Your response should follow this format:

## Evaluation//Your final evaluation should start with this exact same markdown title

Here is my evaluation of the solution: //Your evaluation should start with this exact same phrase.
... //Your evaluation here. You are required to:

1. **Verify the Label:** Is this strictly the best category? Compare it against potential alternative labels.
2. **Audit the Reasoning:** Specifically analyze whether the cited evidence supports the label. For correct classifications, explain why the interpretation is sound. For errors, explain exactly where the logic diverges from the definitions or the text.

Based on my evaluation, the final verdict is: ... //Output strictly either "Correct" or "Incorrect"

---

Here is your task input:

## Part of the interview

{}

{}

## Question

{}

## Solution (Proposed Classification)

{}











# Refinement Prompt

{}

## Preliminary Assessment to Refine

Below are sample classification(s) generated for this interview excerpt, accompanied by a critical evaluation. Your objective is to derive a superior, definitive classification. You should achieve this by correcting the misinterpretations identified in the evaluation(s) while retaining any accurate observations regarding the interviewee's rhetorical strategy or linguistic patterns.

### Draft Analysis
{}

### Critical Review
{}

## Final Instruction
// Your final response must strictly follow the output format defined in the original instruction. Crucially, you must include a `## Analysis` section that explicitly links the text evidence to the chosen category definition, resolving any prior ambiguities.
