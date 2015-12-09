#include <SFML/Graphics.hpp>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "config.h"

using namespace sf;
using namespace std;

void position_hand(ConvexShape &hand, float height, float width) {
	hand.setPointCount(3);
	hand.setPoint(0, sf::Vector2f(0, 0));
	hand.setPoint(1, sf::Vector2f(width / 2, -height));
	hand.setPoint(2, sf::Vector2f(width, 0));
	hand.setPosition(float(START_X), float(START_Y));
	hand.setOrigin(width / 2, -height / 8);
}

void shapes_position(Shapes &shape) {
	position_hand(shape.hourHand, 140, 12);
	shape.hourHand.setFillColor(sf::Color::Black);
	position_hand(shape.minuteHand, 175, 8);
	shape.minuteHand.setFillColor(sf::Color::Black);
	position_hand(shape.secondHand, 190, 6);
	shape.secondHand.setFillColor(sf::Color::Red);
	shape.backgroung.setSize(WINDOW_SIZE);
	shape.backgroung.setPosition(0, 0);
}

void coordinate_points(sf::Vector2f(&pointCoordinates)[AMOUNT_POINTS]) {
	sf::Vector2f coordinatePoint;
	for (int i = 0; i < AMOUNT_POINTS; i++) {
		coordinatePoint.x = START_X + SCALE * cos(i * 6 * float(M_PI) / 180);
		coordinatePoint.y = START_Y + SCALE * sin(i * 6 * float(M_PI) / 180);
		pointCoordinates[i] = { coordinatePoint.x, coordinatePoint.y };
	}
}

void draw_secondPoint(RenderWindow &window, Shapes &shape, sf::Vector2f(&pointCoordinates)[AMOUNT_POINTS]) {
	for (int i = 0; i < AMOUNT_POINTS; i++) {
		if (i % 15 == 0) {
			shape.point.setRadius(3);
			shape.point.setOrigin(6 / 2, 6 / 2);
			shape.point.setFillColor(sf::Color::Black);
		}
		else if (i % 5 == 0) {
			shape.point.setRadius(2);
			shape.point.setOrigin(4 / 2, 4 / 2);
			shape.point.setFillColor(sf::Color::Black);
		}
		else {
			shape.point.setRadius(1);
			shape.point.setOrigin(2 / 2, 2 / 2);
			shape.point.setFillColor(sf::Color::Black);
		}
		shape.point.setPosition(pointCoordinates[i].x, pointCoordinates[i].y);
		window.draw(shape.point);
	}
}

void start_time(RenderWindow &window, Shapes &shape) {
	sf::Vector2f pointCoordinates[AMOUNT_POINTS];
	SYSTEMTIME sysTime;
	coordinate_points(pointCoordinates);
	while (window.isOpen()) {
		GetSystemTime(&sysTime);
		shape.secondHand.setRotation(float(sysTime.wSecond * 6)); //по 6 градусов на одну секунду 
		shape.minuteHand.setRotation(float(sysTime.wMinute * 6));
		shape.hourHand.setRotation(float((sysTime.wHour + 3) * 30));
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(shape.backgroung);
		window.draw(shape.hourHand);
		window.draw(shape.minuteHand);
		window.draw(shape.secondHand);
		draw_secondPoint(window, shape, pointCoordinates);
		window.display();
	}
}

int main() {
	Shapes shape;
	shapes_position(shape);

	sf::ContextSettings settings; // аньтяльзинг 
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(unsigned int(WINDOW_SIZE.x), unsigned int(WINDOW_SIZE.y)), "Clock", sf::Style::Default, settings);
	
	start_time(window, shape);
	return 0;
}