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
                int ty,int tz, int mID);
        TypeObj(int idx, int x, int y, int x1, int y1,
                int t0, int t1,int t2, int t3, int mID);
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
    public:
        Blender();
        void readObj(const std::string);
        void readMaterial(const std::string);
        int loadTexture(std::string);
        void renderObj(float, float, float);
};
