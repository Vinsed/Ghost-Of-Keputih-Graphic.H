bool showGameOverScreen() {
    PlaySound(NULL, NULL, 0);
    
    while (GetAsyncKeyState(VK_LBUTTON)) delay(10);
    delay(1000); 
    
    while (true) {
        setbkcolor(BLACK);

        // Draw black box in the middle
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