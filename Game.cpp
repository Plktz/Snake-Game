#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <sstream>

int grid_size = 30,
    score = 0;

sf::Vector2f pixel_size(30.f, 30.f);

class Food
{
public:
    sf::Color food_color;
    sf::RectangleShape food_rect;
    sf::FloatRect food_bound;
    Food()
    {
        food_color = {21, 131, 209};

        food_rect.setPosition(pixel_size.x * (grid_size / 2), pixel_size.y * (grid_size / 2));
        food_rect.setSize(pixel_size);
        food_rect.setFillColor(food_color);
        food_bound = food_rect.getGlobalBounds();
    }

    void reset()
    {
        food_rect.setPosition(pixel_size.x * (grid_size / 2), pixel_size.y * (grid_size / 2));
        food_bound = food_rect.getGlobalBounds();
    }

    void update()
    {
        int x = (rand() % grid_size) * pixel_size.x,
            y = (rand() % grid_size) * pixel_size.x;
        food_rect.setPosition(sf::Vector2f(x, y));
        food_bound = food_rect.getGlobalBounds();
    }
} food;

class Snake
{
public:
    sf::Color head_color;
    sf::Color tail_color;
    sf::RectangleShape tail[50];
    sf::FloatRect tail_bound[50];
    sf::RectangleShape head;
    sf::FloatRect head_bound;
    sf::Vector2f velocity;
    int speed,
        time_per_move,
        length;
    float time_per_move_dec;

    Snake()
    {
        head_color = {249, 110, 90};

        tail_color = {254, 208, 106};

        length = 3;
        time_per_move = 200;
        time_per_move_dec = 10;
        speed = pixel_size.x;
        velocity.x = 0;
        velocity.y = speed;

        for (int i = 0; i < length; i++)
        {
            tail[i].setSize(pixel_size);
            tail[i].setPosition(sf::Vector2f(pixel_size.x, pixel_size.y * (i + 1)));
            tail[i].setFillColor(tail_color);
        }

        head.setPosition(sf::Vector2f(pixel_size.x, pixel_size.y * 4));
        head.setSize(pixel_size);
        head.setFillColor(head_color);
        head_bound = head.getGlobalBounds();
    }

    void reset()
    {
        length = 3;
        time_per_move = 200;
        time_per_move_dec = 10;
        speed = pixel_size.x;
        velocity.x = 0;
        velocity.y = speed;

        for (int i = 0; i < length; i++)
        {
            tail[i].setPosition(sf::Vector2f(pixel_size.x, pixel_size.y * (i + 1)));
        }

        head.setPosition(sf::Vector2f(pixel_size.x, pixel_size.y * 4));
        head_bound = head.getGlobalBounds();
    }

    void update_dir(char x)
    {
        switch (x)
        {
        case 'w':
            if (velocity != sf::Vector2f(0, speed))
            {
                velocity.x = 0;
                velocity.y = -1 * speed;
            }
            break;
        case 'a':
            if (velocity != sf::Vector2f(speed, 0))
            {
                velocity.x = -1 * speed;
                velocity.y = 0;
            }
            break;
        case 's':
            if (velocity != sf::Vector2f(0, -1 * speed))
            {
                velocity.x = 0;
                velocity.y = speed;
            }
            break;
        case 'd':
            if (velocity != sf::Vector2f(-1 * speed, 0))
            {
                velocity.x = speed;
                velocity.y = 0;
            }
            break;

        default:
            break;
        }
    }

    void ate()
    {
        length += 1;
        tail[length - 1].setSize(pixel_size);
        tail[length - 1].setFillColor(tail_color);
        tail[length - 1].setPosition(tail[length - 2].getPosition());
        time_per_move -= time_per_move_dec;
        time_per_move_dec == time_per_move_dec - 10 * (1 / time_per_move_dec);
    }

    void update()
    {
        for (int i = length - 1; i > 0; i--)
        {
            tail[i].setPosition(tail[i - 1].getPosition());
        }
        tail[0].setPosition(head.getPosition());

        for (int i = 0; i < length; i++)
            tail_bound[i] = tail[i].getGlobalBounds();

        head.move(velocity);
        head_bound = head.getGlobalBounds();
    }

    void snake_draw(sf::RenderWindow *window)
    {
        for (int i = 0; i < length; i++)
            window->draw(tail[i]);
        window->draw(head);
    }
} snake;

int main()
{
    int game_over = 0;
    sf::Clock clock;
    sf::Color background(136, 216, 176);
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text Score_Board("Score : 0", font, 20);
    Score_Board.setFillColor(sf::Color{249, 110, 120});
    Score_Board.setOutlineColor(sf::Color{101, 203, 218});
    Score_Board.setOutlineThickness(1);
    Score_Board.setPosition(750, 20);

    sf::Text Final_Score("Score : 0", font, 50);
    Final_Score.setFillColor(sf::Color{249, 110, 120});
    Final_Score.setOutlineColor(sf::Color{101, 203, 218});
    Final_Score.setOutlineThickness(1);
    Final_Score.setPosition(330, 220);

    sf::Text Game_Over("Game Over", font, 100);
    Game_Over.setFillColor(sf::Color{249, 110, 120});
    Game_Over.setOutlineColor(sf::Color{101, 203, 218});
    Game_Over.setOutlineThickness(1);
    Game_Over.setPosition(200, 100);

    sf::Text Replay("Press any key to continue.", font, 30);
    Replay.setFillColor(sf::Color{250, 90, 150});
    Replay.setOutlineColor(sf::Color{101, 203, 218});
    Replay.setOutlineThickness(1);
    Replay.setPosition(265, 280);

    sf::RenderWindow window(sf::VideoMode(900, 900), "Snake");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    snake.update_dir('w');
                    break;
                case sf::Keyboard::A:
                    snake.update_dir('a');
                    break;
                case sf::Keyboard::S:
                    snake.update_dir('s');
                    break;
                case sf::Keyboard::D:
                    snake.update_dir('d');
                    break;

                default:
                    break;
                }

                if (game_over == 1)
                {
                    snake.reset();
                    food.reset();
                    score = 0;
                    Score_Board.setString("Score : 0");
                    Final_Score.setString("Score : 0");
                    game_over = 0;
                }
            }
        }

        if (game_over == 0)
        {
            if (snake.head_bound.intersects(food.food_bound))
            {
                food.update();
                score += 10;
                std::ostringstream sss;
                sss << "Score : " << score;
                sf::String Score_string = sss.str();
                Score_Board.setString(Score_string);
                Final_Score.setString(Score_string);
                snake.ate();
            }

            if (clock.getElapsedTime().asMilliseconds() > snake.time_per_move)
            {
                snake.update();
                clock.restart();
            }

            for (int i = 0; i < snake.length; i++)
            {
                if (snake.head_bound.intersects(snake.tail_bound[i]))
                    game_over = 1;
            }
        }

        for (int i = 0; i < snake.length; i++)
        {
            if (snake.head_bound.intersects(snake.tail_bound[i]))
                game_over = 1;
        }

        if (snake.head.getPosition().x < 0 ||
            snake.head.getPosition().x > 900 ||
            snake.head.getPosition().y < 0 ||
            snake.head.getPosition().y > 900)
            game_over = 1;

        window.clear(background);
        snake.snake_draw(&window);
        window.draw(food.food_rect);
        window.draw(Score_Board);
        if (game_over == 1)
        {
            window.draw(Game_Over);
            window.draw(Final_Score);
            window.draw(Replay);
        }
        window.display();
    }
}