#define TICKS_PER_FRAME (268123480/60)

#include <m3dia.hpp>
#include <iostream>
#include <string>
#include <stdio.h>

//Generate random values for X food position taking bottom screen width
int GenerateRandomFoodXPos(){
    return rand() % 320;   
}

//Generate random values for Y food position taking bottom screen height
int GenerateRandomFoodYPos(){
    return rand() % 240;
}

//Convert char by copying data from std::string to char[]
void convertToCharString(char destination[], std::string orig){
    strcpy(destination, orig.c_str());
}

//Read the file where it's stored the highscore
int readSaveGame(){
    //Where data will be stored
    char buffer[10];
    
    //Open file in read mode
    FILE *saveGameData = fopen("sdmc:/3ds/snake/savegame.dat", "r");
    if(saveGameData != NULL){
        //While not the end of file keep adding to buffer char array
        while(!feof(saveGameData)){
            fread(buffer, sizeof(buffer), 1, saveGameData);
        }
            
        //Save a int the score and close the file
        int s;
        sscanf(buffer, "%d", &s);
        fclose(saveGameData);
        return s;
    }else{
        buffer[10] = '0';
        saveGameData = fopen("sdmc:/3ds/snake/savegame.dat", "w");
        fwrite(buffer, sizeof(buffer), 1, saveGameData);
        fclose(saveGameData);
        return 0;
    }
}

//Write highscore to file
void writeSaveGame(int scoreToSave){
    //Convert from int to std::string
    std::string highScoreConverted = std::to_string(scoreToSave);
    
    //Convert from std::string to char[]
    char charHighScore[highScoreConverted.size() + 1];
    convertToCharString(charHighScore, highScoreConverted.c_str());
    
    //Save and clode the file
    FILE *saveGameData = fopen("sdmc:/3ds/snake/savegame.dat", "w");
    fwrite(charHighScore, sizeof(charHighScore), 1, saveGameData);
    fclose(saveGameData);
}

