//#define TICKS_PER_FRAME (268123480/60)

#include <m3dia.hpp>
#include <iostream>
#include <string>

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
    bool canGenerateFood = true;
    bool isDied = false;
    bool isInTop = false;
    int score = 0;
    int snakeSize = 5;
    int posX = 5;
    int posY = 0;
    int foodPosX = GenerateRandomFoodXPos();
    int foodPosY = GenerateRandomFoodYPos();
    std::vector <m3d::Rectangle> snakeParts;
    m3d::Rectangle food(foodPosX, foodPosY, 5,5,m3d::Color(255,0,0));
    m3d::Applet app;
    m3d::Console console;
    m3d::Screen screen;
    m3d::Music bgm("romfs:/bgm.mp3");
    //For select the topScreen
    PrintConsole topScr;
    m3d::Rectangle snake1(10,20, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake2(10,25, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake3(10,30, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake4(10,35, snakeSize, snakeSize, m3d::Color(255,255,255));
    snakeParts.push_back(snake1);
    snakeParts.push_back(snake2);
    snakeParts.push_back(snake3);
    snakeParts.push_back(snake1);
    float actualVol = 0.5f;
    bgm.loop(true);
    bgm.play();
    console.useScreen(m3d::RenderContext::ScreenTarget::Top);
            
    // app's main loop
    while(app.isRunning()) {
        console.printAt(20,15, "Puntuacion: " + std::to_string(score));    
        bgm.setVolume(actualVol);
        //printf("\x1b[16;10HScore: %d", score);
        //screen.clear();   
        if (m3d::Input::buttonPressed(m3d::Input::Button::B)) {
            app.exit();
        }

        if (m3d::Input::buttonPressed(m3d::Input::Button::L)) {
            if(actualVol > 0.0f && actualVol < 1.0f){
                actualVol -= 0.1f;
            }
        }

        if (m3d::Input::buttonPressed(m3d::Input::Button::R)) {
            if(actualVol > 0.0f && actualVol < 1.0f){
                actualVol += 0.1f;
            }
        }

        if(!isDied){
            if(canGenerateFood){
                foodPosX = GenerateRandomFoodXPos();
                foodPosY = GenerateRandomFoodYPos();
                food.setPosition(foodPosX, foodPosY);
                food.setWidth(5);
                food.setHeight(5);
                food.setColor(m3d::Color(255,0,0));
                canGenerateFood = false;
            }

            if(m3d::Input::buttonPressed(m3d::Input::Button::DPadDown)){
                posY = 5;
                posX = 0;
            }

            if(m3d::Input::buttonPressed(m3d::Input::Button::DPadUp)){
                posY = -5;
                posX = 0;
            }

            if(m3d::Input::buttonPressed(m3d::Input::Button::DPadRight)){
                posX = 5;
                posY = 0;
            }

            if(m3d::Input::buttonPressed(m3d::Input::Button::DPadLeft)){
                posY = 0;
                posX = -5;
            }
            
            if(
                snakeParts[0].getXPosition() > screen.getScreenWidth(m3d::RenderContext::ScreenTarget::Bottom) ||
                snakeParts[0].getYPosition() > screen.getScreenHeight() ||
                snakeParts[0].getYPosition() <= 0 ||
                snakeParts[0].getXPosition() <= 0 || 
                isInTop
            )
            {
                isDied = true;
            }
            int headPosX = snakeParts[0].getXPosition() + posX;
            int headPosY = snakeParts[0].getYPosition() + posY;
            m3d::Rectangle newHead(headPosX, headPosY, snakeSize, snakeSize, m3d::Color(255,255,255));
            snakeParts.insert(snakeParts.begin(),newHead);
            
            if(snakeParts[0].getXPosition() >= foodPosX - 5 && snakeParts[0].getXPosition() <= foodPosX + 5  && snakeParts[0].getYPosition() >= foodPosY - 5 && snakeParts[0].getYPosition() <= foodPosY + 5){
                canGenerateFood = true;
                score++;
            }else {
                snakeParts.pop_back();
            }

            //Check if it is in same pos of tail
            for(int b = 1; b < snakeParts.size(); b++){
                if(snakeParts[0].getXPosition() == snakeParts[b].getXPosition() && snakeParts[0].getYPosition() == snakeParts[b].getYPosition())
                    isInTop = true;
            }

            //Drawing the snake
            for(int j = 0; j < snakeParts.size(); j++){
                screen.drawBottom(snakeParts[j]);
            }

            screen.drawBottom(food);
        }
        

        screen.render();
        gspWaitForVBlank();
        /*now_time = svcGetSystemTick();
        frame_time = now_time - last_time;
        last_time = now_time;
        if (frame_time < TICKS_PER_FRAME) svcSleepThread(TICKS_PER_FRAME - frame_time);   */
    }

    return(0);
}