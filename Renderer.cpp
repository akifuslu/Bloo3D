#include "Renderer.h"

#include<iostream>

void AssignRGBA(unsigned char* pixel, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    pixel[3] = a;
}

void Renderer::Render(unsigned char* buffer, int width, int height, int frame) 
{
    size_t index = 0;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            AssignRGBA(buffer + index, frame % 256, 150, 0, 255);
            index += 4;
        }        
    }
       
}
