#include <Env/ScoutBee.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
// Inclusion à cause de la prédéclaration dans la superclasse Bee.hpp
#include <Env/WorkerBee.hpp>


// Initialisation des attributs statiques (différents états)

State const ScoutBee::IN_HIVE(createUid());
State const ScoutBee::LF_FLOWER(createUid());
State const ScoutBee::BACK_TO_HIVE(createUid());


// Constructeur et destructeur

ScoutBee::ScoutBee(Hive& h, Vec2d const& pos)
    : Bee(h, pos, {IN_HIVE, LF_FLOWER, BACK_TO_HIVE},  getAppConfig().scout_size, getAppConfig().scout_initial_energy, getAppConfig().scout_speed)
      , shareCounter(0)
{
    hive.changeScoutNumber(true);
}

ScoutBee::~ScoutBee(){
    hive.changeScoutNumber(false);
}


// Getter

j::Value const& ScoutBee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["scout"];
}


// Dessin d'une éclaireuse

void ScoutBee::drawOn(sf::RenderTarget& target) const {

    Bee::drawOn(target);

    Vec2d renderPos1(center.x(), center.y() + getAppConfig().scout_size*1.2);           // Pour afficher le type de l'abeille et son énergie
    Vec2d renderPos2(renderPos1.x(), renderPos1.y() + 10);                              // Pour afficher l'état actuel de l'éclaireuse

    if (isDebugOn()){
        auto const text = buildText("Scout: energy "+ to_nice_string(energy), renderPos1, getAppFont(), 10, sf::Color::Black);
        target.draw(text);

        if (getState() == LF_FLOWER){
            auto const text = buildText("seeking_flower", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (getState() == BACK_TO_HIVE){
            auto const text = buildText("back_to_hive", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (energy < getAppConfig().scout_energy_to_leave_hive){
            auto const text = buildText("in_hive_eating", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if ((memory == nullptr) or shareCounter == getAppConfig().scout_max_sharing){
            auto const text = buildText("in_hive_leaving", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (shareCounter < getAppConfig().scout_max_sharing){
            auto const text = buildText("in_hive_sharing ["+ to_nice_string(shareCounter) + "]", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
    }

}


// Gestion des états

void ScoutBee::onState(State state, sf::Time dt){

    if (state == IN_HIVE){
        if (energy < getAppConfig().scout_energy_to_leave_hive){
            energy += hive.takePollen(dt.asSeconds()*getAppConfig().scout_eating_rate);
        } else {
            if ((memory == nullptr) or (shareCounter >= 1)){
                nextState();
            }
        }

    } else if (state == LF_FLOWER){

        memory = getAppEnv().getCollidingFlowerPosition(Collider(center, radius + getConfig()["visibility range"].toDouble()));
        bool seenFlowers(memory != nullptr);

        if (seenFlowers or (energy < getAppConfig().scout_energy_seek_flowers)){
            nextState();
        }

    } else if (state == BACK_TO_HIVE){
        if (hive>*this){
            nextState();
        }
    }
}

void ScoutBee::onEnterState(State state){
    if (state == IN_HIVE){
        mode = Mode::repos;
        delete target;
        target = nullptr;

    } else if (state == LF_FLOWER){
        mode = Mode::random;
        delete memory;
        memory = nullptr;
        shareCounter = 0;

    } else if (state == BACK_TO_HIVE){
        mode = Mode::target;
        target = new Vec2d(hive.getPosition());
    }
}


// Interactions

void ScoutBee::interact(Bee* other){
    other->interactWith(this);
}

void ScoutBee::interactWith(ScoutBee*){
    return;
}

void ScoutBee::interactWith(WorkerBee* worker){
    if ((memory != nullptr) and (shareCounter < getAppConfig().scout_max_sharing)){
        ++shareCounter;
        worker->learnFlowerLocation(*memory);
    }
}




