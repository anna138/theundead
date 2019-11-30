#pragma once 


class TileParser{
    private:
        

    public:
        TileParser(const std::string);
        ~TileParser();
        void parseCSV(const std::string);
        void parseImageIndex(const std::string);
};
