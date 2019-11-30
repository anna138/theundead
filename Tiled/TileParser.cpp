#include <fstream>
#include "TileParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "pugixml.hpp"

//using namespace pugi;


TileParser::TileParser(const std::string fn)
{
    parseXML(fn);
}

TileParser::~TileParser()
{

}
void TileParser::parseXML(const std::string fn)
{
    // std::vector<std::vector<int>> floorid;
    // std::vector<std::vector<int>> propsid;
    // std::string floorname;
    // std::string propsname;
    // std::ifstream in;
    // int id = 0;
    // in.open(fn.c_str());
    // int width = 0;
    // int height = 0;
    // if (in.is_open()) {
    //     std::string line;
    //     while (std::getline(in, line)) {
    //         if (line.find("<layer") != std::string::npos) {
                
    //             int pos = line.find("width=")+5;
    //             std::string temp(line.begin()+pos+2, line.begin()+pos+4);
    //             width = stoi(temp);
    //             pos = line.rfind("height=")+6;
    //             std::string temp2(line.begin()+pos+2, line.begin()+pos+4);
    //             height = stoi(temp2);
    //             pos = line.find("name=")+4;
    //             std::string temp3(line.begin()+pos+2, line.end()-1);
    //             pos = temp3.find('"');
    //             std::string imgsrc(temp3.begin(), temp3.begin()+pos);
    //             if(id == 0){
    //                 floorname = imgsrc;
    //             } else if (id == 1) {
    //                 propsname = imgsrc;
    //             }
    //         } else if (line.find("<data") != std::string::npos) {
    //             std::string word;
    //             for (int i = 0; i < width; i++) {
    //                 std::getline(in,line);
    //                 std::stringstream l(line);
    //                 std::vector<int> row;
    //                 while (getline(l, word, ',')) {
    //                     row.push_back(stoi(word));
    //                 }
    //                 if (id == 0) {
    //                     floorid.push_back(row);
    //                 }
    //                 else if(id == 1) {
    //                     propsid.push_back(row);
    //                 }
    //             }
    //             id++;
    //         }

    //     }
    //     parseImageIndex(floorname);
    //}
    doc.load_file(fn.c_str());
    for (auto &l1:doc.children()) {
        for (auto &l2 : l1.children()) {
            if(l2.name() == "layer")
                parseCSV(l2);
        }
    }

}
void TileParser::parseCSV(pugi::xml_node & layer){

    for (auto& csv : layer.children()) {
        if (csv.name() == "data") {
            if (csv.attribute("encoding").name() == "csv") { 
                std::string data = csv.text().as_string();
                std::stringstream line(data);
                std::string word;
                while (getline(line, word, ',')) {
                    row.push_back(stoi(word));
                }
                if (id == 0) {
                    floorid.push_back(row);
                }
                else if(id == 1) {
                    propsid.push_back(row);
                }


            }

        }
    }

}

void TileParser::parseImageIndex(const std::string fn) 
{
    //fn = fn + ".tsx";
    std::ifstream in(fn);

    if (in.is_open()) {
    

    }


}
