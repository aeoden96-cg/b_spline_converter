#ifndef PROJECT_BACKGROUND_HPP
#define PROJECT_BACKGROUND_HPP

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Background {
public:
    std::vector<GLfloat> points;
    float offsets[3];

    Background();
    std::vector<GLfloat> update(glm::vec3 position);
};


#endif //PROJECT_BACKGROUND_HPP
