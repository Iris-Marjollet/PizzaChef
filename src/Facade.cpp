#include "Facade.h"
#include "SFML/Audio/Music.hpp"
#include <iostream>
#include <map>

const sf::Time Facade::TimePerFrame = sf::seconds(1.f/60.f); // The game is running at 60 FPS

Facade::Facade()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktopMode.width;
    unsigned int screenHeight = desktopMode.height;

    // Create the SFML window with the screen size
    window.create(sf::VideoMode(screenWidth, screenHeight), "Pizza Glouton");
    window.setVerticalSyncEnabled(true);

    //setting textures
    std::vector<std::string> vect = {"bois","storage_cheese", "you_lost","storage_tomatoe", "storage_pepperoni", "storage_mushroom", "preparation_tomatoe", "preparation_pepperoni", "preparation_cheese", "cooked_cheese", "check_mark", "sound_on", "sound_off", "timer", "3", "2", "1", "madame", "monsieur", "arm", "tomato_sauce", "pepperonis", "mushrooms", "post-it", "cheese", "pepperoni", "mushroom", "tomato", "pan", "storage_pepper", "pepper", "peppers_cut", "preparation_mushroom", "preparation_pepper2"};
    for (auto& element : vect) {
        textures.insert(addTextureFromFile(element));
    }
    kitchenManager.setTextures(textures);

    //Background
    sprite_background.setTexture(textures.at("bois"));
    // Calculate the scale factors to fill the window
    float scaleX = static_cast<float>(window.getSize().x) / textures.at("bois").getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / textures.at("bois").getSize().y;
    sprite_background.setScale(scaleX, scaleY); // Set the scale of the sprite to fill the window


    //Create ingredients and filling the pizza pool
    Ingredient tomatoe("tomatoe",0, (170*screenWidth-20)/2500, textures.at("tomato_sauce"), "preparation_tomatoe", "storage_tomatoe", textures.at("tomato"), 1, "preparation_pepper2");
    Ingredient cheese("cheese", 1, (170*screenWidth-20)/2500, textures.at("cooked_cheese"), "preparation_cheese", "storage_cheese", textures.at("cheese"),1, "preparation_pepper2");
    Ingredient pepperoni("pepperoni", 2, (170*screenWidth-20)/2500, textures.at("pepperonis"), "preparation_pepperoni", "storage_pepperoni", textures.at("pepperoni"), 1, "preparation_pepper2");
    Ingredient mushroom("mushroom", 3, (170*screenWidth-20)/2500, textures.at("mushrooms"), "preparation_mushroom", "storage_mushroom", textures.at("mushroom"), 1, "pan");
    Ingredient pepper("pepper", 4, (170*screenWidth-20)/2500, textures.at("peppers_cut"), "preparation_pepper2", "storage_pepper", textures.at("pepper"), 2, "pan");
    ingredients.push_back(tomatoe);
    ingredients.push_back(cheese);
    ingredients.push_back(pepperoni);
    ingredients.push_back(mushroom);
    ingredients.push_back(pepper);
    pizzaManager.setIngredients(ingredients);
    kitchenManager.setIngredients(ingredients);


    //Set up the score
    sf::Font font;
    if (!font.loadFromFile("resources/font.ttf")) {
    }
    scoreText.setFont(font);
    for (int j=0; j<4; j++){
        sf::Text recipe;
        recipe.setFont(font);
        recipeNotes.push_back(recipe);
    }


    //Set up the music
    if (!music.openFromFile("resources/music.ogg")) {
    }
    music.setVolume(20.0);
    music.play();
    sound_on_off = true;

    run();
}

void Facade::run() {

    // Set up the clock
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    //Initialize the game
    init();
    pizzaManager.pizzaGenerator(window, postit);
    draw_init( window.getSize().x, window.getSize().y);

    //Main part of the game
    while (window.isOpen()) {

        //In case of game over
        if(pizzaManager.getLives()<0) window.close();

        //The game is updated to match the 60fps defined earlier
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            update(elapsedTime);
            elapsedTime = clock.restart();
            pizzaManager.movePizzas(window, lifeline, textures, postit);
            render();
        }
    }

    if(pizzaManager.getLives()<=0){
        renderLost();
    }

}

void Facade::init(){

    //Set up the storages and preparations

    kitchenManager.initPrepAndStorage();

}

