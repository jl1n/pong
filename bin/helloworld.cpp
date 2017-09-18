#include <SFML/Graphics.hpp>
//#include <iostream>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace sf;
using namespace std;

 // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    sf::Vector2f paddleSize(25, 100);
    float ballRadius = 8.f;
    enum Mode { start, playing, finish };
    Mode mode = start;
    sf::Font font;

int main()
{

    std::srand(std::time(0));

   

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Pong",
        sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);


    // Load the sounds used in the game
    /*sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);*/

    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize);
    leftPaddle.setFillColor(sf::Color(100, 100, 200));
    leftPaddle.setOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize);
    rightPaddle.setFillColor(sf::Color(200, 100, 100));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    if (!font.loadFromFile("../res/ChocolateCoveredRaindrops.ttf"))
        return 0;

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(60);
    score.setColor(sf::Color::White);
    score.setString("0 - 0");
    score.setPosition(gameWidth/2.f, 0);
    score.setOrigin(score.getLocalBounds().width/2.0f,score.getLocalBounds().height/2.0f);

    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(40);
    startText.setColor(sf::Color::White);
    startText.setString("Pong! ENTER to start.");
    startText.setPosition(gameWidth/2.f, gameHeight/2.f - 20);
    startText.setOrigin(startText.getLocalBounds().width/2.0f,startText.getLocalBounds().height/2.0f);

    sf::Text finishText;
    finishText.setFont(font);
    finishText.setCharacterSize(40);
    finishText.setColor(sf::Color::White);
    finishText.setString("Finished! ENTER to play again.");
    finishText.setPosition(gameWidth/2.f, gameHeight/2.f - 20);
    finishText.setOrigin(finishText.getLocalBounds().width/2.0f,finishText.getLocalBounds().height/2.0f);

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.4f);
    const float paddleSpeed = 1.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 1.f;
    float ballAngle         = 0.f; // to be changed later

    sf::Clock clock;
    //bool isPlaying = false;
    mode = start;
    int leftScore = 0;
    int rightScore = 0;
    bool Right = false;
    bool Left = false;


    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {

            if(event.type == sf::Event::KeyPressed) {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
             ((event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            else if ((event.key.code == sf::Keyboard::Return))
            {

                if(mode == start || mode == finish)
                {
                    // (re)start the game
                    //isPlaying = true;
                    mode = playing;
                    clock.restart();

                    leftScore = 0;
                    rightScore = 0;
                    score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));

                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition(paddleSize.x, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - paddleSize.x, gameHeight / 2);
                    ball.setPosition(gameWidth / 2, gameHeight / 2);

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }

               else if (event.key.code == Keyboard::D) {
                    Right = true;
                    break;
                }

                  else if (event.key.code == Keyboard::A) {
                    Left = true;
                    break;
                }

            }
            else if(event.type == sf::Event::KeyReleased) {
                if (event.key.code == Keyboard::A)
                    Left = false;
 
                else if (event.key.code == Keyboard::D)
                    Right = false;
            }


        }

        /*if(mode == start) {
            startText.setString("Welcome to pong!\nPress space to start the game");
            startText.setPosition(gameWidth/2.f, gameHeight/2.f -20);
            startText.setOrigin(startText.getLocalBounds().width/2.0f,startText.getLocalBounds().height/2.0f);
        }

        if(mode == finish) {
            finishText.setString("Defeat");
            finishText.setPosition(gameWidth/2.f, gameHeight/2.f - 20);
            finishText.setOrigin(finishText.getLocalBounds().width/2.0f,finishText.getLocalBounds().height/2.0f);

            }*/

        if (mode == playing)
        {
            //float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (Left == true &&
             (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move(0.f, -paddleSpeed/* * deltaTime*/);
            }
            if (Right == true &&
             (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.move(0.f, paddleSpeed/* * deltaTime*/);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.move(0.f, rightPaddleSpeed/* * deltaTime*/);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            float factor = ballSpeed /** deltaTime*/;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                //isPlaying = false;
                //mode = start;
                //startText.setString("You lost!\nPress space to restart or\nescape to exit");
                ball.setPosition(gameWidth/2.f - ballRadius, gameHeight/2.f - ballRadius);
                do
                {
                        // Make sure the ball initial angle is not too much vertical
                    ballAngle = (std::rand() % 360) * 2 * pi / 360;
                }
                while (std::abs(std::cos(ballAngle)) < 0.7f);
                rightScore++;
                score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));

                if(rightScore > 1)
                    mode = finish;

            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                //isPlaying = false;
                //mode = start;
                //startText.setString("You won!\nPress space to restart or\nescape to exit");
                ball.setPosition(gameWidth/2.f - ballRadius, gameHeight/2.f - ballRadius);
                do
                {
                        // Make sure the ball initial angle is not too much vertical
                    ballAngle = (std::rand() % 360) * 2 * pi / 360;
                }
                while (std::abs(std::cos(ballAngle)) < 0.7f);
                leftScore++;
                score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));
                if(leftScore > 1)
                    mode = finish;

            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                //ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                //ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                //ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                //ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }

        // Clear the window
        window.clear(/*sf::Color(50, 200, 50)*/);

        if (/*isPlaying*/ mode == playing)
        {
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
            window.draw(score);
        }
        else if(mode == start)
        {
            // Draw the pause message
            window.draw(startText);
        }

        else if(mode == finish)
        {
            // Draw the pause message
            window.draw(finishText);
        }


        // Display things on screen
        window.display();
    }

    return 0;
}
