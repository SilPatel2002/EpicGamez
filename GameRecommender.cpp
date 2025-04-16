#include "GameRecommender.h"

Game::Game(string name, vector<string> genres, vector<string> features, int rating, float price){

    this->name = name;
    this-> genres = genres;
    this-> features = features;
    this->rating = rating;
    this->price = price;

}