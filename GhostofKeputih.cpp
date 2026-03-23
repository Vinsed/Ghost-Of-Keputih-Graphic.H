#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

const int WIDTH = 800, HEIGHT = 600;
const int MOVE_SPEED = 7, PUNCH_TIME = 5, KNOCKBACK = 20, ATTACK_DIST = 80;

int playerScore = 0;

void* loadimage(const char* filename, int width, int height) {
    void* buffer = new char[imagesize(0,0,width, height)];
    readimagefile(filename,0,0,width,height);
    getimage(0,0,width,height, buffer);
    cleardevice();
    return buffer;
};

void* fighterImg = nullptr;
void* enemyImg = nullptr;
void* punchImg = nullptr;
void* enemyPunchImg = nullptr;
void* playerhitImg = nullptr;
void* enemyhitImg = nullptr;
void* background = nullptr;
void* jumpImg = nullptr;
void* enemyJumpImg = nullptr;
void* specialPunchImg = nullptr;
void* enemymaskingImg = nullptr;
void* fightermaskingImg = nullptr;
void* enemyhitmaskingImg = nullptr;
void* playerhitmaskingImg = nullptr;
void* jumpmaskingImg = nullptr;
void* specialPunchmaskingImg = nullptr;
void* punchmaskingImg = nullptr;
void* enemypunchmaskingImg = nullptr;

void initImages() {
    fighterImg = loadimage("C:/Project Prolan/fighter.gif", 150, 200);
    fightermaskingImg = loadimage("C:/Project Prolan/fightermasking.gif", 150, 200);
    enemyImg = loadimage("C:/Project Prolan/enemy.gif", 150, 200);
    enemymaskingImg = loadimage("C:/Project Prolan/enemymasking.gif", 150, 200);
    punchImg = loadimage("C:/Project Prolan/punch.gif", 150, 200);
    punchmaskingImg = loadimage("C:/Project Prolan/punchmasking.gif", 150, 200);
    enemyPunchImg = loadimage("C:/Project Prolan/enemy_punch.gif", 150, 200);
    enemypunchmaskingImg = loadimage("C:/Project Prolan/enemy_punchmasking.gif", 150, 200);
    playerhitImg = loadimage("C:/Project Prolan/player_hit.gif", 150, 200);
    playerhitmaskingImg = loadimage("C:/Project Prolan/player_hitmasking.gif", 150, 200);
    enemyhitmaskingImg = loadimage("C:/Project Prolan/enemy_hitmasking.gif", 150, 200);
    enemyhitImg = loadimage("C:/Project Prolan/enemy_hit.gif", 150, 200);
    background = loadimage("C:/Project Prolan/background.gif", WIDTH, HEIGHT);
    jumpImg = loadimage("C:/Project Prolan/jump.gif", 150, 200);
    jumpmaskingImg = loadimage("C:/Project Prolan/jumpmasking.gif", 150, 200);
    enemyJumpImg = loadimage("C:/Project Prolan/jump.gif", 150, 200);   
    specialPunchImg = loadimage("C:/Project Prolan/special_punch.gif", 160, 200);
    specialPunchmaskingImg = loadimage("C:/Project Prolan/special_punchmasking.gif", 160, 200);
};


class Background {
public:
    void draw(void* background) {
        if (background == nullptr) {
            return;
        }
        putimage(0, 0, background, COPY_PUT);
    }
};

class Fighter {
public:
    int x, y;            
    int w, h;             // Width and height
    int hp;               // Health points
    int maxHp;            // Maximum health points
    int lastHp;           // Previous health points
    bool punching;        
    bool isEnemy;        
    int punchTimer;       
    int punchCooldown;    // Cooldown between punches
    const char* img;      
    bool Jumping;       
    int jumpVelocity;     
    bool isOnGround;      
    bool isSpecialPunch;  

    static const int PUNCH_COOLDOWN_TIME = 15;
    static const int JUMP_VELOCITY = -15;  
    static const int GRAVITY = 1;           
    static const int GROUND_Y = 340;       
    
    Fighter(int x, int y, int w, int h, bool isEnemy, const char* img = nullptr)
        : x(x), y(y), w(w), h(h), hp(100), maxHp(100), lastHp(100), punching(false), isEnemy(isEnemy),
          punchTimer(0), punchCooldown(0), img(img), Jumping(false), jumpVelocity(0), 
          isOnGround(true), isSpecialPunch(false) {}

