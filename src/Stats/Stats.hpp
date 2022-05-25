#pragma once
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Stats/Graph.hpp>

#include <map>
#include <memory>
#include <vector>


// typedef des types complexes
typedef std::map<int, std::unique_ptr<Graph>> Graphs;
typedef std::map<int, std::string> Strings;

class Stats : public Drawable, public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur par défaut d'une statistique, initialisant son index et son compteur à 0
     */
    Stats();

    /*!
     * @brief Destructeur d'une statistique (par défaut car les unique_ptr se désallouent automatiquement)
     */
    ~Stats() = default;


// Getters et setters

    /*!
     * @brief Setter de l'indice du graphe actif
     *
     * @param l'entier à utiliser comme nouvel indice
     */
    void setActive(int);

    /*!
     * @brief Getter du titre du graphe actif
     *
     * @return la string correspondant à l'indice actuel
     */
    std::string getCurrentTitle() const;

    /*!
     * @brief Passe au graphe suivant des statistiques
     */
    void next();

    /*!
     * @brief Passe au graphe précédent des statistiques
     */
    void previous();


// Méthodes pures

    /*!
     * @brief Dessine les statsistiques (ensemble de graphs) sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle les graphes sont dessinés
     */
    void drawOn(sf::RenderTarget& target) const override;

    /*!
     * @brief Actualise les statistiques
     *
     * @param temps dt sur lequel il faut actualiser les statistiques
     */
    void update(sf::Time dt) override;

// Gestion des statistiques

    /*!
     * @brief Réinitialise les statistiques, en remettant le compteur à 0 et appellant Graph::reset()
     */
    void reset();

    /*!
     * @brief Crée un graph avec les données en paramètre
     *
     * @param l'identifiant du graphe
     * @param le libellé du graphe
     * @param l'ensemble des titres de courbes du graphe
     * @param valeur minimale d'affichage du graphe
     * @param valeur maximale d'affichage du graphe
     * @param dimensions de la fenêtre graphique réalisant les affichages
     */
    void addGraph(int, std::string const&, std::vector<std::string> const&, double min, double max, Vec2d const&);


private:

    Graphs graphs;
    Strings strings;

    int current;

    sf::Time compteur;

};

