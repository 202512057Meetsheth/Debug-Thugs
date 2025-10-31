//execution commands
//g++ part2.cpp -o part2
//chcp 65001
//.\part2.exe

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

namespace T {
#ifdef _WIN32
bool enableVT() {
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

struct TermSize { int rows, cols; };
#ifdef _WIN32
TermSize size() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    int rows = info.srWindow.Bottom - info.srWindow.Top + 1;
    int cols = info.srWindow.Right - info.srWindow.Left + 1;
    return { rows, cols };
}
#else
TermSize size() {
    winsize ws{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return { (int)ws.ws_row, (int)ws.ws_col };
}
#endif
}

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
    if (buf[0] == 0x1B && n >= 3 && buf[1] == '[') {
        if (buf[2] == 'A') return KEY_UP;
        if (buf[2] == 'B') return KEY_DOWN;
        if (buf[2] == 'C') return KEY_RIGHT;
        if (buf[2] == 'D') return KEY_LEFT;
    }
    return buf[0];
}
#endif

enum Dir { NONE=0, UP, DOWN, LEFT, RIGHT };
struct Pt { int r, c; };

class CyberSnake {
public:
    CyberSnake() { init(); }

    void init() {
        auto ts = T::size();
        rows = 15;
        cols = 35;
        if (rows > ts.rows - 4) rows = ts.rows - 6;
        if (cols > ts.cols - 8) cols = ts.cols - 10;

        snake.clear();
        int sr = rows/2, sc = cols/2;
        for (int i=0; i<5; ++i) snake.push_back({sr, sc - i});

        T::clear();
        T::hideCursor();
        #ifdef _WIN32
        T::enableVT();
        #endif
    }

    void run() {
        draw();
        T::flush();
        cout << "\nPress Q to quit.\n";

        while (true) {
            int k = getKeyNonBlocking();
            if (k == 'q' || k == 'Q') break;
            this_thread::sleep_for(milliseconds(50));
        }

        cleanup();
    }

private:
    int rows, cols;
    deque<Pt> snake;

    string neon(int i) {
        int r = (int)(80 + 120.0 * (sin(i*0.5) * 0.5 + 0.5));
        int g = (int)(160 + 80.0 * (cos(i*0.3) * 0.5 + 0.5));
        int b = (int)(200 + 40.0 * (sin(i*0.7) * 0.5 + 0.5));
        char buf[64];
        sprintf(buf, "\x1b[38;2;%d;%d;%dm", r,g,b);
        return string(buf);
    }

    void drawFrame(int top, int left, int h, int w) {
        for (int c = 0; c < w; ++c) {
            string col = neon(c);
            T::moveTo(top, left + c); cout << col << "═";
            T::moveTo(top + h - 1, left + c); cout << col << "═";
        }
        for (int r = 1; r < h-1; ++r) {
            string col = neon(r);
            T::moveTo(top + r, left); cout << col << "║";
            T::moveTo(top + r, left + w - 1); cout << col << "║";
        }
        T::reset();
    }

    void draw() {
        T::clear();
        auto ts = T::size();
        int top = max(1, (ts.rows - (rows + 6)) / 2);
        int left = max(2, (ts.cols - (cols + 36)) / 2);
        int frameW = cols + 4;
        int frameH = rows + 4;

        drawFrame(top, left, frameH, frameW);

        for (int r=0;r<rows;r++){
            for (int c=0;c<cols;c++){
                T::moveTo(top + 2 + r, left + 2 + c);
                if (((r+c)&1)==0) cout << "\x1b[48;2;10;12;16m \x1b[0m";
                else cout << "\x1b[48;2;8;10;14m \x1b[0m";
            }
        }

        for (int i=0;i<snake.size();++i){
            Pt s = snake[i];
            T::moveTo(top+2+s.r, left+2+s.c);
            if (i==0)
                cout << "\x1b[38;2;120;255;230m" << "■" << "\x1b[0m";
            else {
                int t = (int)((double)i / max(1,(int)snake.size()) * 120);
                int r = max(20, 30 + t), g = max(80, 200 - t), b = max(30, 80 + t/2);
                char buf[64]; sprintf(buf, "\x1b[38;2;%d;%d;%dm", r,g,b);
                cout << buf << "■" << "\x1b[0m";
            }
        }

        T::moveTo(top + frameH + 2, left);
        cout << "\x1b[38;2;200;120;255mCyber-Neon Snake — Part 2\x1b[0m";
    }

    void cleanup() {
        T::reset();
        T::showCursor();
        T::moveTo(999,1);
        cout << "\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    T::enableVT();
#endif
    CyberSnake game;
    game.run();
    return 0;
}
