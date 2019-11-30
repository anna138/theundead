#include <fstream>
#include "TileParser.h"
#include <iostream>
#include <sstream>
#include <vector>
TileParser::TileParser(const std::string fn)
{
    parseCSV(fn);
}

TileParser::~TileParser()
{



}
void TileParser::parseCSV(const std::string fn)
{
    std::vector<int> imgid;
    std::ifstream in;
    in.open(fn.c_str());
    int width = 0;
    int height = 0;
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)) {
            if (line.find("<layer") != std::string::npos) {
                
                int pos = line.find_last_of("width=");
                std::string temp(line.begin()+pos+2, line.begin()+pos+4);
                width = stoi(temp);
                pos = line.find_last_of("height=");
                std::string temp2(line.begin()+pos+2, line.begin()+pos+4);
                height = stoi(temp2);
                std::cout << line << std::endl;                
                pos = line.find_last_of("name=");
                std::string temp3(line.begin()+pos+1, line.end()-1);
                std::cout << pos << std::endl;
                pos = temp3.find_first_of('"');
                std::string imgsrc(temp3.begin(), temp3.begin()+pos);
                std::cout << imgsrc << std::endl;

            }else if (line.find("<data") != std::string::npos) {
                std::string word;
                for (int i = 0; i < width; i++) {
                    std::getline(in,line);
                    std::stringstream l(line);
                    while (getline(l, word, ',')) {
                        
                    }
                }
            }

        }

    }
}

void TileParser::parseImageIndex(const std::string fn) 
{
    


}
