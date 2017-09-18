#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace sf;
using namespace std;

const int windowWidth = 800;
const int windowHeight = 600;

sf::Vector2f paddleSize(25, 100);
float ballRadius = 8.f;
enum Mode { start, playing, finish };
Mode mode = start;
sf::Font font;

float paddleSpeed = 12.f;
float ballSpeed   = 8.f;
float ballAngle = 0;

int leftScore = 0;
int rightScore = 0;
bool Up = false;
bool Down = false;

    // unused. Didn't seem necessary.
struct Paddle {
public:
    sf::RectangleShape rect;
    float speed;

    public: void init(sf::Vector2f dimensions, float speed) {
        rect.setSize(dimensions);
            //rect.setFillColor(sf::Color(100, 100, 200));
        rect.setOrigin(dimensions / 2.f);
        this->speed = speed;
    }
};

    // moves ball back to center and sends it in random direction
void resetBall(sf::CircleShape &ball) {
        //sf::sleep(sf::seconds(5));
    ball.setPosition(windowWidth / 2, windowHeight / 2);

    int flip = 0;

    if(std::rand()%2==1)
        flip = 180;
    else
        flip = 0;

    ballAngle = (flip + (std::rand() % 45) - 45);
}

int main()
{

    std::srand(std::time(0));

   // Load sound and text first
    sf::SoundBuffer sound;
    if (!sound.loadFromFile("../res/ding.wav"))
        return 0;
    sf::Sound ballSound(sound);

    if (!font.loadFromFile("../res/ChocolateCoveredRaindrops.ttf"))
        return 0;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Pong",
        sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(60);

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(60);
    score.setColor(sf::Color::White);
    score.setString("0 - 0");
    score.setPosition(windowWidth/2.f, 0);
    score.setOrigin(score.getLocalBounds().width/2.0f,score.getLocalBounds().height/2.0f);

    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(40);
    startText.setColor(sf::Color::White);
    startText.setString("Pong!\nPress ENTER to start.\nType 1-3 for Difficulty\nEsc to exit");
    startText.setPosition(windowWidth/2.f, windowHeight/2.f - 20);
    startText.setOrigin(startText.getLocalBounds().width/2.0f,startText.getLocalBounds().height/2.0f);

    sf::Text finishText;
    finishText.setFont(font);
    finishText.setCharacterSize(40);
    finishText.setColor(sf::Color::White);
    finishText.setString("Finished!\nPress ENTER to play again.\nEsc to quit.");
    finishText.setPosition(windowWidth/2.f, windowHeight/2.f - 20);
    finishText.setOrigin(finishText.getLocalBounds().width/2.0f,finishText.getLocalBounds().height/2.0f);

        // Left pad
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize);
    leftPaddle.setFillColor(sf::Color::Cyan);
    leftPaddle.setOrigin(paddleSize / 2.f);

        //Paddle leftPad = 

        // Right pad
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize);
    rightPaddle.setFillColor(sf::Color::Magenta);
    rightPaddle.setOrigin(paddleSize / 2.f);

        // ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

        // Difficulty Button
    sf::RectangleShape difficultyRect(sf::Vector2f(50, 50));
    difficultyRect.setFillColor(sf::Color::White);
    sf::Text difficultyText;
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(50);
    difficultyText.setColor(sf::Color::Black);
    difficultyText.setString("2");
    difficultyText.setPosition(windowWidth/1.5f + 15, windowHeight/1.5f - 12);
    difficultyRect.setPosition(windowWidth/1.5f, windowHeight/1.5f);


    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {

          switch (event.type) {

            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
            // Window closed or escape key pressed: exit
            if(event.key.code == sf::Keyboard::Escape)
            {
             window.close();
             break;
         }

            // Hitting Enter Starts the Game
         else if ((event.key.code == sf::Keyboard::Return))
         {
            if(mode == start)
            {
                mode = playing;

                    //reset score
                leftScore = 0;
                rightScore = 0;
                score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));

                    // Reset shapes
                leftPaddle.setPosition(paddleSize.x, windowHeight / 2);
                rightPaddle.setPosition(windowWidth - paddleSize.x, windowHeight / 2);
                resetBall(ball);

                ballSpeed = std::stof((std::string) difficultyText.getString()) * 4;
            }
            if(mode == finish)
                mode = start;
        }

        else if (event.key.code == Keyboard::Up) {
            Up = true;
            break;
        }

        else if (event.key.code == Keyboard::Down) {
            Down = true;
            break;
        }

        case sf::Event::KeyReleased:
        if (event.key.code == Keyboard::Up)
            Up = false;

        else if (event.key.code == Keyboard::Down)
            Down = false;

        break;
    }
}

if(mode == start) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        difficultyText.setString("1");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        difficultyText.setString("2");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
    {
        difficultyText.setString("3");
    }
}

if (mode == playing)
{
    // Player's paddle responses
    if (Up == true &&
       (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
    {
        leftPaddle.move(0.f, -paddleSpeed);
    }
    if (Down == true &&
       (leftPaddle.getPosition().y + paddleSize.y / 2 < windowHeight - 5.f))
    {
        leftPaddle.move(0.f, paddleSpeed);
    }

    // Move AI paddle
    if ((rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f) && (ball.getPosition().y + (std::rand() % 10 - 20) - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2))
    {
        rightPaddle.move(0.f, -paddleSpeed/2);
    }
    else if((rightPaddle.getPosition().y + paddleSize.y / 2 < windowHeight - 5.f) && (ball.getPosition().y + (std::rand() % 10 - 20) + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2))
        rightPaddle.move(0.f, paddleSpeed/2);

    // Move ball
    ball.move(std::cos(ballAngle * 3.14159f/180) * ballSpeed, std::sin(ballAngle * 3.14159f/180) * ballSpeed);

    // If point is scored
    if (ball.getPosition().x - ballRadius < 0.f)
    {
        resetBall(ball);
        rightScore++;
        score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));

        if(rightScore > 1) {
            mode = finish;
            finishText.setString("Computer Wins!\nPress ENTER to play again.\nEsc to quit.");

        }
    }

    if (ball.getPosition().x + ballRadius > windowWidth)
    {
        resetBall(ball);
        leftScore++;
        score.setString( std::to_string(leftScore) + " - " +  std::to_string(rightScore));
        if(leftScore > 1) {
            mode = finish;
            finishText.setString("Player Wins!\nPress ENTER to play again.\nEsc to quit.");
        }

    }

    // If ball hits top or bottom wall
    if (ball.getPosition().y - ballRadius < 0.f || (ball.getPosition().y + ballRadius > windowHeight))
    {
        ballSound.play();
        ballAngle = -ballAngle;
    }

    if(ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) || ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()))
    {
        ballAngle = 180 - ballAngle - 15 + std::rand() % 30;
        ballSound.play();
    }

}

window.clear();

if(mode == start)
{
    window.draw(startText);
    window.draw(difficultyRect);
    window.draw(difficultyText);
}
else if (mode == playing)
{
    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.draw(ball);
    window.draw(score);
}

else if(mode == finish)
{
    window.draw(leftPaddle);
    window.draw(rightPaddle);
        //window.draw(ball);
    window.draw(score);
    window.draw(finishText);
}

window.display();
}

return 0;
}
