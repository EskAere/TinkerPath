#pragma once
#include <sstream>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
	class Sprite;
	class Text;
}

class Animation;

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

// Random number generation
int				randomInt(int exclusiveMax);
int				randomRangeInt(int min, int max);

// Vector operations
float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);

template <typename T>
std::string toString(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}