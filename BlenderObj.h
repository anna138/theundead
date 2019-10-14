#pragma once
#include <iostream>
#include <vector>
class Vect{
    public:
        float x, y, z;
        Vect(float a, float b, float c): x(a),y(b), z(c){};
};
class TypeObj{
    public:
        int id;
        int size;
        int * cord;
        TypeObj(int idx, int x, int y, int z){
            id = idx;
            size = 3;
            cord = new int[size];
            cord[0] = x;
            cord[1] = y;
            cord[2] = z;
        }
        TypeObj(int idx, int x, int y, int x1, int y1){
            id = idx;
            size = 4;
            cord = new int[size];
            cord[0] = x;
            cord[1] = y;
            cord[2] = x1;
            cord[3] = y1;

        }

};

class Blender{
    private:
        float ang;
    public:
        Blender();
        void readObj(const std::string, float, float, float);

};
