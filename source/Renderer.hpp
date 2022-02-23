#ifndef PROJECT_RENDERER_HPP
#define PROJECT_RENDERER_HPP

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util/Shader.h"


class Renderer {
private:
    static void enableVA(unsigned numOfInputAttributes);
    static void disableVA(unsigned numOfInputAttributes);


    /**
     * Sends data to GPU.
     * @param points All data points organised AS SHOWN in 'positions'.
     * @param positions Shows how data points are organised by attribute.For example,
     *  (1,3) means first attribute has 1 dimension, second has 3.
     */
    static void setupData(const std::vector<GLfloat>& points, const std::vector<int>& positions);

    /**
     * Draws ONE instance of each of n='count' points.
     * It also enables/disables each attribute for that data set.
     * @param mvp
     * @param count
     * @param current_shader
     * @param numOfInputAttributes
     */
    static void draw(const glm::mat4& mvp,unsigned count,Shader& current_shader,unsigned numOfInputAttributes);

public:
    Renderer()= default;

    /**
     * Render points in this manner:
     * for EVERY mvp in MVPs, draw ALL points from 'data'
     * @param current_shader
     * @param positions Show how attributes are organised. For example,
     *  (1,3) means first attribute has 1 dimension, second has 3.
     * @param data data organised AS SHOWN in 'positions'
     * @param MVPs multiple MVP matrices,each represents instance of set of points
     */
    static void render(Shader& current_shader,
                       std::vector<int> positions,
                       const std::vector<GLfloat>& data,
                       std::vector<glm::mat4>& MVPs);
};


#endif //PROJECT_RENDERER_HPP
