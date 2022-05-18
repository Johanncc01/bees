#pragma once
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>
#include <memory>
#include <vector>


typedef std::map<int, std::unique_ptr<Graph>> Graphs;
typedef std::map<int, std::string> Strings;

class Stats : public Drawable, public Updatable
{
public:
    Stats();

    ~Stats() = default;


    void setActive(int);
    std::string getCurrentTitle() const;
    void next();
    void previous();


    /*!
     * @brief Dessine les statsistiques (ensemble de graphs) sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle l'environnement est dessinée
     */
    void drawOn(sf::RenderTarget& target) const;

    /*!
     * @brief Actualise les statistiques
     *
     * @param temps dt sur lequel il faut actualiser l'environnement
     */
    void update(sf::Time dt);


    void reset() const;

    void addGraph(int, std::string const&, std::vector<std::string> const&, double min, double max, Vec2d const&);


private:

    Graphs graphs;
    Strings strings;

    int current;

};

