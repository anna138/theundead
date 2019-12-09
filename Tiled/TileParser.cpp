#include <fstream>
#include "TileParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "pugixml.hpp"
#include "../Texture.h"

TileParser::TileParser(const std::string fn)
{

    parseXML(fn);
    //posx = ((width/2)-1)+.4;
    posx = width-1+.5;
    posy = posx;
    int size = tiles.size();
    if(true) {
        for(int loop = size-2; loop >= 0; loop--){
            for(int i = 0; i < tiles.at(size-1).size(); i++){
                for(int j = 0; j < tiles.at(size-1).at(i).size(); j++){
                    if(tiles.at(size-1).at(i).at(j) == 0){
                        tiles.at(size-1).at(i).at(j) = tiles.at(loop).at(i).at(j);
                    }
                }
            }
        }

    }
}
void TileParser::reMap(const std::string fn){
    tiles.clear();
    parseXML(fn);
    posx = width-1+.5;
    posy = posx;
    int size = tiles.size();
    if(true) {
        for(int loop = size-2; loop >= 0; loop--){
            for(int i = 0; i < tiles.at(size-1).size(); i++){
                for(int j = 0; j < tiles.at(size-1).at(i).size(); j++){
                    if(tiles.at(size-1).at(i).at(j) == 0){
                        tiles.at(size-1).at(i).at(j) = tiles.at(loop).at(i).at(j);
                    }
                }
            }
        }

    }
}

TileParser::~TileParser()
{
    tiles.clear();
    
}
void TileParser::parseXML(const std::string fn)
{
    doc.load_file(fn.c_str());
    for (auto &l1:doc.children()) {
        for (auto &l2 : l1.children()) {
            std::string attri = l2.name();
            if(attri == "layer"){
                width = l2.attribute("width").as_int();
                height = l2.attribute("height").as_int();
                parseCSV(l2);
                
            }
        }
    }

}
void TileParser::parseCSV(pugi::xml_node & layer){

    for (auto& csv : layer.children()) {
        std::string attri = csv.name();
        if (attri == "data") {
            std::string encoder = csv.attribute("encoding").as_string();
            if (encoder == "csv") { 
                std::string data = csv.text().as_string();
                std::stringstream line(data);
                std::string word;
                std::vector<std::vector<uint32_t>> t;
                for (unsigned int i = 0; i < width; i++) {
                    std::vector<uint32_t> temp;
                    for (unsigned int j = 0; j < height; j++ ){
                        getline(line, word, ',');
                        temp.push_back(stoi(word));
                    }
                    t.push_back(temp);
                }
                tiles.push_back(t);
            }
        }
    }
}
double TileParser::dot(double *a, double *b)
{
    //std::cout << "dot:" <<  (a[0]*b[0])+(a[1]*b[1]) << std::endl;
    return (a[0]*b[0])+(a[1]*b[1]);
}
double TileParser::distance(double *a, double *b)
{
    return sqrt(pow((b[0]-a[0]), 2)+pow((b[1]-a[1]), 2));
}

bool TileParser::isWalkable(int x, int y, int dir)
{
    //x+=32;
    //y+=64;
    // unsigned int arrcoordx = -(y/(height))+(height>>1)-(x/width);
    // unsigned int arrcoordy = ((x/width)+(width>>1)-(y/(height)));
    unsigned int px; 
    unsigned int py;
    if(dir == 0){
        px = -(y/(height>>1))+((height>>1)-1)-(x/(width));//-(y/(height))+(height>>1)-(x/width);
        py = (x/(width))+((width>>1)-1)-(y/(height>>1));//((x/width)+(width>>1)-(y/(height)));
    }else{
        px = +((height>>1)-1  );
        py = -(x/(width>>1))+((width>>1)-1);
    }


    return true;
}