    void draw() {
        if (hp <= 0) return;
        if (img) {
            if (punching && Jumping && isSpecialPunch) {
                putimage(x, y + 10, specialPunchmaskingImg, AND_PUT);
                putimage(x, y + 10, specialPunchImg, OR_PUT);
            }
            else if (punching) {
                if (isEnemy) {
                    putimage(x, y, enemypunchmaskingImg, AND_PUT);
                    putimage(x, y, enemyPunchImg, OR_PUT);
                } else {
                    putimage(x, y, punchmaskingImg, AND_PUT);
                    putimage(x, y, punchImg, OR_PUT);
                }
            } else if (Jumping) {
                if (isEnemy) {
                    putimage(x, y, jumpmaskingImg, AND_PUT);
                    putimage(x, y, jumpImg, OR_PUT);
                } else {
                    putimage(x, y, jumpmaskingImg, AND_PUT);
                    putimage(x, y, jumpImg, OR_PUT);
                }
            } else if (hp < lastHp) { 
                lastHp = hp;
                if (isEnemy) {
                    putimage(x, y, enemyhitmaskingImg, AND_PUT);
                    putimage(x, y, enemyhitImg, OR_PUT);
                    delay(100); 
                } else {
                    putimage(x, y, playerhitmaskingImg, AND_PUT);
                    putimage(x, y, playerhitImg, OR_PUT);
                    delay(100); 
                }
            } else {
                if (isEnemy) {
                    putimage(x, y, enemymaskingImg, AND_PUT);
                    putimage(x, y, enemyImg, OR_PUT);
                } else {
                    putimage(x, y, fightermaskingImg, AND_PUT);
                    putimage(x, y, fighterImg, OR_PUT);
                }
            }
        }
    }

