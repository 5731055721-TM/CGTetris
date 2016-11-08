#include <assert.h>
#include <fstream>

#include "block.h"
#include "vec3f.h"
#include "modelloader.h"

using namespace std;

Block::Block() {}
Block::Block(char t,Vec3f p,Vec3f a,Model_OBJ m) : type(t),pos(p),angle(a),model(m) {}
Block::~Block() {
	model.Model_OBJ::Release();
}

void Block::addAngle(char axis,float a) {
    if(axis=='x') angle[0]+=a;
    else if(axis=='y') angle[1]+=a;
    else angle[2]+=a;
}
void Block::addPos(Vec3f p) {
    pos[0]+=p[0];
    pos[1]+=p[1];
    pos[2]+=p[2];
}

void Block::setType(char t) {
    type = t;
}
void Block::setPos(Vec3f p) {
    pos = p;
}
void Block::setAngle(Vec3f a) {
    angle = a;
}
void Block::setAngle(char axis,float a){
    if(axis=='x') angle[0]=a;
    else if(axis=='y') angle[1]=a;
    else angle[2]=a;
}
void Block::setModel(Model_OBJ m) {
    model=m;
}

char Block::getType() {
    return type;
}
float Block::getPos(char a) {
    return (a=='x') ? pos[0] : (a=='y') ? pos[1] : pos[2];
}
float Block::getAngle(char a) {
    return (a=='x') ? angle[0] : (a=='y') ? angle[1] : angle[2];
}
Model_OBJ Block::getModel() {
    return model;
}

