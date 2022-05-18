#include <Stats/Stats.hpp>
#include <Application.hpp>
#include <algorithm>
#include <Utility/Utility.hpp>

Stats::Stats()
    : current(0)
{

}

void Stats::setActive(int id){
    current = id;
}

std::string Stats::getCurrentTitle() const{
    return strings[current];
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

void Stats::drawOn(sf::RenderTarget& target) const{
    // dessine uniquement le graph current
}

void Stats::update(sf::Time dt){

}

void Stats::reset() const{
    for (auto& graph : graphs){
        graph.second->reset();
    }
}

void Stats::addGraph(int id, std::string const& title, Strings const& series, double min, double max, Vec2d const& size){

    bool exists(false);
    while (!exists){
        try{
            strings.at(id) = title;
            exists = true;
        }
        catch (std::out_of_range){
            strings.push_back("");
        }
    }

    graphs[strings[id]].reset(new Graph(series, size, min, max));


    current = id;



}

