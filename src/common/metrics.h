#pragma once

#include <cmath>
#include <algorithm>

#include "node.h"

dbl TrivialHeuristic(const TNode& lhs, const TNode& rhs) {
    return 0.;
}

dbl EuclidianDistance(const TNode& lhs, const TNode& rhs) {
    int dx = lhs.x - rhs.x;
    int dy = lhs.y - rhs.y;
    return sqrt(dx * dx + dy * dy);
}

dbl ManhattanDistance(const TNode& lhs, const TNode& rhs) {
    return abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y);
}

dbl H2_3(const TNode& lhs, const TNode& rhs) {
    int dx = abs(lhs.x - rhs.x);
    int dy = abs(lhs.y - rhs.y);
    return sqrt(2) * std::min(dx, dy) + abs(dx - dy);
}

dbl H2_4(const TNode& lhs, const TNode& rhs) {
    int dx = abs(lhs.x - rhs.x);
    int dy = abs(lhs.y - rhs.y);
    if (dx > dy)
        std::swap(dx, dy);
    if (2 * dx < dy)
        return (dy - 2 * dx) + sqrt(5) * dx;
    return sqrt(5) * (dy - dx) + sqrt(2) * (2 * dx - dy);
}


dbl H2_5(const TNode& lhs, const TNode& rhs) {
    int dx = abs(lhs.x - rhs.x);
    int dy = abs(lhs.y - rhs.y);
    if (dx > dy)
        std::swap(dx, dy);
    if (3 * dx < dy)
        return (dy - 3 * dx) + sqrt(10) * dx;
    else if (2 * dx < dy)
        return sqrt(10) * (dy - 2 * dx) + sqrt(5) * (3 * dx - dy);
    else if (3 * dx < 2 * dy)
        return sqrt(5) * (2 * dy - 3 * dx) + sqrt(13) * (2 * dx - dy);
    return sqrt(13) * (dy - dx) + sqrt(2) * (3 * dx - 2 * dy);
}


dbl H2_6(const TNode& lhs, const TNode& rhs) {
    int dx = abs(lhs.x - rhs.x);
    int dy = abs(lhs.y - rhs.y);
    if (dx > dy)
        std::swap(dx, dy);
    if (4 * dx < dy)
        return (dy - 4 * dx) + sqrt(17) * dx;
    else if (3 * dx < dy)
        return sqrt(17) * (dy - 3 * dx) + sqrt(10) * (4 * dx - dy);
    else if (5 * dx < 2 * dy)
        return sqrt(10) * (2 * dy - 5 * dx) + sqrt(29) * (3 * dx - dy);
    else if (2 * dx < dy)
        return sqrt(29) * (dy - 2 * dx) + sqrt(5) * (5 * dx - 2 * dy);
    else if (5 * dx < 3 * dy)
        return sqrt(5) * (3 * dy - 5 * dx) + sqrt(34) * (2 * dx - dy);
    else if (3 * dx < 2 * dy)
        return sqrt(34) * (2 * dy - 3 * dx) + sqrt(13) * (5 * dx - 3 * dy);
    else if (4 * dx < 3 * dy)
        return sqrt(13) * (3 * dy - 4 * dx) + 5 * (3 * dx - 2 * dy);
    return 5 * (dy - dx) + sqrt(2) * (4 * dx - 3 * dy);
}


dbl ChebyshevDistance(const TNode& lhs, const TNode& rhs) {
    return std::max(abs(lhs.x - rhs.x), abs(lhs.y - rhs.y));
}
