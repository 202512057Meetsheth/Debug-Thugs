//execution commands:
//g++ part4.cpp -o part4
//chcp 65001
//.\part4.exe

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
    if (buf[0] == 0x1B && n >= 3 && buf[1] == '[') {
        if (buf[2] == 'A') return KEY_UP;
        if (buf[2] == 'B') return KEY_DOWN;
        if (buf[2] == 'C') return KEY_RIGHT;
        if (buf[2] == 'D') return KEY_LEFT;
    }
    return buf[0];
}
#endif

//Game structures
enum Dir { NONE=0, UP, DOWN, LEFT, RIGHT };
struct Pt { int r, c; };
struct Fruit { Pt p; int type; };
//CyberSnake
class CyberSnake {
public:
    CyberSnake() : rng((uint64_t)steady_clock::now().time_since_epoch().count()) {
        init();
    }

    void init() {
        rows = 15; cols = 35;
        snake.clear();
        int sr = rows/2, sc = cols/2;
        for (int i=0;i<5;i++) snake.push_back({sr, sc - i});
        dir = RIGHT;
        alive = true;
        score = 0; level = 1; fruitsEaten = 0;
        obstacles.clear();
        baseDelay = 100; curDelay = baseDelay;
        placeFruit();
        T::clear(); T::hideCursor();
        #ifdef _WIN32
        T::enableVT();
        #endif
    }

    void run() {
        auto next = steady_clock::now();
        while (alive) {
            int k = getKeyNonBlocking();
            if (!handleInput(k)) break;
            auto now = steady_clock::now();
            if (now >= next) {
                step();
                draw();
                next = now + milliseconds(curDelay);
            }
            this_thread::sleep_for(milliseconds(5));
        }
        gameOver();
    }

private:
    int rows, cols;
    deque<Pt> snake;
    vector<Pt> obstacles;
    Fruit fruit;
    Dir dir;
    bool alive;
    int score, level, fruitsEaten;
    int baseDelay, curDelay;
    std::mt19937_64 rng;

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

    void placeFruit() {
        vector<Pt> empties;
        vector<vector<char>> used(rows, vector<char>(cols, 0));
        for (auto &s : snake) used[s.r][s.c] = 1;
        for (auto &o : obstacles) used[o.r][o.c] = 1;
        for (int r=0;r<rows;r++)
            for (int c=0;c<cols;c++)
                if (!used[r][c]) empties.push_back({r,c});
        if (empties.empty()) return;
        uniform_int_distribution<int> d(0,(int)empties.size()-1);
        Pt p = empties[d(rng)];
        uniform_int_distribution<int> t(0,99);
        int tt = t(rng);
        int type = (tt < 75 ? 0 : (tt < 90 ? 1 : 2));
        fruit = {p, type};
    }

    void maybeAddObstacle() {
        if (fruitsEaten > 0 && fruitsEaten % 6 == 0) {
            vector<Pt> empties;
            vector<vector<char>> used(rows, vector<char>(cols, 0));
            for (auto &s : snake) used[s.r][s.c] = 1;
            for (auto &o : obstacles) used[o.r][o.c] = 1;
            for (int r=0;r<rows;r++)
                for (int c=0;c<cols;c++)
                    if (!used[r][c]) empties.push_back({r,c});
            if (empties.empty()) return;
            uniform_int_distribution<int> d(0,(int)empties.size()-1);
            obstacles.push_back(empties[d(rng)]);
        }
    }

    void step() {
        Pt head = snake.front();
        Pt nxt = head;
        switch (dir) {
            case UP: nxt.r--; break;
            case DOWN: nxt.r++; break;
            case LEFT: nxt.c--; break;
            case RIGHT: nxt.c++; break;
            default: break;
        }

        if (nxt.r<0 || nxt.c<0 || nxt.r>=rows || nxt.c>=cols) { alive=false; return; }
        for (auto &o:obstacles) if (o.r==nxt.r && o.c==nxt.c) { alive=false; return; }
        for (auto &s:snake) if (s.r==nxt.r && s.c==nxt.c) { alive=false; return; }

        bool grow = (nxt.r==fruit.p.r && nxt.c==fruit.p.c);
        snake.push_front(nxt);
        if (grow) {
            fruitsEaten++;
            if (fruit.type==0) score+=10;
            else if (fruit.type==1) score+=25;
            else { score+=8; curDelay = max(40, curDelay-10); }
            if (fruitsEaten % 4 == 0) { level++; curDelay = max(40, curDelay-5); }
            placeFruit();
            maybeAddObstacle();
        } else snake.pop_back();
    }

    bool handleInput(int k) {
        if (k == 'q' || k == 'Q') return false;
        if (k == KEY_UP && dir != DOWN) dir = UP;
        else if (k == KEY_DOWN && dir != UP) dir = DOWN;
        else if (k == KEY_LEFT && dir != RIGHT) dir = LEFT;
        else if (k == KEY_RIGHT && dir != LEFT) dir = RIGHT;
        return true;
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

        // obstacles
        for (auto &o:obstacles){
            T::moveTo(top+2+o.r, left+2+o.c);
            cout << "\x1b[48;2;50;10;10m" << "\x1b[38;2;200;80;80m" << "#" << "\x1b[0m";
        }

        // fruit
        T::moveTo(top+2+fruit.p.r, left+2+fruit.p.c);
        if (fruit.type==0) cout << "\x1b[38;2;255;120;120m●\x1b[0m";
        else if (fruit.type==1) cout << "\x1b[38;2;255;215;90m★\x1b[0m";
        else cout << "\x1b[38;2;160;240;160m✦\x1b[0m";

        // snake
        for (int i=0;i<snake.size();++i){
            Pt s=snake[i];
            T::moveTo(top+2+s.r, left+2+s.c);
            if (i==0)
                cout << "\x1b[38;2;120;255;230m■\x1b[0m";
            else{
                int t=(int)((double)i/snake.size()*120);
                int r=max(20,30+t),g=max(80,200-t),b=max(30,80+t/2);
                char buf[64];sprintf(buf,"\x1b[38;2;%d;%d;%dm",r,g,b);
                cout<<buf<<"■\x1b[0m";
            }
        }

        T::moveTo(top + frameH + 2, left);
        cout << "\x1b[38;2;200;120;255mScore: " << score
             << "  Level: " << level
             << "  Obstacles: " << obstacles.size() << "\x1b[0m";
        T::flush();
    }

    void gameOver() {
        T::moveTo(1,1);
        cout << "\x1b[1;38;2;255;100;120mGAME OVER\x1b[0m\n";
        T::showCursor();
    }
};

//main
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
