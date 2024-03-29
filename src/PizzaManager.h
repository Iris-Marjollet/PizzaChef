//
// Created by Sarah Zakon on 07/07/2023.
//
#pragma once
#include "Ingredient.h"
#include <vector>
#include <map>
#include "Pizza.h"
#include "Kitchen.h"
#include "Storage.h"
#include <memory>
#include <optional>
#include <random>
#ifndef GLOUTON_PIZZAMANAGER_H
#define GLOUTON_PIZZAMANAGER_H


class PizzaManager {
private:
    std::vector<Pizza*> pizzas;
    int lives;
    int score;
    static const float xVelocity;
    std::vector<Ingredient> ingredients;
    int numberOfPizzas;
    sf::RenderWindow window;
public:
    explicit PizzaManager();
    void setIngredients(std::vector<Ingredient> ingredients);
    std::vector<Pizza*> getPizzas();
    int getScore();
    int getLives();
    void addRandomIngredient(Pizza* pizza, Ingredient ingredient);
    void pizzaGenerator(sf::RenderWindow& window, sf::Sprite postit);
    std::vector<Ingredient> randomIngrVect();
    std::vector<Ingredient> createVect(int i);
    void randomIngr(Pizza* pizza);
    void movePizzas(sf::RenderWindow& window, sf::Sprite lifeline, std::map<std::string, sf::Texture> textures, sf::Sprite postit);
    void releasePizza(Pizza* pizza, sf::RenderWindow& window, sf::Sprite postit);
    bool checkPizzaClick(std::optional<Kitchen> selected, sf::Vector2i mousePos);
    void printPizza(sf::RenderWindow& window, sf::Sprite postit);
    void printRecipe(sf::RenderWindow& window, sf::Sprite postit);


    };


#endif //GLOUTON_PIZZAMANAGER_H