int main() {
    // Init all stuff
    unsigned int last_time;
    unsigned int now_time;
    unsigned int frame_time;
    bool canGenerateFood = true;
    bool isDied = false;
    bool isInTop = false;
    bool isUp = false;
    bool isLeft = false;
    bool isRight = true;
    bool isDown = false;
    bool newHighScore = false;
    //bool hasChooseMode = false;
    bool gameStarted = false;
    float actualVol = 0.5f;
    int score = 0;
    int snakeSize = 6;
    int speed = 5;
    int posX = speed;
    int posY = 0;
    int highScore;
    //Neccessary instances
    m3d::Applet app;
    m3d::Console consoleTop;
    m3d::Console consoleBottom;
    m3d::Screen screen;

    //The vector composed by rectangles, this is the snake
    std::vector <m3d::Rectangle> snakeParts;
    
    //Food and it position
    int foodPosX = GenerateRandomFoodXPos();
    int foodPosY = GenerateRandomFoodYPos();
    m3d::Rectangle food(foodPosX, foodPosY, snakeSize,snakeSize,m3d::Color(255,0,0));

    

    //Init all music and effects
    m3d::Music bgm("romfs:/bgm.mp3");
    m3d::Music pickFX("romfs:/pickup.mp3");
    m3d::Music dieFX("romfs:/died.mp3");
    
    //We have to declare the first four snake elements
    m3d::Rectangle snake1(10,20, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake2(10,25, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake3(10,30, snakeSize, snakeSize, m3d::Color(255,255,255));
    m3d::Rectangle snake4(10,35, snakeSize, snakeSize, m3d::Color(255,255,255));

    //Music config stuff
    bgm.loop(true);
    bgm.play();
    
    //highScore = checkFolder("sdmc:/3ds/data/snake/");
    consoleTop.useScreen(m3d::RenderContext::ScreenTarget::Top);
    
    // app's main loop
    while(app.isRunning()) { 
        bgm.setVolume(actualVol);
        
        //Exits the homebrew
        if (m3d::Input::buttonPressed(m3d::Input::Button::B)) {
            app.exit();
        }

        //If it is pressed make a boolean true so game can start
        if(m3d::Input::buttonDown(m3d::Input::Button::A)){
            if(!gameStarted){
                if(snakeParts.size() > 1){
                    snakeParts.clear();
                }
                snakeParts.push_back(snake1);
                snakeParts.push_back(snake2);
                snakeParts.push_back(snake3);
                snakeParts.push_back(snake4);
                score = 0;
                newHighScore = false;
                isDied = false;
                gameStarted = true;

                //Get highscore
                highScore = readSaveGame();
                consoleTop.clear();
            }
        }

        //This two if statements are for volum up or volum down the background music
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
            if(gameStarted){
                
                consoleTop.printAt(23, 9, "Hecho por");
                consoleTop.printAt(8, 10, "Jose Luis Fernandez Mateo a.k.a 0c0de");
                consoleTop.printAt(20,17, "Puntuacion: " + std::to_string(score)); 
                
                //Generate food in case of eat the food
                if(canGenerateFood){
                    foodPosX = GenerateRandomFoodXPos();
                    foodPosY = GenerateRandomFoodYPos();
                    food.setPosition(foodPosX, foodPosY);
                    food.setWidth(snakeSize);
                    food.setHeight(snakeSize);
                    food.setColor(m3d::Color(255,0,0));
                    canGenerateFood = false;
                }

                //In this four if statements we control the direction of snake
                if(m3d::Input::buttonPressed(m3d::Input::Button::DPadDown) && !isUp){
                    posY = speed;
                    posX = 0;
                    isUp = false;
                    isDown = true;
                    isLeft = false;
                    isRight = false;
                }

                if(m3d::Input::buttonPressed(m3d::Input::Button::DPadUp) && !isDown){
                    posY = -speed;
                    posX = 0;
                    isUp = true;
                    isDown = false;
                    isLeft = false;
                    isRight = false;
                }

                if(m3d::Input::buttonPressed(m3d::Input::Button::DPadRight) && !isLeft){
                    posX = speed;
                    posY = 0;
                    isUp = false;
                    isDown = false;
                    isLeft = false;
                    isRight = true;
                }

                if(m3d::Input::buttonPressed(m3d::Input::Button::DPadLeft) && !isRight){
                    posY = 0;
                    posX = -speed;
                    isUp = false;
                    isDown = false;
                    isLeft = true;
                    isRight = false;
                }
                
                //Checks if it's out of screen or if it's overlaping if true any of this then the snake will die
                

                //Where all magic happens, make a new X and Y position for the head because what this does it's just
                //generate a new head and remove the tail of snake or pushing to front and poping back
                
                //New position for head composed by the actual position of the head plus the next pos which is driven by
                //the DPad buttons Up,Left,Down,Right
                int headPosX = snakeParts[0].getXPosition() + posX;
                int headPosY = snakeParts[0].getYPosition() + posY;

                //Make a new reactangle using the new position
                m3d::Rectangle newHead(headPosX, headPosY, snakeSize, snakeSize, m3d::Color(255,255,255));

                //As a Javascript developer what I would do will be just unshift but as this is C++ we have vectors
                //and vectors hasn't an unshift method so used this to insert head at the first position of the snake vector
                snakeParts.insert(snakeParts.begin(),newHead);
                
                //Just checking if snake is in the position of the food added some offsets in order to make things a little bit easier
                //If it's in that range then not pops the new rect created so the tail is added
                //And in case not in the position the last element pops out of the vector
                if(snakeParts[0].getXPosition() >= foodPosX - 6 && snakeParts[0].getXPosition() <= foodPosX + 6  && snakeParts[0].getYPosition() >= foodPosY - 6 && snakeParts[0].getYPosition() <= foodPosY + 6){
                    canGenerateFood = true;
                    score++;
                }else {
                    snakeParts.pop_back();
                }

                //Check if some part of the snake it's overlaping
                for(int b = 1; b < snakeParts.size(); b++){
                    if(snakeParts[0].getXPosition() == snakeParts[b].getXPosition() && snakeParts[0].getYPosition() == snakeParts[b].getYPosition())
                        isInTop = true;
                }

                //Drawing the snake
                for(int j = 0; j < snakeParts.size(); j++){
                    if(
                    snakeParts[0].getXPosition() > screen.getScreenWidth(m3d::RenderContext::ScreenTarget::Bottom) ||
                    snakeParts[0].getYPosition() > screen.getScreenHeight() ||
                    snakeParts[0].getYPosition() <= 0 ||
                    snakeParts[0].getXPosition() <= 0 || 
                    isInTop
                    )
                    {
                        consoleTop.clear();
                    
                        if(score > highScore){
                            writeSaveGame(score);
                            pickFX.play();
                            newHighScore = true;
                        }
                        
                        if(!newHighScore){
                            dieFX.play();
                        }
                        isDied = true;
                    
                    }else{
                        screen.drawBottom(snakeParts[j]);
                    }
                }

                //Drawing the food
                screen.drawBottom(food);
            }else{
                //Simple menu to start the game a typical press a to play
                consoleTop.printAt(0,0, "Data from file: " + std::to_string(highScore));
                consoleTop.printAt(23, 9, "Hecho por");
                consoleTop.printAt(8, 10, "Jose Luis Fernandez Mateo a.k.a 0c0de");
                consoleTop.printAt(11,15, "Para iniciar el juego pulsa 'a'");
            }
        }else{
            //Here it is some die logic, another super simple death menu and some sounds playing around
            isDied = true;
            gameStarted = false;
            consoleTop.printAt(13, 10, "Ufff parece que has muerto");
            consoleTop.printAt(13, 11, "Pulsa 'a' para volver a jugar");
            consoleTop.printAt(13, 12, "Pulsa 'b' para salir");
            consoleTop.printAt(20,15, "Puntuacion: " + std::to_string(score));
            
            //Set a new text with a sound in case it's new the highscore
            if(newHighScore){
                consoleTop.printAt(13,17, "Nueva Maxima Puntuacion: " + std::to_string(score) + "!");
            }else{
                consoleTop.printAt(16,17, "Maxima Puntuacion: " + std::to_string(highScore));
            }
            
            //Resetting some vars
            posX = speed;
            posY = 0;
            isInTop = false;
            //dieFX.play();
            bool isUp = false;
            bool isLeft = false;
            bool isRight = true;
            bool isDown = false;
        }
        
        //Render all shapes, text and more...
        screen.render();
        gspWaitForVBlank();
        now_time = svcGetSystemTick();
        frame_time = now_time - last_time;
        last_time = now_time;
        if (frame_time < TICKS_PER_FRAME) svcSleepThread(TICKS_PER_FRAME - frame_time);
    }

    return(0);
}
