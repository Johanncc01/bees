#include <Env/CFSM.hpp>


// Constructeur

CFSM::CFSM(States s)
    : states(s), current(0)
{}


// Getter

State CFSM::getState() const{
    return states[current];
}


// Gestion des états

void CFSM::nextState(){
    ++current;
    // Pour que l'indice de l'état courrant reste cohérent
    current = current % states.size();
    onEnterState(states[current]);
}

void CFSM::action(sf::Time dt){
    onState(states[current], dt);
}