bool TileParser::Walk(float x, float y, int & code, int &face){

    int size = tiles.size();
    posx += x;
    posy += y;
    if(posx >= width || posy >= height || posx <= 0 || posy <= 0){
        posx -= x;
        posy -= y;
        return false;
    }


    if(map == 0){
        bool set = false;
        if(tiles.at(size-1).at((int)posx).at((int)posy) >= 153  || tiles.at(size-1).at((int)posx).at((int)posy) == 10){
            posx -= x;
            posy -= y;
            
            return false;
        }
        if((posx+2 <= width && posy+2 <= height) && (posx-2 >= 0 && posy-2 >= 0)){
            for(int i = posy+1; i < posy+2; i++){
                if(tiles.at(size-1).at((int)posx).at((int)i) >= 153  || 
                    tiles.at(size-1).at((int)posx).at((int)i) == 10){
                        face = 0;
                        set = true;
                }
            }
            for(int i = posy; !set && i >= posy-2; i--){
                if(tiles.at(size-1).at((int)posx).at((int)i) >= 153  || 
                    tiles.at(size-1).at((int)posx).at((int)i) == 10){
                        face = 1;
                        set = true;
                }

            }
            for(int i = posx; !set && i < posx+2; i++){
                if(tiles.at(size-1).at((int)i).at((int)posy) >= 153  || 
                    tiles.at(size-1).at((int)i).at((int)posy) == 10){
                        face = 0;
                        set = true;
                }
            }
            for(int i = posx; !set && i >= posx-2; i--){
                if(tiles.at(size-1).at((int)i).at((int)posy) >= 153  || 
                    tiles.at(size-1).at((int)i).at((int)posy) == 10){
                        face = 1;
                        set = true;
                }
            }
        }
        if(tiles.at(size-1).at((int)posx).at((int)posy) >= 153  || tiles.at(size-1).at((int)posx).at((int)posy) == 10){
            posx -= x;
            posy -= y;
            //code = tiles.at(2).at((int)posx).at((int)posy);
            return false;
        }else{
            code = tiles.at(size-1).at((int)posx).at((int)posy);
            return true;
        }
    }else{
                bool set = false;
        if(tiles.at(size-1).at((int)posx).at((int)posy) >= 123 && 
                tiles.at(size-1).at((int)posx).at((int)posy) < 380 || 
                tiles.at(size-1).at((int)posx).at((int)posy) == 10){
            posx -= x;
            posy -= y;
            
            return false;
        }
        if((posx+2 <= width && posy+2 <= height) && (posx-2 >= 0 && posy-2 >= 0)){
            for(int i = posy+1; i < posy+2; i++){
                if(tiles.at(size-1).at((int)posx).at((int)i) >= 123 && 
                    tiles.at(size-1).at((int)posx).at((int)posy) < 380|| 
                    tiles.at(size-1).at((int)posx).at((int)i) == 10){
                        face = 0;
                        set = true;
                }
            }
            for(int i = posy; !set && i >= posy-2; i--){
                if(tiles.at(size-1).at((int)posx).at((int)i) >= 123  && 
                    tiles.at(size-1).at((int)posx).at((int)posy) < 380|| 
                    tiles.at(size-1).at((int)posx).at((int)i) == 10){
                        face = 1;
                        set = true;
                }

            }
            for(int i = posx; !set && i < posx+2; i++){
                if(tiles.at(size-1).at((int)i).at((int)posy) >= 123  && 
                    tiles.at(size-1).at((int)posx).at((int)posy) < 380|| 
                    tiles.at(size-1).at((int)i).at((int)posy) == 10){
                        face = 0;
                        set = true;
                }
            }
            for(int i = posx; !set && i >= posx-2; i--){
                if(tiles.at(size-1).at((int)i).at((int)posy) >= 123  && 
                    tiles.at(size-1).at((int)posx).at((int)posy) < 380|| 
                    tiles.at(size-1).at((int)i).at((int)posy) == 10){
                        face = 1;
                        set = true;
                }
            }
        }
        if(tiles.at(size-1).at((int)posx).at((int)posy) > 123 && 
                tiles.at(size-1).at((int)posx).at((int)posy) < 380){
            posx -= x;
            posy -= y;
            return false;
        }else{
            
            code = tiles.at(size-1).at((int)posx).at((int)posy);
            if(code == 55 || code == 39){
                face = 1;
            }
            return true;
        }

    }


    
}
void TileParser::reset()
{
    posx = ((width)-1)+.5;
    posy = posx;
}






