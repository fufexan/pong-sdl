#pragma once

#include <SDL2/SDL.h>

class Paddle {
public:
	enum class Type {LEFT, RIGHT};
	enum class Direction {NONE, UP, DOWN};
	Paddle(Type type, int x, int y);
	~Paddle() = default;
	
	void handle_input(SDL_Event const &event);
	void physics(double delta_time);
	void draw(SDL_Renderer *renderer);

private:
	double m_y;
	Type m_type;
	SDL_Rect m_position;
	Direction m_direction;
};