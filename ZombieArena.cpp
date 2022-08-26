// ZombieArena.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"

using namespace sf;

int main()
{
    //Here is the instance of TextureHolder
    TextureHolder holder;

    //The game wiil always be in one of four states
    enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

    //Start with the GAME_OVER state
    State state = State::GAME_OVER;

    //Get the screen resolution and create an SFML window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(
        VideoMode(resolution.x, resolution.y),
        "Zombie Arena", Style::Fullscreen);

    //Create an SFML View for maun action
    View mainView(sf::FloatRect(0, 0,
        resolution.x, resolution.y));

    //Here is our clock for timing everything
    Clock clock;

    //How long has the PLAYING state been active
    Time gameTimeTotal;

    //Where is the mouse in relation to world coordinates
    Vector2f mouseWorldPosition;

    //Where is the mouse in relation to screen coordinates
    Vector2i mouseScreenPosition;

    //Create an instance of the Plauer class
    Player player;

    //The boundaries of the arena
    IntRect arena;

    //Create the background
    VertexArray background;
    //Load the texture for our background vertex array
    //Texture textureBackground;
    //textureBackground.loadFromFile("graphics/background_sheet.png");
    Texture textureBackground = TextureHolder::GetTexture(
        "graphics/background_sheet.png"
    );


    //Prepare for a horde of zombie
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    //The main game loop
    while (window.isOpen())
    {

        /*
        * ******
        * Handle input
        * ******
        */

        //Handle events by polling
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                //Pause a game while playing
                if (event.key.code == Keyboard::Return &&
                    state == State::PLAYING)
                {
                    state = State::PAUSED;
                }

                //Restart while paused 
                else if (event.key.code == Keyboard::Return &&
                    state == State::PAUSED)
                {
                    state = State::PLAYING;
                    //Restart the clock so there isn't a frame jump
                    clock.restart();
                }

                //Start new game while in GAME_OVER state
                else if (event.key.code == Keyboard::Return &&
                    state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }

                if (state == State::PLAYING)
                {

                }


            }
        }//End event polling

        //Handle the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //Handle WASD while playing
        if (state == State::PLAYING)
        {
            //Handle the pressing and releasing of the WASD keys
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.StopUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.StopDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

        }//End WASD while playing

        //Handle the LEVELING_UP state
        if (state == State::LEVELING_UP)
        {
            //Handle the player LEVELING_UP
            if (event.key.code == Keyboard::Num1)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num2)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num3)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                //Prepare the level
                //We will modify the next 2 line later
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                //Pass the vertex array by reference
                //to the createBackground function
                int tileSize = createBackground(background, arena);

                //We will modify this line of code later
                //int tileSize = 50;

                //Spawn the player in the middle of the arena
                player.spawn(arena, resolution, tileSize);

                //Create a horde of zombie
                numZombies = 10;

                //Delete the previosly allocated memory (if it exists)
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                
                //Reset the clock so there isn't a frame jump
                clock.restart();
            }
        }//End LEVELING_UP

        /*
        * *************
        *UPDATE THE FRAME
        * *************
        */

        if (state == State::PLAYING)
        {
            //Update the delta time
            Time dt = clock.restart();

            //Update the total game time
            gameTimeTotal += dt;

            //Make a decimal fraction of 1 from the delta time
            float dtAsSeconds = dt.asSeconds();

            //Where is the mouse pointer
            mouseScreenPosition = Mouse::getPosition();

            //Convert mouse position to world coordinate of mainView
            mouseWorldPosition = window.mapPixelToCoords(
                Mouse::getPosition(), mainView);

            //Update the player
            player.update(dtAsSeconds, Mouse::getPosition());

            //Make a note of the player position
            Vector2f playerPosition(player.getCenter());

            //Make the view centre around the player
            mainView.setCenter(player.getCenter());

            //Loop through each Zombie and update them
            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
            
        }//End updating the scene


        /*
        * **********
        * Draw the scene
        * **********
        */

        if (state == State::PLAYING)
        {
            window.clear();

            //set the mainView to be displayed in the window
            //and draw everything related it
            window.setView(mainView);

            //Draw background
            window.draw(background, &textureBackground);

            //Draw the zombies
            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }

            //Draw the player
            window.draw(player.getSprite());
        }

        if (state == State::LEVELING_UP)
        {

        }

        if (state == State::PAUSED)
        {

        }

        if (state == State::GAME_OVER)
        {

        }

        window.display();

    }//End game loop

    //Delete the previosly allocated memory (if it exists)
    delete[] zombies;

    return 0;
}


