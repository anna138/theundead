#pragma once 
#include "pugixml.hpp"

class TileParser{
    private:
        pugi::xml_document doc;
        std::vector<int> tiles;
        int width;
        int height;
    public:
        TileParser(const std::string);
        ~TileParser();
        void parseCSV(pugi::xml_node&);
        void parseImageIndex(const std::string);
        void parseXML(const std::string)
};
