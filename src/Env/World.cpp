#include "World.hpp"
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <Random/Random.hpp>
#include <iostream>
#include <fstream>


float World::getSize() const{
    return nb_cells * cell_size;
}


void World::reloadConfig(){
    int world_size(getAppConfig().world_size);
    nb_cells = getAppConfig().world_cells;
    cell_size = (world_size / nb_cells);
    std::vector<Kind> grid((nb_cells*nb_cells), Kind::roche);
    cells_ = grid;
    nb_wseed = getAppConfig().world_nb_water_seeds;
    nb_gseed = getAppConfig().world_nb_grass_seeds;
    std::vector<Seed> seeds_init(nb_wseed + nb_gseed);
    seeds_=seeds_init;

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
    for (int x(0); x < nb_cells; ++x) {
         for (int y(0); y < nb_cells; ++y) {
             std::vector<size_t> indexes(indexesForCellVertexes(x,y, nb_cells));
             int i(x+y*nb_cells);
             if (cells_[i] == Kind::herbe){
                 for (auto var : indexes){
                     grassVertexes_[var].color.a = 255;
                     waterVertexes_[var].color.a = 0;
                     rockVertexes_[var].color.a = 0;
                 }
             } else if (cells_[i] == Kind::eau){
                 for (auto var : indexes){
                     grassVertexes_[var].color.a = 0;
                     waterVertexes_[var].color.a = 255;
                     rockVertexes_[var].color.a = 0;
                 }
             } else {
                 for (auto var : indexes){
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
    rs_r.texture = &getAppTexture(getAppConfig().rock_texture); // textugre liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs_r);

    renderingCache_.display();
}

void World::reset(bool regenerate){
    reloadConfig();
    reloadCacheStructure();
    int temp_wseed(nb_wseed);
    for (auto& id : seeds_) {
        int x(uniform(0 , nb_cells-1));
        int y(uniform(0 , nb_cells-1));
        sf::Vector2i temp(x,y);
        id.coords = temp;
        if (temp_wseed > 0) {
            --temp_wseed;
            id.type = Kind::eau;
        } else {
            id.type = Kind::herbe;
        }

        int i(x+y*nb_cells);

        if (cells_[i] != Kind::eau) {
            cells_[i] = id.type;
        }


    }


    updateCache();

}

void World::loadFromFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ifstream entree(path);
    if (entree.fail()){
        entree.close();
        std::string error("impossible de lire le fichier ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Fichier chargé : " << path << std::endl;
        entree >> nb_cells >> std::ws >> cell_size >> std::ws;
        std::vector<Kind> grid(nb_cells*nb_cells);
        short lu(0);
        for (size_t i(0); i < grid.size(); ++i){
            entree >> lu;
            grid[i] = static_cast<Kind>(lu);
        }
        cells_ = grid;
        entree.close();
    }
    reloadCacheStructure();
    updateCache();
}





















