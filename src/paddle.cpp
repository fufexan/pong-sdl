#include "paddle.hpp"

Paddle::Paddle(int x, int y)                     
{                                                
    m_position.x = x;                            
    m_position.y = y;                            
    m_position.w = 50;                           
    m_position.h = 100;                          
}                                                

void Paddle::handle_input(SDL_Event const &event)
{                                                

}                                                

void Paddle::update(double delta_time)           
{                                                

}                                                

void Paddle::draw()                              
{                                                

}           