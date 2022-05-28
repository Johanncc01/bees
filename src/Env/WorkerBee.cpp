#include <Env/WorkerBee.hpp>
#include <Application.hpp>


// Initialisation des attributs statiques (différents états)

State const WorkerBee::IN_HIVE(createUid());
State const WorkerBee::TO_FLOWER(createUid());
State const WorkerBee::COLLECT_POLLEN(createUid());
State const WorkerBee::BACK_TO_HIVE(createUid());


// Constructeur et destructeur

WorkerBee::WorkerBee(Hive& h, Vec2d const& pos)
    : Bee(h, pos, {IN_HIVE, TO_FLOWER, COLLECT_POLLEN, BACK_TO_HIVE}, getAppConfig().worker_size, getAppConfig().worker_initial_energy, getAppConfig().worker_speed), pollenQty(0)
{
    getHive().changeWorkerNumber(true);
}

WorkerBee::~WorkerBee(){
    getHive().changeWorkerNumber(false);
}


// Getter

j::Value const& WorkerBee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["worker"];
}

bool WorkerBee::isStateHive() const{
    return getState() == IN_HIVE;
}


// Dessin d'une butineuse

void WorkerBee::drawOn(sf::RenderTarget& target) const {

    Bee::drawOn(target);

    // Pour afficher le type de l'abeille et son énergie
    Vec2d renderPos1(getPosition().x(), getPosition().y() + getAppConfig().worker_size*1.2);
    // Pour afficher l'état actuel de la butineuse
    Vec2d renderPos2(renderPos1.x(), renderPos1.y() + 10);
    // Pour afficher la quantité de pollen de la butineuse
    Vec2d renderPos3(renderPos1.x(), renderPos1.y() + 20);

    if (isDebugOn()){
        auto const text = buildText("Worker: energy "+ to_nice_string(getEnergy()), renderPos1, getAppFont(), 10, sf::Color::Black);
        target.draw(text);

        auto const text2 = buildText("pollenQty : "+ to_nice_string(pollenQty), renderPos3, getAppFont(), 10, sf::Color::Black);
        target.draw(text2);


        if (getState() == TO_FLOWER){
            auto const text = buildText("to_flower", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else if (getState() == COLLECT_POLLEN){
            auto const text = buildText("collecting_pollen", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
        else if (getState() == BACK_TO_HIVE){
            auto const text = buildText("back_to_hive", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
        else if (pollenQty > 0){
            auto const text = buildText("in_hive_pollen", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
        else if (getEnergy() < getAppConfig().worker_energy_to_leave_hive){
            auto const text = buildText("in_hive_eating", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
        else if (getMemory() == nullptr){
            auto const text = buildText("in_hive_no_flower", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        } else {
            auto const text = buildText("in_hive_???", renderPos2, getAppFont(), 10, sf::Color::Black);
            target.draw(text);
        }
    }
}


// Gestion des états

void WorkerBee::onState(State state, sf::Time dt){

    if (state == IN_HIVE){

        if (pollenQty > 0){
            double computed(dt.asSeconds()*getAppConfig().worker_transfer_rate);
            if (pollenQty > computed){
                pollenQty -= getHive().dropPollen(computed);
            } else {
                getHive().dropPollen(pollenQty);
                pollenQty = 0;
            }
        }

        if (pollenQty == 0){
            if (getEnergy() < getAppConfig().worker_energy_to_leave_hive){
                setEnergy(getEnergy() + getHive().takePollen(dt.asSeconds()*getAppConfig().worker_eating_rate));
            } else {
                if (getMemory() != nullptr){
                    nextState();
                }
            }
        }

    } else if (state == TO_FLOWER){

        if (*this>Collider(*getTarget(), getAppConfig().flower_manual_size)){
            if (getAppEnv().getCollidingFlower(*this) == nullptr) {
                nextState();
            }
            nextState();
        }

    } else if (state == COLLECT_POLLEN){

        Flower* eating(getAppEnv().getCollidingFlower(*this));
        if (eating == nullptr) {
            nextState();
        } else {
            double max(getAppConfig().worker_pollen_capacity);
            double computed(dt.asSeconds()*getAppConfig().worker_harvest_rate);
            double left(eating->takePollen(computed));

            if ((pollenQty + computed) < max){
                pollenQty += eating->takePollen(computed);
            } else {
                pollenQty += eating->takePollen(max-pollenQty);
                nextState();
            }

            if (computed != left){
                nextState();
            }
        }

    } else if (state == BACK_TO_HIVE){
        if (getHive()>*this){
            nextState();
        }
    }
}

void WorkerBee::onEnterState(State state){
    if (state == IN_HIVE){
        setMode(Mode::repos);
        delete getTarget();
        setTarget(nullptr);

    } else if (state == TO_FLOWER){
        setMode(Mode::target);
        setTarget(new Vec2d(*getMemory()));
        delete getMemory();
        setMemory(nullptr);

    } else if (state == COLLECT_POLLEN){
        setMode(Mode::repos);
        delete getTarget();
        setTarget(nullptr);

    } else if (state == BACK_TO_HIVE){
        setMode(Mode::target);
        setTarget(new Vec2d(getHive().getPosition()));
    }
}


// Interactions

void WorkerBee::interact(Bee* other){
    other->interactWith(this);
}

void WorkerBee::interactWith(ScoutBee*){
    return;
}

void WorkerBee::interactWith(WorkerBee*){
    return;
}

