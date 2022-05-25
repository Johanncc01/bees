#pragma once
#include <Utility/Utility.hpp>
#include <vector>


// typedef des types complexes
typedef Uid State;
typedef std::vector<State> States;

class CFSM
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'un CFSM (Circular Finite State Machine)
     *
     * @param l'ensemble des états (Uid) possibles
     */
    CFSM(States);

    /*!
     * @brief Destructeur d'un CFSM (par défaut)
     *        Déclaré virtual pour que la destruction d'une sous-classe se fasse complètement
     */
    virtual ~CFSM() = default;

// Getter

    /*!
     * @brief Getter de l'état actuel
     *
     * @return l'état (Uid) correspondant à l'état en cours
     */
    State getState() const;


protected:
// Gestion des états

    /*!
     * @brief Transitionne vers l'état suivant, en réalisant les traitements nécessaires (via onEnterState())
     */
    void nextState();

    /*!
     * @brief Fait évoluer l'automate dans son état actuel sur un temps donné
     *
     * @param temps dt sur lequel l'évolution est effectuée
     */
    void action(sf::Time);


private:

    States states;
    size_t current;

// Gestion des états

    /*!
     * @brief Fait évoluer l'automate dans un état donné sur un temps donné
     *
     * @param état (Uid) correspondant aux traitements voulus
     * @param temps dt sur lequel l'évolution est effectuée
     */
    virtual void onState(State, sf::Time) = 0;

    /*!
     * @brief Réalise les actions lors d'un changement vers un nouvel état donné
     *
     * @param état (Uid) d'entrée de l'automate
     */
    virtual void onEnterState(State) = 0;

};

