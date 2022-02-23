#include "Slither.hpp"
#include "FloatComparisons.hpp"

Slither::Slither(float x, float y, float diff):diff(diff){
    //push in snake HEAD
    //don't use head DIRECTION
    snake.push_back(Chunk(glm::vec2(x, y), glm::vec2(0, 0)));
}

unsigned Slither::getLength() const {
    return snake.size();
}

glm::vec3 Slither::head() {
    return {snake.front().pos.x,snake.front().pos.y,0};
}

float Slither::getDirection() {
    return glm::radians(dir);
}

void Slither::changeDirection(GLfloat fi) {
//        if(fi > 360) fi -=360;
//        if(fi < 0) fi+=360;

    if (dir > fi) dir--;
    else if (dir == fi) ;
    else dir ++;
//
//        if(dir > 360) dir -=360;
//        if(dir < 0) dir+=360;

}

void Slither::add() {
    GLfloat rads = glm::radians(dir);

    Chunk& last = snake.back();
    Chunk chunk(last.pos + glm::vec2(diff, 0), glm::vec2(last.pos));

    snake.push_back(chunk);
}

void Slither::hop(float step_n) {
    //step_n = diff;

    Chunk &head = snake.front(); //never use head.dir
    GLfloat rads = glm::radians(dir);  //direction of new step

    head.pos = head.pos + glm::vec2(glm::cos(rads),glm::sin(rads))*step_n;

//        Chunk newChunk(glm::cos(rads),glm::sin(rads));
//        newChunk.pos =newChunk.pos*0.003f + head.pos;
    //float ampl = glm::length(newChunk.pos-head.pos);
    Chunk lastChunck = head;

    bool first = true;
    for (auto & i : snake)
    {
        if (first) { first = false; continue; }

        if(i.pos.x != i.pos.x){
            std::cout << this->toString();
            throw std::exception();
        }


        float m = i.getMaxDistance();

        //if next safe is safe to do (step is smaller than distance to chunks destination)
        if (definitelyGreaterThan(m,step_n,1e-5)){
            i.pos += i.unitStep()*step_n;
        }

            //chunks destination is less than current step
            //chunk has to travel = to destination + some fraction left from "step length n"
        else if(definitelyGreaterThan(step_n,m,1e-5)){
            i.pos = i.dir;
            i.dir = lastChunck.pos;
            //if distance to next destination is smaller than it should be
            if(definitelyGreaterThan( diff,i.getMaxDistance(),1e-5) ){
                // i.pos += -i.unitStep()*abs(diff-i.getMaxDistance());
                //i.dir += i.unitStep()*abs(diff-i.getMaxDistance());

            }

            i.pos += i.unitStep()*(step_n-m);
        }

        lastChunck=i;
    }

//        for (auto & i : snake){
//                std::swap(i,newChunk);
//        }


}

bool Slither::cutTail() {
    if (snake.size() <=1 ) return false;
    snake.pop_back();
    return true;
}

std::vector<GLfloat> Slither::getCoords() {
    GLfloat i = 1;
    std::vector<GLfloat> T;
    for(const auto& c: snake){
        T.push_back(c.pos.x);
        T.push_back(c.pos.y);
        T.push_back(0);
        T.push_back(radius);
        T.push_back(i);
        i++;
    }
    return T;
}

std::string Slither::toString() {
    std::stringstream s;
    s << "**Snake**[";
    for (auto &chunk :snake)
        s << chunk.toString()<<",";
    s << "]\n";
    s<< "Front: " << snake.front().toString() << "\n";
    s<< "Back: " << snake.back().toString() << "\n";
    return s.str();
}
