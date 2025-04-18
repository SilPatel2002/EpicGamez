#include "GameRecommender.h"

#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>

using namespace std;

Game::Game(){
}

Game::Game(string name, vector<string> genres, vector<string> features, float rating, float price)
{

    this->name = name;
    this-> genres = genres;
    this-> features = features;
    this->rating = rating;
    this->price = price;
}

float GameRecommender::calcGameSimilarity(const Game &game1, const Game &game2){

    unordered_set<string> genreUnion;           //find the difference between the two games. 
    unordered_set<string> genreIntersection;    
    float genreSimilarity;                      // store the similarity between the genres

    for (const auto& genre : game1.genres){
        genreUnion.insert(genre);
    }

    for (const auto& genre: game2.genres){
        genreUnion.insert(genre);

        if (genreUnion.count(genre) == 1){
            genreIntersection.insert(genre);
        }
    }

    genreSimilarity = static_cast<float>(genreIntersection.size()) / static_cast<float>(genreUnion.size());
            // this will be a ratio closer to 1 is more similar

    unordered_set<string> featUnion;            // do the same things with the game features
    unordered_set<string> featIntersection;
    float featSimilarity;

    for (const auto& feat : game1.features){
        featUnion.insert(feat);
    }

    for (const auto& feat : game2.features){
        featUnion.insert(feat);

        if (featUnion.count(feat) == 1){
            featIntersection.insert(feat);
        }
    }

    featSimilarity = static_cast<float>(featIntersection.size()) / static_cast<float>(featUnion.size());


        //maybe change the numbers
    float weightedSimilarity = (0.7f * featSimilarity) + (0.3f * genreSimilarity);
 

    return weightedSimilarity;
}

void GameRecommender::addEdge(const Game &game1, const Game &game2, float& similarity){ //add a single edge to the adjList

    adjList[game1.name].emplace_back(make_pair(game2.name, similarity));


}

void GameRecommender::addGame(const string& name, vector<string>& genre, vector<string>& features, float rating, float price){ 
    
    Game game(name, genre, features, rating, price);

    //add the game data to the gameDatabase
    gameDatabase.emplace(game.name, game);
    
}

void GameRecommender::buildAdjList(){

    for (auto itA = gameDatabase.begin(); itA != gameDatabase.end(); itA++){  //loop through each combination of the games
        for (auto itB = next(itA); itB != gameDatabase.end(); itB++){

            float similarity = calcGameSimilarity(itA -> second, itB -> second);

            cout << similarity << endl;
            if (similarity > 0.5f){   //store edges between games that are similar
                addEdge(itA -> second, itB ->second, similarity);
                addEdge(itB -> second, itA -> second, similarity);
            }
        }
    }


    for (auto& [name, neighbors] : adjList){
        cout << name << " ";
        for (auto& neighbor : neighbors){
            cout << neighbor.first << " " << neighbor.second << " ";
        }
        cout << endl;
    }

}

vector<pair<string, float>> GameRecommender::getSimilarGames(Game game, int count){

    vector<pair<string, float>> similarGames;

    similarGames = adjList.at(game.name);

    sort(similarGames.begin(), similarGames.end(), [](const auto& a, const auto& b) {return a.second > b.second;});
            //sort the list from highest similarity score to lowest

    if (similarGames.size() > count){
        similarGames.resize(count);
    }


    return similarGames;

}

vector<string> GameRecommender::depthFirstSearch(string src, string dest){

    vector<string> dfs;
    set<string> visited;
    stack<string> s;

    visited.insert(src);
    s.push(src);

    while(!s.empty()){
        
        string u = s.top();
        dfs.push_back(u);
        s.pop();
        vector<pair<string,float>> neighbors = adjList[u];

        for (auto& v : neighbors){ // v is a pair of (string, float)
            
            if (visited.count(v.first) == 0){   // if game not visited add it to the set and stack
                visited.insert(v.first);
                s.push(v.first);
            }

        }

    }


    return dfs;
}

vector<string> GameRecommender::breadthFirstSearch(string src, string dest){

    vector<string> bfs;
    set<string> visited;
    queue<string> q;

    visited.insert(src);
    q.push(src);

    while(!q.empty()){
        
        string u = q.front();
        bfs.push_back(u);
        q.pop();
        vector<pair<string,float>> neighbors = adjList[u];
        sort(neighbors.begin(), neighbors.end(), [](const auto& a, const auto& b) {return a.second > b.second;});
            //sort the list from highest similarity score to lowest

        for (auto& v : neighbors){ // v is a pair of (string, float)
            
            if (visited.count(v.first) == 0){   // if game not visited add it to the set and stack
                visited.insert(v.first);
                q.push(v.first);
            }
        }
    }  

    return bfs;
}
