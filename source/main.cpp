#define TICKS_PER_FRAME (268123480/60)

#include <m3dia.hpp>
#include <iostream>

void printAt(int t_x, int t_y, const std::string& t_text) {
    printf("\x1b[%d;%dH%s\n", t_y, t_x, t_text.c_str());
}

int GenerateRandomFoodXPos(){
    return rand() % 320;   
}

int GenerateRandomFoodYPos(){
    return rand() % 240;
}



int main() {
    // Init all stuff
    unsigned int last_time;
    unsigned int now_time;
    unsigned int frame_time;
    int score = 0;
    int snakeLength = 4;
    int snakeSize = 5;
    bool isUp = false;
    bool isDown = true;
    bool isLeft = false;
    bool isRight = false;
    bool canGenerateFood = true;
    bool q = true;
    int posX = 10;
    int posY = 20;
    int foodPosX = GenerateRandomFoodXPos();
    int foodPosY = GenerateRandomFoodYPos();
    std::vector <m3d::Rectangle> snakeParts;
    m3d::Rectangle food(foodPosX, foodPosY, 5,5,m3d::Color(255,0,0));
    m3d::Applet app;
    m3d::Screen screen;
    //For select the topScreen
    PrintConsole topScr;
    
    consoleInit(GFX_TOP, &topScr);
    
    // app's main loop
    while(app.isRunning()) {
        consoleSelect(&topScr);
        printf("\x1b[16;10HScore: %d", score);

        //Drawing the snake
        if(q){
            
            for(int x = 0; x < snakeLength; x++){
                    if(x != 0){
                        m3d::Rectangle snake_one(snakeParts[0].getXPosition(),snakeParts[0].getYPosition() + (x * snakeSize), snakeSize,snakeSize, m3d::Color(255,255,255));
                        snakeParts.push_back(snake_one);
                    }else{
                        m3d::Rectangle snake_one(posX, posY, snakeSize,snakeSize, m3d::Color(255,255,255));
                        snakeParts.push_back(snake_one);
                    }
                
                
            }
        }
        //q = false;
        printf("\x1b[0;0HSize of snake vector is: %d and variable is: %d", snakeParts.size(), snakeLength);

        if(canGenerateFood){
            foodPosX = GenerateRandomFoodXPos();
            foodPosY = GenerateRandomFoodYPos();
            food.setPosition(foodPosX, foodPosY);
            food.setWidth(5);
            food.setHeight(5);
            food.setColor(m3d::Color(255,0,0));
            canGenerateFood = false;
        }

        if (m3d::Input::buttonPressed(m3d::Input::Button::B)) {
            app.exit();
        }

        if(m3d::Input::buttonPressed(m3d::Input::Button::DPadDown)){
            isDown = true;
            isUp = false;
            isLeft = false;
            isRight = false;
        }

        if(m3d::Input::buttonPressed(m3d::Input::Button::DPadUp)){
            isDown = false;
            isUp = true;
            isLeft = false;
            isRight = false;
        }

        if(m3d::Input::buttonPressed(m3d::Input::Button::DPadRight)){
            isDown = false;
            isUp = false;
            isLeft = false;
            isRight = true;
        }

        if(m3d::Input::buttonPressed(m3d::Input::Button::DPadLeft)){
            isDown = false;
            isUp = false;
            isLeft = true;
            isRight = false;
        }

        if(isUp){
            posY = 5;
            int x = snakeParts[0].getXPosition();
            int y = snakeParts[0].getYPosition() + 5;
            snakeParts[0].setPosition(x,y);
        }

        if(isDown){
            posY = -5;
            int x = snakeParts[0].getXPosition();
            int y = snakeParts[0].getYPosition() - 5;
            snakeParts[0].setPosition(x,y);
        }
        
        if(isLeft){
            posX = -5;
            int x = snakeParts[0].getXPosition() - 5;
            int y = snakeParts[0].getYPosition();
            snakeParts[0].setPosition(x,y);
        }
        
        if(isRight){
            posX = 5;
            int x = snakeParts[0].getXPosition() + 5;
            int y = snakeParts[0].getYPosition();
            snakeParts[0].setPosition(x,y);
        }

        if(snakeParts[0].getXPosition() >= foodPosX - 5 && snakeParts[0].getXPosition() <= foodPosX + 5  && snakeParts[0].getYPosition() >= foodPosY - 5 && snakeParts[0].getYPosition() <= foodPosY + 5){
            canGenerateFood = true;
            score++;
            snakeLength++;
            q = true;
        }

        
        for(int a = 0; a < snakeParts.size(); a++){
            if(a == snakeParts.size() - 1){
                snakeParts[0] = snakeParts[a];
                snakeParts.pop_back();
            }
            
           
            screen.drawBottom(snakeParts[a]);    
        }

        screen.drawBottom(food);
        snakeParts.pop_back();
        screen.render();
        gspWaitForVBlank();
        now_time = svcGetSystemTick();
        frame_time = now_time - last_time;
        last_time = now_time;
        if (frame_time < TICKS_PER_FRAME) svcSleepThread(TICKS_PER_FRAME - frame_time);   
    }

    return(0);
}