    void update(bool moveLeft, bool moveRight, bool punch) {
        if (hp <= 0) return;

        if (moveLeft)  x -= MOVE_SPEED;
        if (moveRight) x += MOVE_SPEED;

        if (!isEnemy && !Jumping && (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
            Jumping = true;
            jumpVelocity = JUMP_VELOCITY;
            isOnGround = false;
        }

        // mekanisme lompat
        if (Jumping) {
            y += jumpVelocity;
            jumpVelocity += GRAVITY;

            if (y >= GROUND_Y) {
                y = GROUND_Y;
                Jumping = false;
                isOnGround = true;
            }
        }
        
        if (punchCooldown > 0)
            punchCooldown--;

        if (punch && !punching && punchCooldown == 0 && !isSpecialPunch) {
            punching = true;
            punchTimer = PUNCH_TIME;
            punchCooldown = PUNCH_COOLDOWN_TIME;
        }

        // special punch cuma bisa dipakai saat di udara
        if (!isEnemy && (GetAsyncKeyState('S') & 0x8000) && !punching && punchCooldown == 0 && Jumping) {
            punching = true;
            isSpecialPunch = true;
            punchTimer = PUNCH_TIME * 2;
            punchCooldown = PUNCH_COOLDOWN_TIME * 2;
        }

        if (punching) {
            punchTimer--;
            if (punchTimer <= 0) {
                punching = false;
                isSpecialPunch = false;
            }
        }
        x = std::max(0, std::min(x, WIDTH - w));
    }

    RECT hitbox() const {
        return { x, y, x + 100, y + h };
    }

    void knockback(int d) {
        if (hp <= 0) return;
        if (isEnemy) {
            x += d;
        }
        else {
            x -= d;
        }
    }

    void reset(int startX, int startY) {
        x = startX;
        y = startY;
        hp = maxHp;
        lastHp = maxHp;
        punching = false;
        punchTimer = 0;
        punchCooldown = 0;
        Jumping = false;
        jumpVelocity = 0;
        isOnGround = true;
        isSpecialPunch = false;
    }
};

void enemyAI(Fighter& enemy, const Fighter& player) {
    if (enemy.hp <= 0) return;

    bool moveLeft  = enemy.x > player.x + ATTACK_DIST;
    bool moveRight = enemy.x < player.x - ATTACK_DIST;
    bool punch = abs(enemy.x - player.x) <= ATTACK_DIST && (rand() % 100 < 25);

    enemy.update(moveLeft, moveRight, punch);
}

// cek hit
bool checkHit(const Fighter& attacker, const Fighter& defender) {
    if (!attacker.punching || attacker.hp <= 0 || defender.hp <= 0) return false;
    RECT aBox = attacker.hitbox();
    RECT dBox = defender.hitbox();
    bool overlap = (aBox.right > dBox.left && aBox.left < dBox.right &&
                    aBox.bottom > dBox.top && aBox.top < dBox.bottom);

    if (attacker.isEnemy && attacker.x > defender.x) return overlap;
    if (!attacker.isEnemy && attacker.x < defender.x) return overlap;
    return false;
}

// Draw HP bar
void drawHP(int x, int y, int w, int hp) {
    int barWidth = w * hp / 100;
    setfillstyle(SOLID_FILL, GREEN); 
    bar(x, y, x + barWidth, y + 20);
    setcolor(WHITE);
    rectangle(x, y, x + w, y + 20);
}

void drawScore() {
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char scoreText[50];
    sprintf(scoreText, "SCORE: %d", playerScore);
    outtextxy(WIDTH / 2 - textwidth(scoreText) / 2, 10, scoreText);
}

bool showMenu() {
    setbkcolor(BLACK);
    cleardevice();

    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(WIDTH / 2 - textwidth((char*)"GHOST OF KEPUTIH") / 2, 150, (char*)"GHOST OF KEPUTIH");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    rectangle(WIDTH / 2 - 100, 300, WIDTH / 2 + 100, 350);
    outtextxy(WIDTH / 2 - 50, 310, (char*)"PLAY");

    rectangle(WIDTH / 2 - 100, 380, WIDTH / 2 + 100, 430);
    outtextxy(WIDTH / 2 - 50, 390, (char*)"EXIT");

    while (true) {
        if (GetAsyncKeyState(VK_LBUTTON)) {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(GetForegroundWindow(), &cursor);
            int mx = cursor.x, my = cursor.y;

            if (mx >= WIDTH / 2 - 100 && mx <= WIDTH / 2 + 100) {
                if (my >= 300 && my <= 350) return true;
                if (my >= 380 && my <= 430) return false;
            }
        }
        delay(100);
    }
}

bool showGameOverScreen() {
    PlaySound(NULL, NULL, 0);
    
    while (GetAsyncKeyState(VK_LBUTTON)) delay(10);
    delay(1000); 
    
    while (true) {
        setbkcolor(BLACK);

        int boxWidth = 400, boxHeight = 350;
        int boxX = WIDTH / 2 - boxWidth / 2;
        int boxY = 120;
        setfillstyle(SOLID_FILL, BLACK);
        bar(boxX, boxY, boxX + boxWidth, boxY + boxHeight);
        setcolor(WHITE);
        rectangle(boxX, boxY, boxX + boxWidth, boxY + boxHeight);
        
        settextstyle(BOLD_FONT, HORIZ_DIR, 4);
        setcolor(RED);
        outtextxy(WIDTH / 2 - textwidth((char*)"GAME OVER") / 2, 150, (char*)"GAME OVER");

        // display score
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        char scoreText[50];
        sprintf(scoreText, "Kill Count: %d", playerScore);
        outtextxy(WIDTH / 2 - textwidth(scoreText) / 2, 220, scoreText);

        //button Retry 
        setcolor(WHITE);
        rectangle(WIDTH / 2 - 100, 300, WIDTH / 2 + 100, 350);
        outtextxy(WIDTH / 2 - textwidth((char*)"RETRY") / 2, 310, (char*)"RETRY");

        // button Exit 
        rectangle(WIDTH / 2 - 100, 380, WIDTH / 2 + 100, 430);
        outtextxy(WIDTH / 2 - textwidth((char*)"EXIT") / 2, 390, (char*)"EXIT");

        if (GetAsyncKeyState(VK_LBUTTON)) {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(GetForegroundWindow(), &cursor);
            int mx = cursor.x, my = cursor.y;

            if (mx >= WIDTH / 2 - 100 && mx <= WIDTH / 2 + 100) {
                if (my >= 300 && my <= 350) {
                    while (GetAsyncKeyState(VK_LBUTTON)) delay(10);
                    delay(200);
                    return true;// Retry
                }
                if (my >= 380 && my <= 430) {
                    while (GetAsyncKeyState(VK_LBUTTON)) delay(10);
                    delay(200);
                    return false;// Exit 
                }
            }
        }
        delay(50);
    }
}

int main() {
    initwindow(WIDTH, HEIGHT, "Ghost of Keputih");
    initImages();  
    
    while (true) {
        if (!showMenu()) {
            // freeImages();
             closegraph();
            return 0;
        }

        playerScore = 0;

        Fighter player(260, 340, 150, 200, false, "C:/Project Prolan/fighter.gif");
        Fighter enemy(550, 340, 150, 200, true, "C:/Project Prolan/enemy2.gif");
        Background bg;

        PlaySound(TEXT("C:/Project Prolan/backsound1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

        int activePage = 0, visualPage = 1;
        setactivepage(activePage);
        setvisualpage(visualPage);

        bool gameRunning = true;
        
        while (gameRunning) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                gameRunning = false;
                break;
            }
            
            setactivepage(activePage);
            
            bg.draw(background);

            bool left = GetAsyncKeyState('A') & 0x8000;
            bool right = GetAsyncKeyState('D') & 0x8000;
            bool punch = GetAsyncKeyState('W') & 0x8000;

            player.update(left, right, punch);
            enemyAI(enemy, player);

            if (checkHit(player, enemy)) { 
                enemy.hp = std::max(0, enemy.hp - 5); 
                enemy.knockback(KNOCKBACK); 
            }
            if (checkHit(enemy, player)) { 
                player.hp = std::max(0, player.hp - 5); 
                player.knockback(KNOCKBACK); 
            }

         if (enemy.hp <= 0) {
            playerScore++;
            enemy.reset(550, 340);
            }
            
            player.draw();
            enemy.draw();
            drawHP(50, 30, 300, player.hp);
            drawHP(WIDTH - 350, 30, 300, enemy.hp);
            drawScore();

            setvisualpage(activePage);
            activePage = 1 - activePage;
            

            if (player.hp <= 0) {
                delay(1000); 
                if (!showGameOverScreen()) {
                    gameRunning = false; 
                    break;
                } else {
                    player.reset(260, 340);
                    enemy.reset(550, 340);
                    playerScore = 0;
                    PlaySound(TEXT("C:/Project Prolan/backsound1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
            }
            
            delay(25);
        }
        PlaySound(NULL, NULL, 0);
    }
    closegraph();
    return 0;
}