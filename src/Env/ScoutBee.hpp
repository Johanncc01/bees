#pragma once
#include <Env/Bee.hpp>


class ScoutBee : public Bee
{
public:
// Constructeur et destructeur

    /*!
     * @brief Constructeur de base d'une éclaireuse
     *
     * @param référence à une ruche
     * @param position de l'éclaireuse
     */
    ScoutBee(Hive&, Vec2d const&);

    /*!
     * @brief Destructeur d'une éclaireuse (par défaut)
     */
    ~ScoutBee();


// Dessin d'une abeille (substitution depuis Bee)

    /*!
     * @brief Dessine l'éclaireuse selon ses caractéristiques sur une cible donnée (+ gestion spécifique de l'affichage du debug)
     *
     * @param "RenderTarget" sur laquelle l'éclaireuse est dessinée
     */
    void drawOn(sf::RenderTarget&) const override;


// Gestion des états (substitution depuis CFSM)

    /*!
     * @brief Fait évoluer l'éclaireuse dans un état donné sur un temps donné
     *
     * @param état (Uid) correspondant aux traitements voulus
     * @param temps dt sur lequel l'évolution est effectuée
     */
    void onState(State, sf::Time) override;

    /*!
     * @brief Réalise les actions lors d'un changement vers un nouvel état donné
     *
     * @param état (Uid) d'entrée de l'éclaireuse
     */
    void onEnterState(State) override;


// Interactions (substitution depuis Bee)

    /*!
     * @brief Interragit avec une autre abeille (double dispatch : appelle la méthode interactWith() de l'autre abeille avec comme argument "this")
     *
     * @param pointeur sur l'abeille avec laquelle on veut interragir
     */
    void interact(Bee*) override;

    /*!
     * @brief Interragit concrètement avec une autre abeille (l'instance courrante, pour que le polymorphisme fonctionne)
     *
     * @param pointeur sur l'abeille (éclaireuse) qui voulait interragir ("this" de Bee::interact())
     */
    void interactWith(ScoutBee*) override;

    /*!
     * @brief Interragit concrètement avec une autre abeille (l'instance courrante, pour que le polymorphisme fonctionne)
     *
     * @param pointeur sur l'abeille (butineurse) qui voulait interragir ("this" de Bee::interact())
     */
    void interactWith(WorkerBee*) override;


private:

    // Etats d'une éclaireuse (arguments de classe)
    static State const IN_HIVE;
    static State const LF_FLOWER;
    static State const BACK_TO_HIVE;

    int shareCounter;


// Getter privé
    /*!
    * @brief Getter de la configuration d'une éclaireuse
    *
    * @return un tableau contenant les valeurs coresspondantes aux éclaireuses dans le .json
    */
    j::Value const& getConfig() const override;

};

