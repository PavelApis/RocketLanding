#pragma once
#include <iostream>
using namespace std;


//Changable parameters:
const float FUEL = 100;
const float CRUSH_SPEED = 5.0;
const float ROCKET_INITIAL_X = 200.0;
const float ROCKET_INITIAL_Y = 10.0;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
const int LANDING_FIELD_LENGHT = 100;
const float GRAVITY_ACCELERATION = 0.1;
const float AIR_RESISTANCE = 0.02;

struct Rocket
{
private:

	float x, y, fuel,
		velocityX, velocityY,
		sidePower, centralPower;

public:

	Rocket(float x_ = ROCKET_INITIAL_X, float y_ = ROCKET_INITIAL_Y, float fuel_ = FUEL) :
x(x_), y(y_), fuel(fuel_),
velocityX(0), velocityY(0),
sidePower(0), centralPower(0) {}

//------- GETTERS -------//
float getX()
{
	return x;
}

float getY()
{
	return y;
}

float getVelocityX()
{
	return velocityX;
}

float getVelocityY()
{
	return velocityY;
}

float getFuel()
{
	return fuel;
}

float getSidePower()
{
	return sidePower;
}

float getCentralPower()
{
	return centralPower;
}
//------- ------- -------//

//------- SETTERS -------//
void setX(float coordinate)
{
	x = coordinate;
}

void setY(float coordinate)
{
	y = coordinate;
}

void setVelocityX(float velocity)
{
	velocityX = velocity;
}

void setVelocityY(float velocity)
{
	velocityY = velocity;
}

void setFuel(float fuel_)
{
	fuel = fuel_;
}

void setSidePower(float power)
{
	sidePower = power;
}

void setCentralPower(float power)
{
	centralPower = power;
}
//------- ------- -------//

void landingAlgorithm(int landingFieldSize, int landingFieldPosition, float gravity, float wind,
	int windowHeight, int windowWidth, float crushSpeed, float fuel);

};

void Rocket::landingAlgorithm(int landingFieldSize, int landingFieldPosition, float gravity, float wind,
	int windowHeight, int windowWidth, float crushSpeed, float fuel) {

	float x = getX(), y = getY(), vX = getVelocityX(), vY = getVelocityY();
	float speed = sqrt(vX*vX + vY * vY);

	float h = windowHeight - y - 30;
	float rToBoard  = vX > 0 ? WINDOW_WIDTH - x - 10  : - x + 10;
	int directionX = vX > 0 ? 1 : -1;
	float sLF = x > landingFieldPosition ? -x + landingFieldPosition + landingFieldSize : landingFieldPosition - x;


	float tToBroadWithoutP = (-directionX*vX + sqrt(vX*vX + 2 * (abs(wind)*directionX*rToBoard))) / abs(wind);
	float tOfFallWithoutP = (-vY + sqrt(vY*vY + 2 * gravity*h)) / gravity;
	float tToBroadWithP = -1;
	float tOfFallWithP = -1;

	if (landingFieldPosition + landingFieldSize > x && x > landingFieldPosition) {
		sLF = 0;
	}
	else if (x > landingFieldPosition) {
		sLF = landingFieldPosition + landingFieldSize - x;
	}
	else if (x < landingFieldPosition) {
		sLF = landingFieldPosition - x;
	}
	


	// WE KNOW: H = Vt + (gt^2)/2, THEN 2V^3 > 2gH
	// Solving a problem to set power for a rocket to get to the platform:

	if (getFuel() > 0)
	{
		if (speed < (1 - 2*gravity) * crushSpeed && sLF != 0) {
			setSidePower((gravity)* sLF / abs(sLF));
		}
		else if (speed > (1 - 2 * gravity) * crushSpeed && vY < 3) {
			setSidePower(gravity * (-directionX));
		}
		else if (sLF == 0 && abs(vX) > 0.1) {
			setSidePower(gravity * (-directionX));
		}

		//float tOfCrushX = -1
		//if(vX*vX - 2* (0.9*(gravity ))

		cout << "\n\n\n" << vX * vX - 2 * (0.9 * (gravity - abs(wind)) * directionX * rToBoard) << "\n\n\n";

		if (vX*vX - 2 * (0.9 * (gravity - abs(wind)) * directionX * rToBoard) > 0) {
			tToBroadWithP = abs((-vX + sqrt(vX*vX - 2 * (0.9 * (gravity - abs(wind)) * directionX * rToBoard))) / abs(wind));
		}
		if (tToBroadWithP > 0) {
			setSidePower(-directionX*(2*gravity+abs(wind)));
		}
		

		if (vY*vY - 2 * (0.9*gravity)*h > 0) {
			tOfFallWithP = abs((-vY + sqrt(vY*vY - 2 * (0.9*gravity)*h)) / gravity);
		}
		if (tOfFallWithP > 0 && vY > -1) {
			setCentralPower(-2 * gravity);
		}		
	}
	else
	{
		cout << "\n OUT OF FUEL! \n";
	}

	cout << "\nHeight: " << h << "\n Distance to Board in present way: " << rToBoard << "\nS to LF: " << sLF << "\nTime of fall: " << tOfFallWithoutP
		<< "\nTime to Board in present direction" << tToBroadWithoutP << " " << tOfFallWithP << "\nTime of stop: "
		<< "\nCentral Power " << getCentralPower() << "\n Speed: " << speed;
}