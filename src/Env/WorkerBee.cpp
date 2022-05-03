#include <Env/WorkerBee.hpp>
#include <Application.hpp>

State const WorkerBee::IN_HIVE(createUid());

WorkerBee::WorkerBee(Hive& h, Vec2d const& pos)
    : Bee(h, pos, {IN_HIVE}, getAppConfig().worker_size, getAppConfig().worker_initial_energy, getAppConfig().worker_speed)
{

}


j::Value const& WorkerBee::getConfig() const{
    return getValueConfig()["simulation"]["bees"]["worker"];
}
