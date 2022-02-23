#include <numeric>
#include "Renderer.hpp"

void Renderer::enableVA(unsigned int numOfInputAttributes) {
    for (int i=0;i<numOfInputAttributes ; i++)
        glEnableVertexAttribArray(i);
}

void Renderer::disableVA(unsigned int numOfInputAttributes) {
    for (int i=0;i<numOfInputAttributes ; i++)
        glDisableVertexAttribArray(i);
}

void Renderer::draw(
        const glm::mat4 &mvp,
        unsigned int count,
        Shader &current_shader,
        unsigned int numOfInputAttributes) {
    enableVA(numOfInputAttributes);

    current_shader.setMat4("MVP" ,&mvp[0][0]);

    if(!current_shader.isUsingTess())
        glDrawArrays(GL_LINE_STRIP,0,(int)count);
    else{
        glPatchParameteri(GL_PATCH_VERTICES,1);
        glDrawArrays(GL_PATCHES,0,(int)count);
    }

    disableVA(numOfInputAttributes);
}

void Renderer::setupData(
        const std::vector<GLfloat> &points,
        const std::vector<int> &positions) {

    unsigned short W = std::accumulate(positions.begin(), positions.end(), 0);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (unsigned)(sizeof(GLfloat) * points.size()), &points[0], GL_DYNAMIC_DRAW);
    int i=0;
    unsigned short formerAttribs=0;
    for (auto& currentAttrib : positions)
    {
        glVertexAttribPointer(
                i,                                              //i.th attribute
                currentAttrib,                                  //num of dimensions for ith attrib
                GL_FLOAT,
                GL_FALSE,
                (int)(sizeof(GLfloat)*W),                            //width of one row
                (const GLvoid *)(sizeof(GLfloat) * formerAttribs)   //offset
        );
        formerAttribs+=currentAttrib;
        i++;
    }
}

void Renderer::render(
        Shader &current_shader,
        std::vector<int> positions,
        const std::vector<GLfloat> &data,
        std::vector<glm::mat4> &MVPs) {

    current_shader.use();

    Renderer::setupData(data,positions);
    int sum = std::accumulate(positions.begin(), positions.end(), 0);
    for (const auto& mvp: MVPs){
        Renderer::draw(mvp,
                       data.size()/sum,   // = num of points to be rendered
                       current_shader,
                       positions.size());
    }


}