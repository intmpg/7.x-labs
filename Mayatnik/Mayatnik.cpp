#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

ConvexShape stick(){
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(-76, 105));
	convex.setPoint(2, sf::Vector2f(0, 10));
	convex.setPoint(3, sf::Vector2f(76, 108));
	convex.setFillColor(Color::Blue);
	return convex;
}

ConvexShape cargo() {
	sf::ConvexShape convex;
	convex.setPointCount(7);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(-40, 0));
	convex.setPoint(2, sf::Vector2f(-10, -40));
	convex.setPoint(3, sf::Vector2f(-10, -250));
	convex.setPoint(4, sf::Vector2f(10, 250));
	convex.setPoint(5, sf::Vector2f(10, -40));
	convex.setPoint(6, sf::Vector2f(40, 0));
	convex.setFillColor(Color::Yellow);
	convex.setOrigin(0, -250);
	return convex;
}

float x_position(float rad_b, float & grad_pos) {
	float x = float(rad_b * cos(grad_pos * M_PI / 180));
	return x;
}

float y_position(float rad_b, float & grad_pos) {
	float y = float(rad_b * sin(grad_pos * M_PI / 180));
	return y;
}

ConvexShape shark(float x_pos, float y_pos, int tooth, float rad_b, float rad_s) {
	sf::ConvexShape convex;
	float x1 = 0;
	float y1 = 0;
	float x, y;
	float grad = float(360 / (tooth * 3));
	int point_num = tooth * 3;
	std::cout << tooth << std::endl;
	bool big = true;
	int flag = 0;
	float grad_pos = 0;
	convex.setPointCount(point_num);
	convex.setPosition(x_pos, y_pos);
	convex.setFillColor(Color::Green);
	for (int i = 0; i < point_num; i++)
	{

		if (big) {
			grad_pos = grad_pos - grad;
			x = x_position(rad_b, grad_pos);
			y = y_position(rad_b, grad_pos);
			grad_pos = grad_pos + grad;
			flag++;
			if (flag == 1) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x_position(rad_s, grad_pos);
			y = y_position(rad_s, grad_pos);
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		convex.setPoint(i, sf::Vector2f(x, y));
		grad_pos = grad_pos + grad;
	}
	return convex;
}

ConvexShape gear(float x_pos, float y_pos, int tooth, float rad_b, float rad_s) {
	sf::ConvexShape convex;
	float x1 = 0;
	float y1 = 0;
	float x, y;
	float grad = float(360 / (tooth * 4));
	int point_num = tooth * 4;
	bool big = true;
	int flag = 0;
	float grad_pos = 0;
	convex.setPointCount(point_num);
	convex.setPosition(x_pos, y_pos);
	convex.setFillColor(Color::Blue);
	for (int i = 0; i < point_num; i++)
	{

		if (big) {
			x = x_position(rad_b, grad_pos);
			y = y_position(rad_b, grad_pos);
			flag++;
			if (flag == 2) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x_position(rad_s, grad_pos);
			y = y_position(rad_s, grad_pos);
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		if (i == 0) {

		}
		convex.setPoint(i, sf::Vector2f(x, y));
		grad_pos = grad_pos + grad;
	}
	return convex;
}


struct Mayatnik {
	Mayatnik(int value1, int value2, int value3):
	gear_tooth1(value1), gear_tooth2(value2), shark_tooth(value3), present_degree(0), present_speed(0)
	{}
	
	ConvexShape gear1;
	int gear_tooth1;
	ConvexShape gear2;
	int gear_tooth2;
	ConvexShape shark1;
	int shark_tooth;
	ConvexShape bob;
	RectangleShape rope;
	CircleShape plumb;

	const float max_degree = 20;
	float present_degree;
	float present_speed;

};

void mechanism_step(Mayatnik & Mayatnik, float & boost, bool & isMove) {

		Mayatnik.gear1.rotate(+ float(360 / ( 2 * Mayatnik.gear_tooth1)));
		Mayatnik.gear2.rotate(- float(360 / (2 * Mayatnik.gear_tooth2)));
		Mayatnik.shark1.rotate(- float(360 / (2 * Mayatnik.shark_tooth)));
}

void speed(Mayatnik & Mayatnik, float boost, float & time) {
	Mayatnik.present_speed = Mayatnik.present_speed + boost * time;
}

void bob_step(Mayatnik & Mayatnik, float boost, bool isMove, Clock & clock, int & direction) {
	float time = float(clock.getElapsedTime().asMilliseconds());
	time = time / 1000;
	Mayatnik.present_degree = Mayatnik.present_degree + abs(time * Mayatnik.present_speed);
	if (Mayatnik.present_degree == Mayatnik.max_degree / 2) {
		boost = boost * (-1);
		clock.restart();
	}
	if (Mayatnik.present_degree >= Mayatnik.max_degree / 2) {
		boost = abs(boost) * (-1);
	}
	if (Mayatnik.present_degree >= Mayatnik.max_degree) {
		boost = boost * (-1);
		Mayatnik.present_degree = 0;
		mechanism_step(Mayatnik, boost, isMove);
		clock.restart();
		direction = direction * (-1);
		if (direction < 0) {
			Mayatnik.bob.setRotation(10);
			Mayatnik.rope.setRotation(10);
			Mayatnik.plumb.setRotation(10);
		}
		else {
			Mayatnik.bob.setRotation(-10);
			Mayatnik.rope.setRotation(-10);
			Mayatnik.plumb.setRotation(-10);
		}
	}


	speed(Mayatnik, boost, time);
	Mayatnik.bob.setRotation(Mayatnik.bob.getRotation() + direction * (time * Mayatnik.present_speed ));
	Mayatnik.rope.setRotation(Mayatnik.bob.getRotation() + direction * (time * Mayatnik.present_speed ));
	Mayatnik.plumb.setRotation(Mayatnik.bob.getRotation() + direction * (time * Mayatnik.present_speed ));

}


void Mayatnik_init(Mayatnik & Mayatnik) {
	Mayatnik.rope = RectangleShape(sf::Vector2f(10, 300));
	Mayatnik.rope.setOrigin(5, 0);
	Mayatnik.rope.setPosition(490, 110);
	Mayatnik.rope.setFillColor(Color::Yellow);
	Mayatnik.rope.setRotation(-10);

	Mayatnik.plumb = CircleShape(30);
	Mayatnik.plumb.setOrigin(30, -300);
	Mayatnik.plumb.setPosition(490, 100);
	Mayatnik.plumb.setFillColor(Color::Yellow);
	Mayatnik.plumb.setRotation(-10);

	Mayatnik.gear1 = gear(360, 293, Mayatnik.gear_tooth1, 100, 75);

	Mayatnik.shark1 = shark(490, 300, Mayatnik.shark_tooth, 100, 80);
	Mayatnik.shark1.setFillColor(Color::Red);

	Mayatnik.gear2 = gear(490, 300, Mayatnik.gear_tooth2, 40, 20);

	Mayatnik.bob = stick();
	Mayatnik.bob.setPosition(490, 110);
	Mayatnik.bob.setRotation(-10);
}

void draw_Mayatnik(Mayatnik & Mayatnik, RenderWindow & window) {
	window.clear();
	window.draw(Mayatnik.plumb);
	window.draw(Mayatnik.rope);
	window.draw(Mayatnik.shark1);
	window.draw(Mayatnik.bob);
	window.draw(Mayatnik.gear1);
	window.draw(Mayatnik.gear2);
	window.display();
}

void process(Mayatnik & Mayatnik, RenderWindow & window) {
	int direction = 1;
	float boost = float(0.00005);
	bool isMove = true;
	Clock clock;
	while (window.isOpen()) {
		bob_step(Mayatnik, boost, isMove, clock, direction);
		draw_Mayatnik(Mayatnik, window);
	}
}


int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(800, 600), "Mayatnik", Style::Default, settings);
	Mayatnik Mayatnik(45, 15, 15);
	Mayatnik_init(Mayatnik);
	process(Mayatnik, window);

    return 0;
}

