#include "World.hpp"
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <Random/Random.hpp>
#include <iostream>
#include <fstream>


// Getters

float World::getSize() const{
    return nb_cells * cell_size;
}

int World::get_id(int x, int y){
    return x + y*nb_cells;
}

int World::get_x(int id){
    return id % nb_cells;
}

int World::get_y(int id){
    return id / nb_cells;
}


// Fonctions graphiques

void World::reloadConfig(){
    size_t world_size(getAppConfig().world_size);
    nb_cells = getAppConfig().world_cells;
    cell_size = (world_size / nb_cells);
    Grille grid((nb_cells*nb_cells), Kind::roche);
    cells_ = grid;
    nb_wseed = getAppConfig().world_nb_water_seeds;
    nb_gseed = getAppConfig().world_nb_grass_seeds;
    Seeds seeds_init(nb_wseed + nb_gseed);
    seeds_=seeds_init;

}

void World::reloadCacheStructure(){
        Vertexes vertexes(generateVertexes(getAppConfig().world_textures, nb_cells, cell_size));
        grassVertexes_ = vertexes;
        waterVertexes_ = vertexes;
        rockVertexes_ = vertexes;
        renderingCache_.create(nb_cells*cell_size, nb_cells*cell_size);
}

void World::updateCache(){
    renderingCache_.clear();
    for (int x(0); x < nb_cells; ++x) {
         for (int y(0); y < nb_cells; ++y) {
             std::vector<size_t> indexes(indexesForCellVertexes(x,y, nb_cells));
             if (cells_[get_id(x,y)] == Kind::herbe){
                 for (auto var : indexes){
                     grassVertexes_[var].color.a = 255;
                     waterVertexes_[var].color.a = 0;
                     rockVertexes_[var].color.a = 0;
                 }
             } else if (cells_[get_id(x,y)] == Kind::eau){
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
    size_t temp_wseed(nb_wseed);
    for (auto& seed : seeds_) {
        int cells(nb_cells);
        size_t x(uniform(0 , cells-1));
        size_t y(uniform(0 , cells-1));
        size_t id(get_id(x,y));
        sf::Vector2i temp(x,y);
        seed.coords = temp;

        if (temp_wseed > 0) {
            --temp_wseed;
            seed.type = Kind::eau;
        } else {
            seed.type = Kind::herbe;
        }
        if (cells_[id] != Kind::eau) {
            cells_[id] = seed.type;
        }
    }
    if (regenerate){
        steps(getAppConfig().world_generation_steps);
        smooths(getAppConfig().world_generation_smoothness_level);
    }
    updateCache();
}

void World::drawOn(sf::RenderTarget& target){
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}


// Génération aléatoire

void World::step(){
    for (auto& seed : seeds_){
        if (seed.type == Kind::herbe){
            sf::Vector2i temp(randomN());
            seed.coords += temp;
            clamp(seed.coords);
        } else {
            if (bernoulli(getAppConfig().water_seeds_teleport_proba) == 0){
                sf::Vector2i temp(randomN());
                seed.coords += temp;
                clamp(seed.coords);
            } else {
                int x(uniform(0 , nb_cells-1));
                int y(uniform(0 , nb_cells-1));
                sf::Vector2i temp(x,y);
                seed.coords = temp;
            }
        }
        size_t i(seed.coords.x+seed.coords.y*nb_cells);
        if (cells_[i] != Kind::eau) {
            cells_[i] = seed.type;
        }
    }
}


void World::steps(int nb, bool regenerate){
    for (int i(0); i < nb ; ++i){
        step();
    }
    if (regenerate){
        updateCache();
    }
}

void World::smooth(){
    auto copie_de_cells_ = cells_;
    double seuil_w(getAppConfig().smoothness_water_neighbor_ratio);
    double seuil_g(getAppConfig().smoothness_grass_neighbor_ratio);
    for (size_t id(0); id < copie_de_cells_.size(); ++id){
        int x(get_x(id));
        int y(get_y(id));
        double neighbour_counter(0);
        if (copie_de_cells_[id] != Kind::eau){
            double water_counter(0);

           for (int a(-1); a <= 1 ; ++a){
               for (int b(-1); b <= 1 ; ++b){
                   sf::Vector2i index(x+a,y+b);
                   sf::Vector2i copie(index);
                   clamp(index);
                   if (index == copie){
                       ++neighbour_counter;
                       if (copie_de_cells_[get_id(x+a, y+b)] == Kind::eau){
                           ++water_counter;
                       }
                   }
               }
           }
           --neighbour_counter;
           if (water_counter/neighbour_counter > seuil_w){
               copie_de_cells_[id] = Kind::eau;
           }
        }
        if (copie_de_cells_[id] == Kind::roche){
            double grass_counter(0);

           for (int a(-1); a <= 1 ; ++a){
               for (int b(-1); b <= 1 ; ++b){
                   sf::Vector2i index(x+a,y+b);
                   sf::Vector2i copie(index);
                   clamp(index);
                   if (index == copie){
                       if (copie_de_cells_[get_id(x+a, y+b)] == Kind::herbe){
                           ++grass_counter;
                       }
                   }
               }
           }
           if (grass_counter/neighbour_counter > seuil_g){
               copie_de_cells_[id] = Kind::herbe;
           }
        }
    }
    std::swap(cells_, copie_de_cells_); // quand le lissage est fini on copie copie_de_cells_ dans cell_ (le swap est une optimisation).
}

void World::smooths(int nb, bool regenerate){
    for (int i(0); i < nb ; ++i){
        smooth();
    }
    if (regenerate){
        updateCache();
    }
}


// Sauvegarde

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
        Grille grid(nb_cells*nb_cells);
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

void World::saveToFile(){
    std::string path(getApp().getResPath()+getAppConfig().world_init_file);
    std::ofstream sortie(path);
    if (sortie.fail()){
        sortie.close();
        std::string error("impossible d'écrire le fichier sous ");
        throw std::runtime_error(error+path);
        return;
    } else {
        std::cout << "Ecriture dans : " << path << std::endl;
        sortie << nb_cells << std::endl << cell_size << std::endl;
        for (size_t i(0); i < cells_.size(); ++i){
            short lu(static_cast<short>(cells_[i]));
            sortie << lu << " ";
        }
        std::cout << "Opération terminée!" << std::endl;
        sortie.close();
    }
}


// Fonctions d'implémentation

sf::Vector2i World::randomN(){
    int i(uniform(0,3));
    if (i == 0){
        sf::Vector2i temp(-1,0);
        return temp;
    } else if (i == 1){
        sf::Vector2i temp(1,0);
        return temp;
    } else if (i == 2){
        sf::Vector2i temp(0,-1);
        return temp;
    } else {
        sf::Vector2i temp(0,1);
        return temp;
    }
}

void World::clamp(sf::Vector2i& vect){
    if (vect.x > nb_cells-1){
        vect.x = nb_cells-1;
    }
    if (vect.y > nb_cells-1){
        vect.y = nb_cells-1;
    }
    if (vect.x < 0){
        vect.x = 0;
    }
    if (vect.y < 0){
        vect.y = 0;
    }
}
