#pragma once

#include <queue>
#include <unordered_set>

#include "common/node.h"

class TClosed {
public:
    void Push(std::shared_ptr<TNode> node) {
        Used.insert(node->Hash());
    } 

    bool Contains(const std::shared_ptr<TNode>& node) const {
        return Used.find(node->Hash()) != Used.end();
    } 

    bool IsEmpty() const {
        return Used.empty();
    }

    int Size() const {
        return Used.size();
    }

private:
    std::unordered_set<ll> Used;
};
