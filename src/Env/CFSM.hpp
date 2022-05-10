#pragma once
#include <Utility/Utility.hpp>
#include <vector>

typedef Uid State;
typedef std::vector<State> States;

class CFSM
{
public:
    CFSM(States);

    State getState() const;
    void nextState();

    virtual void onState(State, sf::Time) = 0;
    virtual void onEnterState(State) = 0;
    void action(sf::Time);



private:

    States states;
    size_t current;

};

