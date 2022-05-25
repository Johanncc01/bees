#pragma once
#include <Env/Collider.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>


// Prédéclaration de la classe
class Bee;

// typedef des types complexes
typedef std::vector<Bee*> Bees;

class Hive : public Collider, public Drawable, public Updatable
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une ruche
     *
     * @param centre de la ruche
     * @param rayon de la ruche
     */
    Hive(Vec2d const&, double);

    /*!
     * @brief Destructeur de la ruche, qui détruit les abeilles
     */
    ~Hive();

    // Interdit les copies et affectations pour éviter les erreurs
    Hive(Hive const&) = delete;
    Hive& operator=(Hive const&) = delete;


// Getters

    /*!
     * @brief Getter de la vie de la ruche
     *
     * @return vrai si la ruche n'a plus de pollen ni d'abeilles
     */
    bool isDead() const;

    /*!
     * @brief Getter de la quantité de pollen de la ruche
     *
     * @return le pollen présent dans la ruche
     */
    double getPollen() const;

    /*!
     * @brief Getter du nombre d'éclaireuses de la ruche
     *
     * @return le nombre d'éclaireuses associées à la ruche
     */
    size_t getScoutNumber() const;

    /*!
     * @brief Getter du nombre de butineuses de la ruche
     *
     * @return le nombre de butineuses associées à la ruche
     */
    size_t getWorkerNumber() const;


// Méthodes pures

    /*!
     * @brief Dessine la ruche selon ses caractéristiques sur une cible donnée
     *
     * @param "RenderTarget" sur laquelle la ruche est dessinée
     */
    void drawOn(sf::RenderTarget&) const override;

    /*!
     * @brief Actualise la ruche et ses caractéristiques sur un temps donné
     *
     * @param temps dt sur lequel il faut actualiser la ruche
     */
    void update(sf::Time) override;


// Pollen

    /*!
     * @brief Dépose une valeur donnée de pollen dans la ruche
     *
     * @param quantité de pollen à déposer
     */
    double dropPollen(double);

    /*!
     * @brief Retire de la ruche la quantité de pollen demandée
     *
     * @param quantité demandée
     *
     * @return quantité qui a pu être retirée
     */
    double takePollen(double);


// Stats

    /*!
     * @brief Adapte le nombre d'éclaireuses de la ruche (appelé dans le constructeur/destructeur)
     *
     * @param vrai pour en ajouter (constructeur), faux pour en retirer une (destructeur)
     */
    void changeScoutNumber(bool);

    /*!
     * @brief Adapte le nombre de butineuses de la ruche (appelé dans le constructeur/destructeur)
     *
     * @param vrai pour en ajouter une (constructeur), faux pour en retirer une (destructeur)
     */
    void changeWorkerNumber(bool);


protected:

// Bees

    /*!
    * @brief Ajoute une abeille dans la ruche
    */
    Bee* addBee(double scoutProb = 0);


private:

    Bees bees;
    double pollen;

    size_t scoutNumber;
    size_t workerNumber;

};

