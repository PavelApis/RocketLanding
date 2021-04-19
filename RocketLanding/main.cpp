#include <SFML/Graphics.hpp>
#include "Rocket.h"
#include <cstdlib>
#include <string>
#include <ctime>
#include <random>

using namespace sf;

int main()
{
	//------- App's Window -------//
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RocketLanding");
	random_device rd;
	mt19937 mersenne(rd());
	int landingFieldPosition = mersenne() % (WINDOW_WIDTH - LANDING_FIELD_LENGHT);
	Rocket rocket;

	// Timer: 
	srand(static_cast<unsigned>(time(0)));


	// Game State:
	enum GAME
	{
		ON,
		WIN,
		LOSE
	};
	GAME game = ON;

	RectangleShape h(sf::Vector2f(WINDOW_HEIGHT - rocket.getY()- 30, 5));
	h.setFillColor(Color(255, 0, 0));
	h.rotate(90);
	
	RectangleShape rR(sf::Vector2f(WINDOW_WIDTH - rocket.getX() - 10, 5));
	rR.setFillColor(Color(255, 0, 0));

	RectangleShape rL(sf::Vector2f(-rocket.getX() + 10, 5));
	rL.setFillColor(Color(255, 0, 0));

	RectangleShape sLF(sf::Vector2f(rocket.getX() > landingFieldPosition ? rocket.getX() - landingFieldPosition - LANDING_FIELD_LENGHT : landingFieldPosition - rocket.getX(), 5));
	sLF.setFillColor(Color(0, 255, 0));
	


	// Drawing a Ground and a Landing Field:
	RectangleShape ground(Vector2f(WINDOW_WIDTH, 19.f));
	ground.setOutlineThickness(1.f);
	ground.setOutlineColor(Color::Black);
	ground.setFillColor(Color::White);
	ground.setPosition(0, WINDOW_HEIGHT - 10);

	RectangleShape landingField(Vector2f(LANDING_FIELD_LENGHT, 19.f));
	landingField.setFillColor(Color(250, 250, 250));
	landingField.setOutlineThickness(1.f);
	landingField.setOutlineColor(Color::Black);
	landingField.setPosition(landingFieldPosition, WINDOW_HEIGHT - 30);

	RectangleShape rocketHIT(Vector2f(5, 5));
	rocketHIT.setFillColor(Color(100, 100, 100));


	// Drawing a Rocket and an Explosion:
	Texture RocketTexture;
	RocketTexture.loadFromFile("D:\\C++\\RocketLandingSolution-main\\images\\Rocket.png");
	RocketTexture.setSmooth(true);
	Sprite RocketSprite(RocketTexture);
	RocketSprite.scale(1, 1);

	Texture ExplosionTexture;
	ExplosionTexture.loadFromFile("D:\\C++\\RocketLandingSolution-main\\images\\Explosion.png");
	ExplosionTexture.setSmooth(true);
	IntRect rectSourceSprite(0, 0, 96, 96);
	Sprite ExplosionSprite(ExplosionTexture, rectSourceSprite);

	// Declaration of Time Lose for Explosion animation:
	clock_t tLose;

	// Declaration of paramaters for Explosion animation: 
	int i = 0;
	int j = 0;

	Texture FlameTexture;
	FlameTexture.loadFromFile("D:\\C++\\RocketLandingSolution-main\\images\\Flame.png");
	FlameTexture.setSmooth(true);
	Sprite FlameSprite(FlameTexture);

	// Main Application Cycle:
	while (window.isOpen())
	{
		
		if (game == ON) {

			//rocket.landingAlgorithm(LANDING_FIELD_LENGHT, landingFieldPosition,
				//GRAVITY_ACCELERATION, AIR_RESISTANCE, WINDOW_HEIGHT,
				//WINDOW_WIDTH, CRUSH_SPEED, rocket.getFuel());

			rocket.setVelocityX(rocket.getVelocityX() + rocket.getSidePower() - AIR_RESISTANCE);
			rocket.setVelocityY(rocket.getVelocityY() + rocket.getCentralPower() + GRAVITY_ACCELERATION);

			rocket.setX(rocket.getX() + rocket.getVelocityX());
			rocket.setY(rocket.getY() + rocket.getVelocityY());

			// Setting parameters for Lose:
			if (rocket.getX() < 0
				|| rocket.getX() > WINDOW_WIDTH
				|| rocket.getY() < 0
				|| rocket.getY() > WINDOW_HEIGHT - 20
				|| (rocket.getX() >= landingFieldPosition
					&& rocket.getX() <= landingFieldPosition + LANDING_FIELD_LENGHT
					&& rocket.getY() > (WINDOW_HEIGHT - 45)
					&& (rocket.getVelocityX() * rocket.getVelocityX()
						+ rocket.getVelocityY() * rocket.getVelocityY() > 25)))
			{
				game = LOSE;
				tLose = clock();
				continue;
			}
			// Setting parameters for Win:
			else if (rocket.getX() >= landingFieldPosition
				&& rocket.getX() <= landingFieldPosition + LANDING_FIELD_LENGHT
				&& rocket.getY() > (WINDOW_HEIGHT - 45)
				&& (rocket.getVelocityX() * rocket.getVelocityX()
					+ rocket.getVelocityY() * rocket.getVelocityY() <= 25))
			{
				game = WIN;
				continue;
			}
			else
			{
				window.clear(Color::White);
				if (rocket.getSidePower() > 0)
				{
					rocket.setFuel(rocket.getFuel() - rocket.getSidePower());
					FlameSprite.setPosition(rocket.getX() - 20, rocket.getY());
					window.draw(FlameSprite);

				}
				else if (rocket.getSidePower() < 0)
				{
					rocket.setFuel(rocket.getFuel() + rocket.getSidePower());
					FlameSprite.setPosition(rocket.getX() - 20, rocket.getY());
					window.draw(FlameSprite);
				}
				if (rocket.getCentralPower() < 0)
				{
					rocket.setFuel(rocket.getFuel() + rocket.getCentralPower());
					FlameSprite.setPosition(rocket.getX() - 20, rocket.getY());
					window.draw(FlameSprite);
				}
				rocket.setCentralPower(0);
				rocket.setSidePower(0);

				// Processing queue of events of cycle:
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();

					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::A)
							if (rocket.getFuel() > 0)
							{
								rocket.setSidePower(-2 * GRAVITY_ACCELERATION);

								FlameSprite.setPosition(rocket.getX() - 55, rocket.getY());
								window.draw(FlameSprite);
							}
						if (event.key.code == Keyboard::D)
							if (rocket.getFuel() > 0)
							{
								rocket.setSidePower(2 * GRAVITY_ACCELERATION);

								FlameSprite.setPosition(rocket.getX() - 55, rocket.getY());
								window.draw(FlameSprite);
							}
						if (event.key.code == Keyboard::W)
							if (rocket.getFuel() > 0)
							{
								rocket.setCentralPower(-2 * GRAVITY_ACCELERATION);

								FlameSprite.setPosition(rocket.getX() - 55, rocket.getY());
								window.draw(FlameSprite);
							}
						if (event.key.code == Keyboard::S)
						{
							rocket.setCentralPower(2 * GRAVITY_ACCELERATION);

						}
					}
				}

				h.setPosition(rocket.getX(), rocket.getY());
				h.setSize(sf::Vector2f(WINDOW_HEIGHT - rocket.getY() - 30, 5));
				window.draw(h);

				rR.setPosition(rocket.getX(), rocket.getY());
				rR.setSize(sf::Vector2f(WINDOW_WIDTH - rocket.getX() - 10, 5));
				window.draw(rR);

				rL.setPosition(rocket.getX(), rocket.getY());
				rL.setSize(sf::Vector2f(- rocket.getX() + 10, 5));
				window.draw(rL);

				sLF.setPosition(rocket.getX(), rocket.getY());
				sLF.setSize(sf::Vector2f(rocket.getX() > landingFieldPosition ? -(rocket.getX() - landingFieldPosition - LANDING_FIELD_LENGHT) : landingFieldPosition - rocket.getX(), 5));
				window.draw(sLF);

				window.draw(ground);
				window.draw(landingField);

				RocketSprite.setPosition(rocket.getX() - 13.5, rocket.getY() - 70);
				window.draw(RocketSprite);

				rocketHIT.setPosition(rocket.getX(), rocket.getY());
				//window.draw(rocketHIT);
			}

			cout << "\n ROCKET LANDING GAME " << "\n You may use WASD buttons! \n";

			if (rocket.getFuel() >= 0)
			{
				cout << "\n FUEL: " << rocket.getFuel();
			}
			else
			{
				cout << "\n YOU'RE OUT OF FUEL! ";
			}

			cout << "\n\n COORDINATES: \n X: " << rocket.getX() << "; Y: " << rocket.getY() <<
				"\n\n VELOCITY: \n Vx: " << rocket.getVelocityX() << "; Vy: " << rocket.getVelocityY() <<
				"\n\n ACCELERATION: \n Ax: " << rocket.getSidePower() << "; Ay: " << rocket.getCentralPower() + GRAVITY_ACCELERATION;

			system("cls");
		}
		else if (game == WIN)
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			window.clear(Color::White);

			window.draw(ground);
			window.draw(landingField);



			RocketSprite.setPosition(rocket.getX() - 16, rocket.getY() - 70);
			window.draw(RocketSprite);

			Texture WinTexture;
			WinTexture.loadFromFile("D:\\C++\\RocketLandingSolution-main\\images\\Win.png");
			WinTexture.setSmooth(true);
			Sprite WinSprite(WinTexture);
			WinSprite.setPosition(WINDOW_WIDTH / 2 - 450, WINDOW_HEIGHT / 2 - 150);
			window.draw(WinSprite);
		}
		else if (game == LOSE)
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			ExplosionSprite.setPosition(rocket.getX() - 60, rocket.getY() - 100);
			clock_t t = clock();
			if ((t - tLose) % 50 == 0)
			{
				IntRect rectSourceSprite(96 * i, 96 * j, 96, 96);
				ExplosionSprite.setTextureRect(rectSourceSprite);
				i++;
				if (i == 5)
				{
					if (j != 3)
					{
						j++;
					}
					i = 0;
				}
			}
			window.clear(Color::White);

			window.draw(ground);
			window.draw(landingField);

			window.draw(ExplosionSprite);

			//RocketSprite.setPosition(rocket.getX() - 35, rocket.getY() - 70);
			//window.draw(RocketSprite);
			Texture LoseTexture;
			LoseTexture.loadFromFile("D:\\C++\\RocketLandingSolution-main\\images\\Lose.png");
			LoseTexture.setSmooth(true);
			Sprite LoseSprite(LoseTexture);
			LoseSprite.setPosition(WINDOW_WIDTH / 2 - 450, WINDOW_HEIGHT / 2 - 150);
			window.draw(LoseSprite);

		}

		// Drawing a window:	
		window.display();
	}
	system("pause");
	return 0;
}