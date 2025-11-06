# 🐍 Cyber-Neon Snake

A **modern, terminal-based Snake game** with **cyberpunk neon visuals**, built entirely in **C++**.  
The game runs directly in your console window with smooth animations, colorful gradients, and enhanced controls.

---

## ✨ Features

- 🟢 **Neon-Themed Interface** — dynamic color gradients for a futuristic look.  
- 🍎 **Three Fruit Types**:
  - `●` Normal fruit — +10 points  
  - `★` Bonus fruit — +25 points  
  - `✦` Speed fruit — +8 points and faster gameplay  
- 🚧 **Progressive Obstacles** — appear as you advance through levels.  
- 🌈 **Animated Tail Gradient** — snake body glows with fading neon tones.  
- 🎮 **Smooth Controls & Input Handling** — supports both arrow keys and WASD.  
- 💀 **Game Over / Pause / Restart** options built-in.  
- 🖥️ **Cross-platform** — works on Windows, Linux, and macOS terminals that support ANSI escape codes.

---

# ⚙️ Setup & Compilation

### 🪟 For Windows

1. **Install g++ (MinGW or similar).**
2. Place all source files (e.g., `part5.cpp`) in a folder.
3. Open **Command Prompt** or **PowerShell** in that folder.
4. Run the following commands:

   ```bash
   g++ part5.cpp -o part5
   chcp 65001
   .\part5.exe
