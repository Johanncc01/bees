#include <Stats/Stats.hpp>
#include <Application.hpp>


// Constructeur

Stats::Stats()
    : current(0), compteur(sf::Time::Zero)
{}


// Getters et setters

void Stats::setActive(int id){
    current = id;
}

std::string Stats::getCurrentTitle() const{
    return strings.at(current);
}

void Stats::next(){
    ++current;
    current = current % graphs.size();
}

void Stats::previous(){
    --current;
    if (current < 0){
        current = graphs.size()-1;
    }
}


// Méthodes pures

void Stats::drawOn(sf::RenderTarget& target) const{
    graphs.at(current)->drawOn(target);
}

void Stats::update(sf::Time dt){
    compteur += dt;
    if (compteur > sf::seconds(getAppConfig().stats_refresh_rate)){
        std::unordered_map<std::string,double> new_data(getAppEnv().fetchData(strings[current]));
        graphs[current]->updateData(compteur, new_data);
        compteur = sf::Time::Zero;
    }
}


// Gestion des statistiques

void Stats::reset(){
    compteur = sf::Time::Zero;
    for (auto& graph : graphs){
        graph.second->reset();
    }
}

void Stats::addGraph(int id, std::string const& title, std::vector<std::string> const& series, double min, double max, Vec2d const& size){
    graphs[id].reset(new Graph(series, size, min, max));
    strings[id] = title;
    current = id;
}

