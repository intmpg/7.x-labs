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
	Vector2f(0, 0);
	Vector2f(x, y);
	float grad = float(360 / (tooth * 3));
	int point_num = tooth * 3;
	std::cout << "tooth = " << tooth << std::endl;
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


struct Pendulum {
	Pendulum(int value1, int value2, int value3):
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

void mechanism_step(Pendulum & Pendulum, float & boost, bool & isMove) {

		Pendulum.gear1.rotate(+ float(360 / ( 2 * Pendulum.gear_tooth1)));
		Pendulum.gear2.rotate(- float(360 / (2 * Pendulum.gear_tooth2)));
		Pendulum.shark1.rotate(- float(360 / (2 * Pendulum.shark_tooth)));
}

void speed(Pendulum & Pendulum, float boost, float & time) {
	Pendulum.present_speed = Pendulum.present_speed + boost * time;
}

void bob_step(Pendulum & Pendulum, float boost, bool isMove, Clock & clock, int & direction) {
	float time = float(clock.getElapsedTime().asMilliseconds());
	time = time / 1000;
	Pendulum.present_degree = Pendulum.present_degree + abs(time * Pendulum.present_speed);
	if (Pendulum.present_degree == Pendulum.max_degree / 2) {
		boost = boost * (-1);
		clock.restart();
	}
	if (Pendulum.present_degree >= Pendulum.max_degree / 2) {
		boost = abs(boost) * (-1);
	}
	if (Pendulum.present_degree >= Pendulum.max_degree) {
		boost = boost * (-1);
		Pendulum.present_degree = 0;
		mechanism_step(Pendulum, boost, isMove);
		clock.restart();
		direction = direction * (-1);
		if (direction < 0) {
			Pendulum.bob.setRotation(10);
			Pendulum.rope.setRotation(10);
			Pendulum.plumb.setRotation(10);
		}
		else {
			Pendulum.bob.setRotation(-10);
			Pendulum.rope.setRotation(-10);
			Pendulum.plumb.setRotation(-10);
		}
	}


	speed(Pendulum, boost, time);
	Pendulum.bob.setRotation(Pendulum.bob.getRotation() + direction * (time * Pendulum.present_speed ));
	Pendulum.rope.setRotation(Pendulum.bob.getRotation() + direction * (time * Pendulum.present_speed ));
	Pendulum.plumb.setRotation(Pendulum.bob.getRotation() + direction * (time * Pendulum.present_speed ));

}


void Pendulum_init(Pendulum & Pendulum) {
	Pendulum.rope = RectangleShape(sf::Vector2f(10, 300));
	Pendulum.rope.setOrigin(5, 0);
	Pendulum.rope.setPosition(490, 110);
	Pendulum.rope.setFillColor(Color::Yellow);
	Pendulum.rope.setRotation(-10);

	Pendulum.plumb = CircleShape(30);
	Pendulum.plumb.setOrigin(30, -300);
	Pendulum.plumb.setPosition(490, 100);
	Pendulum.plumb.setFillColor(Color::Yellow);
	Pendulum.plumb.setRotation(-10);

	Pendulum.gear1 = gear(360, 293, Pendulum.gear_tooth1, 100, 75);

	Pendulum.shark1 = shark(490, 300, Pendulum.shark_tooth, 100, 80);
	Pendulum.shark1.setFillColor(Color::Red);

	Pendulum.gear2 = gear(490, 300, Pendulum.gear_tooth2, 40, 20);

	Pendulum.bob = stick();
	Pendulum.bob.setPosition(490, 110);
	Pendulum.bob.setRotation(-10);
}

void draw_Pendulum(Pendulum & Pendulum, RenderWindow & window) {
	window.clear();
	window.draw(Pendulum.plumb);
	window.draw(Pendulum.rope);
	window.draw(Pendulum.shark1);
	window.draw(Pendulum.bob);
	window.draw(Pendulum.gear1);
	window.draw(Pendulum.gear2);
	window.display();
}

void process(Pendulum & Pendulum, RenderWindow & window) {
	int direction = 1;
	float boost = 0.00005f;
	bool isMove = true;
	Clock clock;
	while (window.isOpen()) {
		bob_step(Pendulum, boost, isMove, clock, direction);
		draw_Pendulum(Pendulum, window);
	}
}


int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(800, 600), "Pendulum", Style::Default, settings);
	Pendulum Pendulum(45, 15, 15);
	Pendulum_init(Pendulum);
	process(Pendulum, window);

    return 0;
}

