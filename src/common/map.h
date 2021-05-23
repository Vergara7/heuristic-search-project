#pragma once

#include <fstream>
#include <vector>

#include <iostream>
using std::cout, std::endl;

using uint = unsigned int;
using ll = long long;

class TMap{
public:
    TMap(std::string file_path);
    bool CellIsClear(int x, int y) const;
    bool PathIsClear(int x1, int y1, int x2, int y2) const;

private:
    uint Height, Width;
    std::vector<std::vector<bool>> MapGrid;

    bool HorisontalEdgeIsClear(int x, int y) const;
    bool VerticalEdgeIsClear(int x, int y) const;
};

TMap::TMap(std::string file_path) {
    std::ifstream in(file_path);
    std::string s;
    getline(in, s);
    assert(s == "type octile");
    in >> s;
    assert(s == "height");
    in >> Height;
    in >> s;
    assert(s == "width");
    in >> Width;
    in >> s;
    assert(s == "map");

    MapGrid.resize(Height);
    for (int i = 0; i < Height; i++)
        MapGrid[i].resize(Width);
    for (int i = 0; i < Height; i++){
        in >> s;
        assert(s.size() == Width);
        for (int j = 0; j < Width; j++){
            if (s[j] != '.' && s[j] != 'G' && s[j] != 'S')
                MapGrid[i][j] = true;
        }
    }
}

bool TMap::CellIsClear(int x, int y) const {
    if (!(x >= 0 && x <= Height && y >= 0 && y <= Width))
        return false;
    if (x > 0 && y > 0 && !MapGrid[x - 1][y - 1])
        return true;
    if (x > 0 && y < Width && !MapGrid[x - 1][y])
        return true;
    if (x < Height && y > 0 && !MapGrid[x][y - 1])
        return true;
    if (x < Height && y < Width && !MapGrid[x][y])
        return true;
    return false;
}

bool TMap::HorisontalEdgeIsClear(int x, int y) const {
    if (x > 0 && !MapGrid[x - 1][y])
        return true;
    return x < Height && !MapGrid[x][y];
}

bool TMap::VerticalEdgeIsClear(int x, int y) const {
    if (y > 0 && !MapGrid[x][y - 1])
        return true;
    return y < Width && !MapGrid[x][y];
}

bool TMap::PathIsClear(int x1, int y1, int x2, int y2) const {
    if (!CellIsClear(x1, y1) || !CellIsClear(x2, y2))
        return false;
    if (x1 > x2){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    if (x1 == x2 && y1 > y2)
        std::swap(y1, y2);
    if (x1 == x2){
        for (int y = y1; y < y2; y++){
            if (!HorisontalEdgeIsClear(x1, y))
                return false;
        }
        return true;
    }
    if (y1 == y2){
        for (int x = x1; x < x2; x++){
            if (!VerticalEdgeIsClear(x, y1))
                return false;
        }
        return true;
    }
    int dx = x2 - x1, dy = y2 - y1;
    auto vectProduct = [](int x1, int y1, int x2, int y2){ return (ll)x1 * y2 - x2 * y1;};
    if (y1 < y2){
        int y = y1;
        for (int x = x1; x < x2; x++){
            while (vectProduct(dx, dy, x + 1 - x1, y + 1 - y1) > 0){
                if (MapGrid[x][y])
                    return false;
                y++;
            }  
            if (MapGrid[x][y])
                return false;
        }
    } else {
        int y = y1 - 1;
        for (int x = x1; x < x2; x++){
            while (vectProduct(x + 1 - x, y - y1, dx, dy) > 0){
                if (MapGrid[x][y])
                    return false;
                y--;
            }
            if (MapGrid[x][y])
                return false;
        }
    }
    return true;
}