void Facade::draw_init(unsigned int screenWidth, unsigned int screenHeight) {


    window.setFramerateLimit(60);

    //Score
    score_board.setSize(sf::Vector2f(400, 100));
    score_board.setPosition(20, 20);
    score_board.setFillColor(sf::Color::Black);
    score_board.setOutlineColor(sf::Color::White);
    score_board.setOutlineThickness(3);

    setText(scoreText, 50, sf::Color::White, 40, 30, "Your Score: " + std::to_string(pizzaManager.getScore()));
    //Set up the sound
    setTextureScalePosition(sound, textures.at("sound_on"), 0.09, screenWidth * 14 / 15, 0);

    //Set up the conveyor belt
    belt.setSize(sf::Vector2f(screenWidth, screenHeight / 3));
    belt.setPosition(0, 6 * screenHeight / 10+85);
    sf::Color greyColor(105, 105, 105);
    belt.setFillColor(greyColor);

    //Set up the lives
    setTextureScalePosition(lifeline, textures.at("3"), 0.15, 0,
                            score_board.getPosition().y + score_board.getSize().y - 20);

    //Characters
    setTextureScalePosition(postit, textures.at("post-it"), 0.5 * screenWidth / 2500, 0,
                            belt.getPosition().y - textures.at("post-it").getSize().y * 0.5 * screenWidth / 2500);
    setText(recipeNotes[0], 30, sf::Color::Black,
            postit.getPosition().x + postit.getTextureRect().height * postit.getScale().x * 0.1,
            postit.getPosition().y + postit.getTextureRect().width * postit.getScale().y * 0.3, "Next Pizza: ");
    for (int i = 1; i < recipeNotes.size(); i++) {
        setText(recipeNotes[i], 30, sf::Color::Black,
                postit.getPosition().x + postit.getTextureRect().height * postit.getScale().x * 0.1,
                postit.getPosition().y + postit.getTextureRect().width * postit.getScale().y * (0.3 + 0.2 * i),
                "Pizza " + std::to_string(i + 1) + ": ");
    }
    monsieur.setTexture(textures.at("monsieur"));
    float scaleFactorMonsieur = 1.1 * screenWidth / 2500;
    sf::Vector2f monsieurPosition(
            window.getSize().x * 0.65 - monsieur.getTextureRect().height * scaleFactorMonsieur / 3,
            belt.getPosition().y - monsieur.getTextureRect().height * scaleFactorMonsieur / 4);
    monsieur.setScale(scaleFactorMonsieur, scaleFactorMonsieur);
    monsieur.setPosition(monsieurPosition);

    setTextureScalePosition(monsieur_arm, textures.at("arm"), scaleFactorMonsieur, monsieurPosition.x,
                            monsieurPosition.y);

    kitchenManager.setSprites();
}

void Facade::render() {

    window.clear();
    window.draw(sprite_background);

    kitchenManager.printKitchen(window);


    window.draw(score_board);
    scoreText.setString("Your Score: " + std::to_string(pizzaManager.getScore()));
    window.draw(scoreText);

    if (pizzaManager.getLives() == 2) {
        lifeline.setTexture(textures.at("2"));
    } else if (pizzaManager.getLives() == 1) {
        lifeline.setTexture(textures.at("1"));
    }
    if (pizzaManager.getLives() == 0) {
        cout << "Perdu";
        window.close();
    }
    window.draw(lifeline);

    window.draw(postit);
    window.draw(belt);
    window.draw(monsieur);
    window.draw(recipeNote);


    pizzaManager.printPizza(window, postit);
    for (int i = 0; i<recipeNotes.size(); i++){
        window.draw(recipeNotes[i]);
    }

    window.draw(monsieur_arm);

    window.draw(sound);

    window.display();

}

void Facade::renderLost(){
    sf::VideoMode desktopMode2 = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth2 = desktopMode2.width;
    unsigned int screenHeight2 = desktopMode2.height;
    sf::RenderWindow window2(sf::VideoMode(800, 600), "Window 2");

    window2.setFramerateLimit(60);
    window2.create(sf::VideoMode(screenWidth2, screenHeight2), "Lose window");
    while (window2.isOpen()) {
        sf::Texture dead = loadTextureFromFile("resources/you_lost.png");
        sf::Sprite death;
        death.setTexture(dead);
        float scaleX2 = static_cast<float>(window2.getSize().x) / dead.getSize().x;
        float scaleY2 = static_cast<float>(window2.getSize().y) / dead.getSize().y;
        death.setScale(scaleX2, scaleY2); // Set the scale of the sprite to fill the window
        sf::Event event2;
        while(window2.pollEvent(event2)) {
            if (event2.type == sf::Event::Closed) window2.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window2.close();
        }
        window2.clear();
        window2.draw(death);
        window2.display();

    }
}

void Facade::update(sf::Time elapsed_time) {

    bool isTouched = false; //to test if an object was touched

    //Update the preparations preparing
    kitchenManager.prepareIfNeeded(elapsed_time);

    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                // Check for storage click
                bool isTouchedStorage = kitchenManager.onTouchedStorage(mousePos, window);
                bool isTouchedPreparation = kitchenManager.onTouchedPreparation(mousePos, window);

                if (kitchenManager.getSelectedType() == "preparation") {
                    bool added = pizzaManager.checkPizzaClick(kitchenManager.getSelected(), mousePos);
                    if(added){
                        kitchenManager.addIngredient();
                    }
                }
                if(sound.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if(sound_on_off) {
                        sound_on_off = false;
                        sound.setTexture(textures.at("sound_off"));
                        music.pause();

                    }
                    else{
                        sound_on_off = true;
                        sound.setTexture(textures.at("sound_on"));
                        music.play();
                    }
                }

                if (!isTouchedStorage && !isTouchedPreparation){
                    kitchenManager.resetSelected();
                    kitchenManager.setSelectedType("nothing");
                }
        }
    }

}

sf::Texture Facade::loadTextureFromFile(const std::string& filePath) const {
    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
    }
    return texture;
}


std::pair<std::string,sf::Texture> Facade::addTextureFromFile(const std::string& name) const {
    sf::Texture texture;
    if (!texture.loadFromFile("resources/"+name+".png")) {
    }

    return std::make_pair(name, texture);
}

void Facade::setTextureScalePosition(sf::Sprite& sprite, sf::Texture& texture, double scale, double position_x, double position_y) const {
    sprite.setTexture(texture);
    sprite.setScale(scale,scale);
    sprite.setPosition(position_x, position_y);
}

void Facade::setText(sf::Text& text, int characterSize, sf::Color color, float position_x, float position_y, std::string string) const {
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
    text.setPosition(position_x, position_y);
    text.setString(string);
}
