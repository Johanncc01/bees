#pragma once
#include <Env/Bee.hpp>


class WorkerBee : public Bee
{
public:

    WorkerBee(Hive&, Vec2d const&);

    /*!
     * @brief Destructeur d'une butineuse (par défaut)
     */
    ~WorkerBee() = default;


    /*!
     * @brief Getter de la configuration en fonction de la sous-classe d'abeille
     *
     * @return un tableau contenant les valeurs coresspondantes au bon type dans le .json
     */
    j::Value const& getConfig() const override;

    void onState(State, sf::Time) override;

    void onEnterState(State) override;

    void drawOn(sf::RenderTarget&) const override;


// Polymorphisme
    void interact(Bee*) override;

    void interactWith(ScoutBee*) override;

    void interactWith(WorkerBee*) override;



private:

    static State const IN_HIVE;
    static State const TO_FLOWER;
    static State const COLLECT_POLLEN;
    static State const BACK_TO_HIVE;

    double pollenQty;

};

