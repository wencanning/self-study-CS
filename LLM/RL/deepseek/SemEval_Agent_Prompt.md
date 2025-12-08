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

## Analysis// Your final analysis should start with this exact same markdown title

Here is my analysis of the text classification task:// Your analysis should start with this exact same phrase

...// Your analysis here.

Based on my analysis , the final label should be: ... // where ... should be the final label based on your analysis.

---

Here is your task input:

## Part of the interview

Q. Yet over the last several months, we've seen many members of your own party in Congress embroiled in one scandal or another and all too ready to blame somebody else, whether prosecutors or Democrats or even the media. So I'm wondering, why do you think it is so many people in your own party have failed to live up to the standards of the responsibility era?

A.If any person in any party fails to live up to high standards, they ought to be held to account, Richard. It's important for there to be trust in the Halls of Congress and in the White House and throughout government. People got to trust elected leaders in order for democracy to work to its fullest extent. And I fully expect people to be held to account if there's wrongdoing, just like I expect corporate executives to be held to account for wrongdoing, just like I expect people throughout our society to be held to account for wrongdoing.People do have to take responsibility for the decisions they make in life. I take responsibility for the decisions I make. I also understand that those of us in positions of responsibility have the duty to bring honor to the offices we hold. People don't have to agree with somebody's opinion—there's all kinds of opinions here—but in order to make this country work and to make democracy succeed, there's got to be high standards, and people must be held to account to achieve those standards.I thank you for your time. See you on the campaign trail.

## Question

Why do you think it is so many people in your own party have failed to live up to the standards of the responsibility era?



# Verification Prompt

## Instruction

Your task is to critically evaluate the accuracy and reasoning of a text classification assignment. The "Problem" consists of an interview question and an answer. The "Solution" is a proposed classification of that answer into specific categories (e.g., Explicit, Dodging, Deflection, etc.), accompanied by a rationale.

You must act as a strict adjudicator and score the solution based on the following criteria:

- **Score 1 (Correct):** The classification label is indisputably the best fit according to the standard definitions. The accompanying rationale provides robust textual evidence, explicitly linking the interviewee's specific wording to the category's criteria, and successfully rules out other similar categories (e.g., correctly distinguishing between 'Dodging' and 'Deflection').
- **Score 0.5 (Acceptable but Flawed):** The classification label is plausible or partially correct, but the reasoning is weak, generic, or misses key rhetorical nuances. Alternatively, the solution chooses a broader category (e.g., 'Implicit') when a more specific one (e.g., 'Deflection') would be more accurate.
- **Score 0 (Incorrect):** The classification label is objectively wrong, misinterprets the text, hallucinates information not present in the excerpt, or fails to address the specific question asked.

**Important Constraints:**
- Do not accept generic justifications. The solution must reference specific phrases or logic within the text.
- If the solution cites "Context" or "Intent," it must be grounded in the provided text, not assumed external knowledge.

Please carefully scrutinize the quality of the classification below. In your final response, present a detailed critique followed by your score.

Your response should follow this format:

**Here is my evaluation of the solution:**
... // Your evaluation here. You are required to:
1. **Verify the Label:** Is this strictly the best category? Compare it against potential alternative labels.
2. **Audit the Reasoning:** specific analyze whether the cited evidence supports the label. For correct steps, explain why the interpretation is sound. For errors, explain exactly where the logic diverges from the definitions or the text.

**Based on my evaluation, the final overall score should be:**
\boxed{...} // where ... should be the final overall score (0, 0.5, or 1) based on the criteria above.

---

Here is your task input:

## Problem (Context & Question/Answer)
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
