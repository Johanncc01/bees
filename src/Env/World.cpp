#include "World.hpp"
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <iostream>
#include <fstream>


float World::getSize() const{
    return nb_cells * cell_size;
}


void World::reloadConfig(){
    nb_cells = getAppConfig().world_cells;
    cell_size = ((getAppConfig().world_size) / nb_cells);
    std::vector<Kind> new_grid((nb_cells*cell_size), Kind::roche);
    cells_ = new_grid;
}

void World::reloadCacheStructure(){
        std::vector<sf::Vertex> vertexes(generateVertexes(getAppConfig().world_textures, nb_cells, cell_size));
        grassVertexes_ = vertexes;
        waterVertexes_ = vertexes;
        rockVertexes_ = vertexes;
        renderingCache_.create(nb_cells*cell_size, nb_cells*cell_size);
}

void World::drawOn(sf::RenderTarget& target){
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}

void World::updateCache(){
    renderingCache_.clear();

    for (int x = 0; x < nb_cells; ++x) {
         for (int y = 0; y < nb_cells; ++y) {
             std::vector<size_t> indexes(indexesForCellVertexes(x,y, nb_cells));
             int i(x+y*nb_cells);
             for (auto var : indexes){
                 if (cells_[i] == Kind::herbe){
                     grassVertexes_[var].color.a = 255;
                     waterVertexes_[var].color.a = 0;
                     rockVertexes_[var].color.a = 0;
                 } else if (cells_[i] == Kind::eau){
                     grassVertexes_[var].color.a = 0;
                     waterVertexes_[var].color.a = 255;
                     rockVertexes_[var].color.a = 0;
                 } else {
                     grassVertexes_[var].color.a = 0;
                     waterVertexes_[var].color.a = 0;
                     rockVertexes_[var].color.a = 255;
                 }
             }
         }
    }
    sf::RenderStates rs_g;
    rs_g.texture = &getAppTexture(getAppConfig().grass_texture); // texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs_g);

    sf::RenderStates rs_w;
    rs_w.texture = &getAppTexture(getAppConfig().water_texture); // texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs_w);

    sf::RenderStates rs_r;
    rs_r.texture = &getAppTexture(getAppConfig().rock_texture); // texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs_r);

    renderingCache_.display();
}

void World::reset(bool regenerate){
    reloadConfig();
    reloadCacheStructure();
    updateCache();
}

void World::loadFromFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ifstream entree(path);
    if (entree.fail()){
        entree.clear();
        std::string error("impossible de lire le fichier :");
        throw std::runtime_error(error+path);
    } else {
        std::cout << "Fichier chargé : " << path << std::endl;
        int world_size;
        entree >> world_size >> std::ws >> cell_size >> std::ws;
        nb_cells = world_size/cell_size;
        std::vector<Kind> new_grid(nb_cells*cell_size);
        short i(0);
        for (size_t j(0); j < new_grid.size(); ++j){
            entree >> i;
            new_grid[j] = static_cast<Kind>(i);
        }
        cells_ = new_grid;
        entree.close();
    }
    reloadCacheStructure();
    updateCache();
}





















