# LLM Log

**Setup:**
- Model(s): Gemini 3.1 Pro
- Tool / harness: Antigravity IDE (VS Code) agent
- IDE / editor: VS Code
- Did you paste this assignment document into it?: Yes, Parts A-E were pasted entirely.

**The Session:**
- We had a session consisting of roughly 13 prompts total to reach working code.
- We first mistakenly attempted to modify `part5.cpp` over several prompts because we didn't know `part52.cpp` was the final code, adjusting speed and fixing Windows terminal rendering flickering issues. 
- Once we realized `part52.cpp` was the target, the AI read the file and I sent 2 prompts to port the multiplayer changes: "make 52 file work thats final code" and "implement it".
- What code was given to the LLM: The whole repository was available to the LLM agent to read autonomously, but it specifically focused on reading the entire `part5.cpp` and `part52.cpp` files.
- The AI correctly implemented double buffering/string buffers to prevent flickering and utilized `<random>` to randomize the spawns so they were not parallel, and isolated the input for WASD and Arrows.
