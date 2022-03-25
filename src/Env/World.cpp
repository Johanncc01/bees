#include "World.hpp"
#include <Application.hpp>
#include <Utility/Vertex.hpp>


float World::getSize() const{
    return nb_cells * cell_size;
}


void World::reloadConfig(){
    nb_cells = getAppConfig().world_cells;
    cell_size = ((getAppConfig().world_size) / nb_cells);
    std::vector<Kind> new_grid((getAppConfig().world_size*cell_size), Kind::roche);
    cells_ = new_grid;
}

void World::reloadCacheStructure(){
        auto test(getValueConfig()["simulation"]["world"]["textures"]);            // A AJOUTER : raccourci dans config.cpp
        int size(nb_cells*cell_size);
        std::vector<sf::Vertex> vertexes(generateVertexes(getValueConfig()["simulation"]["world"]["textures"], nb_cells, cell_size));
        grassVertexes_ = vertexes;
        waterVertexes_ = vertexes;
        rockVertexes_ = vertexes;
        renderingCache_.create(size, size);
}

void World::drawOn(sf::RenderTarget& target){
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}

void World::updateCache(){
    renderingCache_.clear();
    sf::RenderStates rs;
    rs.texture = &getAppTexture(getAppConfig().rock_texture); // texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
    renderingCache_.display();
}

void World::reset(bool regenerate){
    reloadConfig();
    reloadCacheStructure();
    updateCache();
}























