#pragma once
#include "Vector2.h"
#include "Serialization/JSON.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
//#include <iostream>
namespace nu 
{
    class Transform
    {
    public:
        glm::vec3 position{ 0.0f };
        glm::vec3 rotation{ 0.0f };
        glm::vec3 scale{ 1.0f };


        void Read(const json::value_t& value)
        {
            //JSON_READ(value, position);
            //JSON_READ(value, rotation);
            //JSON_READ(value, scale);
        }
    };
}