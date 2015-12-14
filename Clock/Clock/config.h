#pragma once

const int RADIUS = 200;
const int START_X = 200;
const int START_Y = 200;
const int AMOUNT_POINTS = 60;
const int SCALE = 180;

sf::Vector2f WINDOW_SIZE = { 400, 400 };

struct Shapes {
	sf::RectangleShape background;
	sf::ConvexShape hourHand;
	sf::ConvexShape minuteHand;
	sf::ConvexShape secondHand;
	sf::CircleShape point;
};