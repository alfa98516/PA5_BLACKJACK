#pragma once
#include "Hand.hpp"

class Dealer {
    std::shared_ptr<Hand> hand;
    bool stand;

  public:
    void Action();
};
