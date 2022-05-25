#pragma once
#include <Env/Collider.hpp>
#include <Env/CFSM.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <Env/Hive.hpp>

// Prédéclaration des classes
class ScoutBee;
class WorkerBee;

// Mode de déplacement des abeilles
enum class Mode : short { repos, random, target};


class Bee : public Collider, public CFSM, public Drawable , public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une abeille
     *
     * @param référence à une ruche
     * @param position de l'abeille
     * @param tableau des états possibles pour l'abeille
     * @param rayon de l'abeille
     * @param énergie initiale de l'abeille
     * @param amplitude initiale de la vitesse de l'abeille
     */
    Bee(Hive&, Vec2d const&, States, double, double, double);

    /*!
     * @brief Destructeur d'une abeille, qui désalloue l'espace pour la mémoire et la cible
     *        Déclaré virtual pour que la destruction des sous-classe se fasse complètement
     */
    virtual ~Bee();


// Getters

    /*!
     * @brief Getter de la vie de l'abeille
     *
     * @return vrai si l'énergie n'est pas nulle ou négative
     */
    bool isDead();


// Méthodes pures

    /*!
     * @brief Dessine l'abeille selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle l'abeille est dessinée
     */
    virtual void drawOn(sf::RenderTarget&) const override;

    /*!
     * @brief Actualise l'abeille et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser l'abeille
     */
    void update(sf::Time) override;


// Interactions

    /*!
     * @brief Interragit avec une autre abeille (double dispatch : appelle la méthode interactWith() de l'autre abeille avec comme argument "this")
     *
     * @param pointeur sur l'abeille avec laquelle on veut interragir
     */
    virtual void interact(Bee*) = 0;

    /*!
     * @brief Interragit concrètement avec une autre abeille (l'instance courrante, pour que le polymorphisme fonctionne)
     *
     * @param pointeur sur l'abeille (éclaireuse) qui voulait interragir ("this" de Bee::interact())
     */
    virtual void interactWith(ScoutBee*) = 0;

    /*!
     * @brief Interragit concrètement avec une autre abeille (l'instance courrante, pour que le polymorphisme fonctionne)
     *
     * @param pointeur sur l'abeille (butineurse) qui voulait interragir ("this" de Bee::interact())
     */
    virtual void interactWith(WorkerBee*) = 0;

    /*!
     * @brief Fait mémoriser une position de fleur à l'instance
     *
     * @param le Vec2d associé à la position à mémoriser
     */
    void learnFlowerLocation(Vec2d const&);


protected:

    Hive& hive;
    Vec2d vitesse;
    double energy;

    Vec2d const* memory;
    Vec2d const* target;

    Mode mode;

// Getters pour les classes héritées

    /*!
     * @brief Getter de la configuration en fonction de la sous-classe d'abeille
     *
     * @return un tableau contenant les valeurs coresspondantes au bon type dans le .json
     */
    virtual j::Value const& getConfig() const = 0;

    /*!
     * @brief Getter de la ruche parente
     *
     * @return une référence à la ruche dont l'abeille appartient
     */
    Hive& getHive() const;


// Déplacement

    /*!
     * @brief Déplace l'abeille sur un temps donné en fonction du mode actuel (attribut)
     *
     * @param temps dt sur lequel le déplacement est calculé
     */
    void move(sf::Time);

    /*!
     * @brief Déplace l'abeille aléatoirement sur un temps donné
     *
     * @param temps dt sur lequel le déplacement est calculé
     */
    void randomMove(sf::Time);

    /*!
     * @brief Déplace l'abeille vers le but (attribut) sur un temps donné
     *
     * @param temps dt sur lequel le déplacement est calculé
     */
    void targetMove(sf::Time);


private:


    sf::Time avoidanceClock_;

    // Fonctions d'implémentation

    /*!
     * @brief Affiche des informations avancées (état de la mémoire + cible) de l'abeille sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle le texte est affiché
     */
    void advancedDebugText(sf::RenderTarget&) const;
};

