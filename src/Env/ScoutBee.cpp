#include <Env/ScoutBee.hpp>
#include <Application.hpp>
#include <Utility/Utility.hpp>
// Inclusion à cause de la prédéclaration dans la superclasse Bee.hpp
#include <Env/WorkerBee.hpp>


State const ScoutBee::IN_HIVE(createUid());
State const ScoutBee::LF_FLOWER(createUid());
State const ScoutBee::BACK_TO_HIVE(createUid());

ScoutBee::ScoutBee(Hive& h, Vec2d const& pos)
    : Bee(h, pos, {IN_HIVE, LF_FLOWER, BACK_TO_HIVE},  getAppConfig().scout_size, getAppConfig().scout_initial_energy, getAppConfig().scout_speed)
      , shareCounter(0)
{}

j::Value const& ScoutBee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["scout"];
}

void ScoutBee::onState(State state, sf::Time dt){

    if (state == IN_HIVE){
        if (energy < getAppConfig().scout_energy_to_leave_hive){
            isEating = true;
            energy += hive.takePollen(dt.asSeconds()*getAppConfig().scout_eating_rate);
        } else {
            isEating = false;
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


void ScoutBee::drawOn(sf::RenderTarget& target) const {

    Bee::drawOn(target);

    if (isDebugOn()){
        auto const text = buildText("Scout: energy "+ to_nice_string(energy), {center.x(), center.y() + 15}, getAppFont(), 10, sf::Color::Black);
        target.draw(text);
        if (getState() == LF_FLOWER){
            auto const text = buildText("seeking_flower", {center.x(), center.y() + 25}, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (getState() == BACK_TO_HIVE){
            auto const text = buildText("back_to_hive", {center.x(), center.y() + 25}, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (isEating){
            auto const text = buildText("in_hive_eating", {center.x(), center.y() + 25}, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if ((memory == nullptr) or shareCounter == getAppConfig().scout_max_sharing){
            auto const text = buildText("in_hive_leaving", {center.x(), center.y() + 25}, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (shareCounter < getAppConfig().scout_max_sharing){
            auto const text = buildText("in_hive_sharing ["+ to_nice_string(shareCounter) + "]", {center.x(), center.y() + 25}, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
    }

}


// Polymorphisme

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




