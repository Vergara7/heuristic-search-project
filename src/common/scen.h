#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <fstream>

typedef long double dbl;

struct TScen{
    int xst, yst, xfin, yfin;
    int Level;
    dbl Answer;
    std::string MapName;
};

std::vector<TScen> LoadScens(std::string file_path){
    std::ifstream in(file_path);
    std::string s;
    getline(in, s);
    assert(s == "version 1");
    std::vector<TScen> scens;
    while (getline(in, s)){
        TScen scen;
        std::stringstream ss(s);
        int h, w;
        ss >> scen.Level >> scen.MapName;
        ss >> h >> w >> scen.xst >> scen.yst >> scen.xfin >> scen.yfin >> scen.Answer;
        if (!scens.empty())
            assert(scen.MapName == scens[0].MapName);
        scens.push_back(scen);
    }
    return scens;
}
