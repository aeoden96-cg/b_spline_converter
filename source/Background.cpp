//
// Created by mateo on 12. 12. 2021..
//

#include "Background.hpp"

Background::Background():offsets{-1,0,1}{}

std::vector<GLfloat> Background::update(glm::vec3 position) {
    points.clear();
    position.x = glm::round(position.x);
    position.y = glm::round(position.y);

    for(float i: offsets){
        for(float j: offsets){
            points.push_back(position.x+i);
            points.push_back(position.y+j);
            points.push_back(position.z);
        }
    }
    return points;
}