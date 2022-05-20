#pragma once
#include <map>

struct Hexagon {
    int64_t X;
    int64_t Y;
    int64_t Z;
    std::string Type;
    std::map<std::string, std::string> GlobalData;
    std::map<std::string, std::string> LocalData;

    Hexagon(int x_, int y_, int z_, std::string type_, std::map<std::string, std::string> global_data_,std::map<std::string, std::string> local_data_)
            :X(x_), Y(y_), Z(z_), Type(type_), GlobalData(global_data_), LocalData(local_data_) {};

    Hexagon(int x_, int y_, int z_, std::string type_)
            :X(x_), Y(y_), Z(z_), Type(type_) {};
};
