#pragma once

#include <queue>
#include <unordered_set>

#include "node.h"

class TOpen {
public:
    void Push(std::shared_ptr<TNode> node) {
        Used.insert(node->Hash());
        Q.push(std::move(node));
    } 

    bool IsInside(const std::shared_ptr<TNode>& node) const {
        return Used.find(node->Hash()) != Used.end();
    } 

    bool IsEmpty() const {
        return Q.empty();
    }

private:
    std::priority_queue<std::shared_ptr<TNode>, std::vector<std::shared_ptr<TNode>>, TNodeGreaterComp> Q;
    std::unordered_set<ll> Used;
};
