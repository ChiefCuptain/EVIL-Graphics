#pragma once
#include "Vector2.h"
#include "Serialization/JSON.h"
//#include <iostream>
namespace nu 
{
    class Transform
    {
    public:
        Vector2 position{ 0.0f };
        float rotation = 0.0f;
        float scale = 1.0f;


        void Read(const json::value_t& value)
        {
            //std::cout << "position : " << position.x << ", " << position.y << " -> ";
            JSON_READ(value, position);
            //std::cout << position.x << ", " << position.y << "\n";
            //std::cout << "rotation : " << rotation << " -> ";
            JSON_READ(value, rotation);
            //std::cout << rotation << "\n";
            //std::cout << "scale : " << scale << " -> ";
            JSON_READ(value, scale);
            //std::cout << scale << "\n";
        }
    };
}