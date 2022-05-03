#pragma once
#include <Env/Bee.hpp>


class WorkerBee : public Bee
{
public:

    WorkerBee(Hive&, Vec2d const&);


    /*!
     * @brief Getter de la configuration en fonction de la sous-classe d'abeille
     *
     * @return un tableau contenant les valeurs coresspondantes au bon type dans le .json
     */
    j::Value const& getConfig() const override;


private:

    static State const IN_HIVE;



};

