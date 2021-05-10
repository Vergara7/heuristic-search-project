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

dbl DiagonalDistance(const TNode& lhs, const TNode& rhs) {
    int dx = abs(lhs.x - rhs.x);
    int dy = abs(lhs.y - rhs.y);
    return sqrt(2) * std::min(dx, dy) + abs(dx - dy);
}

dbl ChebyshevDistance(const TNode& lhs, const TNode& rhs) {
    return std::max(abs(lhs.x - rhs.x), abs(lhs.y - rhs.y));
}
