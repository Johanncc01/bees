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
    getHive().changeScoutNumber(true);
}

ScoutBee::~ScoutBee(){
    getHive().changeScoutNumber(false);
}


// Getter

j::Value const& ScoutBee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["scout"];
}

bool ScoutBee::isStateHive() const{
    return getState() == IN_HIVE;
}


// Dessin d'une éclaireuse

void ScoutBee::drawOn(sf::RenderTarget& target) const {

    Bee::drawOn(target);

    // Pour afficher le type de l'abeille et son énergie
    Vec2d renderPos1(getPosition().x(), getPosition().y() + getAppConfig().scout_size*1.2);
    // Pour afficher l'état actuel de l'éclaireuse
    Vec2d renderPos2(renderPos1.x(), renderPos1.y() + 10);

    if (isDebugOn()){
        auto const text = buildText("Scout: energy "+ to_nice_string(getEnergy()), renderPos1, getAppFont(), 10, sf::Color::Black);
        target.draw(text);

        if (getState() == LF_FLOWER){
            auto const text = buildText("seeking_flower", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (getState() == BACK_TO_HIVE){
            auto const text = buildText("back_to_hive", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (getEnergy() < getAppConfig().scout_energy_to_leave_hive){
            auto const text = buildText("in_hive_eating", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if ((getMemory() == nullptr) or shareCounter == getAppConfig().scout_max_sharing){
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
        if (getEnergy() < getAppConfig().scout_energy_to_leave_hive){
            setEnergy(getEnergy() + getHive().takePollen(dt.asSeconds()*getAppConfig().scout_eating_rate));
        } else {
            if ((getMemory() == nullptr) or (shareCounter >= 1)){
                nextState();
            }
        }

    } else if (state == LF_FLOWER){

        setMemory(getAppEnv().getCollidingFlowerPosition(Collider(getPosition(), getRadius() + getConfig()["visibility range"].toDouble())));
        bool seenFlowers(getMemory() != nullptr);

        if (seenFlowers or (getEnergy() < getAppConfig().scout_energy_seek_flowers)){
            nextState();
        }

    } else if (state == BACK_TO_HIVE){
        if (getHive()>*this){
            nextState();
        }
    }
}

void ScoutBee::onEnterState(State state){
    if (state == IN_HIVE){
        setMode(Mode::repos);
        delete getTarget();
        setTarget(nullptr);

    } else if (state == LF_FLOWER){
        setMode(Mode::random);
        delete getMemory();
        setMemory(nullptr);
        shareCounter = 0;

    } else if (state == BACK_TO_HIVE){
        setMode(Mode::target);
        setTarget(new Vec2d(getHive().getPosition()));
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
    if ((getMemory() != nullptr) and (shareCounter < getAppConfig().scout_max_sharing)){
        ++shareCounter;
        worker->learnFlowerLocation(*getMemory());
    }
}
