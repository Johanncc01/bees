#include <Env/CFSM.hpp>

CFSM::CFSM(States s)
    : states(s)
{
    current = 0;
}


State CFSM::getState() const{
    return states[current];
}

void CFSM::nextState(){
    ++current;
    if (current == states.size()){
        current = 0;
    }

    onEnterState(states[current]);
}

void CFSM::onEnterState(State s){

}

void CFSM::onState(State s, sf::Time dt){

}

void CFSM::action(sf::Time dt){
    onState(states[current], dt);
}
