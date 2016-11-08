#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED
#include "vec3f.h"
#include "modelloader.h"
class Block{
    private :
        char type;
        Vec3f pos;
        Vec3f angle;
        Model_OBJ model;
    public:
        Block();
        Block(char type,Vec3f pos,Vec3f angle,Model_OBJ model);
        ~Block();

        void addPos(Vec3f pos);
        void addAngle(char axis,float angle);

        char getType();
        float getPos(char axis);
        Vec3f getAngle();
        float getAngle(char axis);
        Model_OBJ getModel();

        void setType(char type);
        void setPos(Vec3f pos);
        void setAngle(Vec3f angle);
        void setAngle(char axis,float angle);
        void setModel(Model_OBJ model);

};

#endif
