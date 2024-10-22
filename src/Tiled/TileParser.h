#pragma once 
#include "pugixml.hpp"
#include <vector>

class TileParser{
    private:
        pugi::xml_document doc;
        std::vector<std::vector<std::vector<uint32_t>>> tiles;
        int width;
        int height;
        float posx;
        float posy;
        
        
    public:
        int map = 0;
        TileParser(const std::string);
        ~TileParser();
        void reset();
        void parseCSV(pugi::xml_node&);
        void parseXML(const std::string);
        bool isWalkable(int x, int y, int dir);
        bool Walk(float, float, int&, int&);
        double distance(double *a, double *b);
        double dot(double *a, double *b);
        void reMap(const std::string);

};
