//execution code below 3 lines
//g++ part1.cpp -o part1
//chcp 65001
//.\part1.exe
#include <bits/stdc++.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
using namespace std;
using namespace std::chrono;

//Terminal / ANSI helpers-
namespace T {
#ifdef _WIN32
bool enableVT() {
    // enable ANSI escape sequences on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    return SetConsoleMode(hOut, dwMode);
}
#endif
inline void esc(const string &s){ cout << "\x1b[" << s; }
inline void reset(){ cout << "\x1b[0m"; }
inline void clear(){ cout << "\x1b[2J"; }
inline void moveTo(int r,int c){ cout << "\x1b[" << r << ";" << c << "H"; }
inline void hideCursor(){ cout << "\x1b[?25l"; }
inline void showCursor(){ cout << "\x1b[?25h"; }
inline void flush(){ cout.flush(); }
}

//Non-blocking input
#ifdef _WIN32
static constexpr int KEY_UP = 1001, KEY_DOWN = 1002, KEY_LEFT = 1003, KEY_RIGHT = 1004;
int getKeyNonBlocking() {
    if (!_kbhit()) return 0;
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        int s = _getch();
        switch (s) {
            case 72: return KEY_UP; case 80: return KEY_DOWN;
            case 75: return KEY_LEFT; case 77: return KEY_RIGHT;
            default: return 0;
        }
    }
    return ch;
}
#else
struct Raw {
    termios orig{}; bool active=false;
    Raw(){ enable(); }
    void enable(){
        if (active) return;
        tcgetattr(STDIN_FILENO,&orig);
        termios raw = orig;
        raw.c_lflag &= ~(ICANON|ECHO);
        raw.c_cc[VMIN]=0; raw.c_cc[VTIME]=0;
        tcsetattr(STDIN_FILENO,TCSANOW,&raw);
        int flags = fcntl(STDIN_FILENO,F_GETFL,0);
        fcntl(STDIN_FILENO,F_SETFL,flags | O_NONBLOCK);
        active = true;
    }
    ~Raw(){ if(active) tcsetattr(STDIN_FILENO,TCSANOW,&orig); }
} rawguard;

static constexpr int KEY_UP = 1001, KEY_DOWN = 1002, KEY_LEFT = 1003, KEY_RIGHT = 1004;
int getKeyNonBlocking() {
    unsigned char buf[8];
    int n = (int)read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) return 0;
    // Arrow keys on most terminals: ESC [ A/B/C/D
    if (buf[0] == 0x1B && n >= 3 && buf[1] == '[') {
        if (buf[2] == 'A') return KEY_UP;
        if (buf[2] == 'B') return KEY_DOWN;
        if (buf[2] == 'C') return KEY_RIGHT;
        if (buf[2] == 'D') return KEY_LEFT;
    }
    return buf[0];
}
#endif

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef _WIN32
    // On Windows console, try to enable ANSI escapes and UTF-8 output
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    T::enableVT();
#endif
    T::clear(); T::hideCursor();
    cout << "Cyber-Neon Snake — Week 1 (input test)\n";
    cout << "Press keys; arrows and WASD are supported. Press Q to quit.\n";
    T::flush();

    while (true) {
        int k = getKeyNonBlocking();
        if (k) {
            if (k == 'q' || k == 'Q') break;
            // Print readable representation for common keys
            if (k == KEY_UP) cout << "[UP]\n";
            else if (k == KEY_DOWN) cout << "[DOWN]\n";
            else if (k == KEY_LEFT) cout << "[LEFT]\n";
            else if (k == KEY_RIGHT) cout << "[RIGHT]\n";
            else cout << "Key: " << k << " ('" << (char)k << "')\n";
            T::flush();
        }
        this_thread::sleep_for(milliseconds(30));
    }

    T::showCursor();
    T::reset();
    cout << "Exiting...\n";
    return 0;
}
