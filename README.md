# Stack Canaries Observations: Which Functions Are Considered Risky?

## 1. Introduction
So far, I explored the behavior of NX, ASLR, and PIE. These are defenses that make exploitation unreliable.

However, stack canaries take a different approach. They detect corruption before control flow is hijacked.

Stack canaries can be enabled at compile time, but depending on compiler options, they may be inserted into some functions but not others.

I learned that this decision is often heuristic-based.
This made me wonder: what kinds of functions are actually protected?

## 2. What Stack Canaries Actually Do
Stack canaries are values inserted into the stack frame before control data such as the return address.

When a buffer overflow occurs, these values may also be overwritten. If the value changes, the program detects the corruption and terminates execution.

There are three commonly discussed types of stack canaries: terminator canaries, random canaries, and random XOR canaries.

Among them, random canaries are the most commonly used on modern systems, and this experiment focuses on observing their behavior.

In this experiment, I changed compiler options and observed how stack canaries behave across multiple functions.

## 3. Experiment Setup
I compiled a C program with different stack protector options and compared the results across functions with different characteristics.

The tested functions included:
- character arrays (small and large)
- integer arrays
- pointer variables
- local structures
- use of strcpy
- simple local integer
- simple math
- global variable only
- heap only
- never called by main function

Canary-related instructions can be observed in the generated assembly, as shown below:
![mov, sub, je,callの行](image.jpg)

Before running the experiment, I made several predictions.

I expected functions using pointers or with large buffers to be consistently protected by stack canaries.

I also expected that character arrays and integer arrays would more likely be protected with -fstack-protector-strong.

At the same time, I thought some functions using local structures, simple local integer, simple math, global variable only and heap only might not always trigger protection unless -fstack-protector-all was used.

For additional curiosity, I also included a function that was never called from main().

## 4. Observations
The experimental results are shown below:
| Function | protector | strong | all |
|---|---|---|---|
| char_array_small | No | yes | yes |
| char_array_large | yes | yes | yes |
| int_array | No | yes | yes |
| pointer_only | No | yes | yes |
| local_struct | yes | yes | yes |
| uses_strcpy | yes | yes | yes |
| simple_local_int | no | no | yes |
| simple_math | no | no | yes |
| global_only | no | no | yes |
| heap_only | no | no | yes |
| never_called | yes | yes | yes |

I learned that stack canary insertion is determined heuristically, and these results revealed several interesting patterns:

- Small character arrays, integer arrays, and pointer-related functions showed similar protection behavior under -fstack-protector-strong.
- As I expected, simple local integer, simple math, global-only, and heap-only functions were not protected unless -fstack-protector-all was used, suggesting they were not considered sufficiently risky by the default heuristics.
- Interestingly, the function never called by the main function always had the canary protection.

## 4.1. -fstack-protector-explicit experiment
During this research, I noticed another option called -fstack-protector-explicit. According to the official document, it only protects the functions with stack_protect attribute.

I compiled the program using this option as well and observed the results. The functions with stack_protect attribute were protected, and other functions did not receive canary protection. 

Compared to the heuristic-based options, this approach felt much more explicit and predictable. But I wondered when we need this option.

One possible use case is performance-sensitive systems such as IoT devices, firmware, or microcontrollers. When stack canaries work, overhead happens. Most laptop users would not notice this overhead much, but for performance-sensitive systems, it could be a big difference. That’s why this option is needed that developers can determine which function is protected or not.

## 4.2. main() without buffer
Actually this experiment started only with multiple functions in main(). Initially, main() did not receive canary protection. I thought this was normal behavior at first. However, after adding a local character buffer (`char buf[32]`), stack canary protection was inserted into main() as well.

## 5. Key Insight
- I learned that GCC heuristics may consider buffer size when deciding whether to insert stack canaries.
- From what I observed, -fstack-protector-strong appears to behave similarly to -fstack-protector-all in many practical cases.
- Result of -fstack-protector-explicit was very different from other options, but I understand it is needed for specific environment.

## 6. Conclusion
This experiment helped me better understand which kinds of functions are considered risky by GCC heuristics.

I started exploring defense mechanisms to better understand how modern systems protect themselves. Through this experiment, I also became more aware of environments such as firmware and IoT systems, which previously felt far removed from my learning.

I would like to continue exploring low-level security mechanisms and mitigation techniques in future experiments.

