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
        int matID;
        int size;
        int * cord;
        int * textureCoord;
        TypeObj(int idx, int x, int y, int z,int tx, 
                int ty,int tz, int mID){
            id = idx;
            matID = mID;
            size = 3;
            cord = new int[size];
            textureCoord = new int[size];
            cord[0] = x;
            cord[1] = y;
            cord[2] = z;
            textureCoord[0] = tx;
            textureCoord[1] = ty;
            textureCoord[2] = tz;
        }
        TypeObj(int idx, int x, int y, int x1, int y1,
                int t0, int t1,int t2, int t3, int mID){
            id = idx;
            matID = mID;
            size = 4;
            cord = new int[size];
            textureCoord = new int[size];
            cord[0] = x;
            cord[1] = y;
            cord[2] = x1;
            cord[3] = y1;
            textureCoord[0] = t0;
            textureCoord[1] = t1;
            textureCoord[2] = t2;
            textureCoord[3] = t3;
        }

};
class Material{

    public:
        std::string matname;
        std::vector<float> spec;
        std::vector<float> diffuse;
        std::vector<float> amb;
        int illum;
        float ns, ni, d;
        int textureID;
        Material(std::string, std::vector<float> &, std::vector<float>&, 
                std::vector<float>&, float, float, float, int, int); 
};



class Blender{
    private:
        std::vector<Vect*> v;
        std::vector<Vect*> vn;
        std::vector<TypeObj*> faces;
        std::vector<Vect*> vt;
        std::vector<Material*> mats;
        float ang;
    public:
        Blender();
        void readObj(const std::string);
        void readMaterial(const std::string);
        int loadTexture(std::string);
        void renderObj(float, float, float);
};
