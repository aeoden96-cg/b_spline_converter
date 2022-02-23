#ifndef PROJECT_SLITHER_HPP
#define PROJECT_SLITHER_HPP

#include <deque>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <sstream>


class Slither{
private:

    struct Chunk{
        glm::vec2 pos;
        glm::vec2 dir;

        Chunk(glm::vec2 position , glm::vec2 direction): pos(position), dir(direction){}
        float getMaxDistance() const{return glm::length(dir-pos);}
        glm::vec2 unitStep() const{  return glm::vec2(dir-pos)/getMaxDistance();}

        std::string toString() const{
            std::stringstream s;
            s << "(" << pos.x << ", " << pos.y << ")";
            return s.str();
        }

    };

    float radius =  1/16.0;
    GLfloat dir = 0;
    const float diff;

public:
    std::deque<Chunk> snake;
    Slither(float x,float y,float diff);
    unsigned getLength() const;
    glm::vec3 head();
    float getDirection();
    void changeDirection(GLfloat fi);
    void add();
    void hop(float step_n=0.003f);
    bool cutTail();
    std::vector<GLfloat> getCoords();
    std::string toString();
};

#endif //PROJECT_SLITHER_HPP