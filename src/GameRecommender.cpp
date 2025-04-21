#include "GameRecommender.h"

#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <tuple>
#include <omp.h>

using namespace std;

Game::Game() {
    cout << "Creating empty Game object" << endl;
}

Game::Game(string name, vector<string> genres, vector<string> features, float rating, float price)
{
    this->name = name;
    this->genres = genres;
    this->features = features;
    this->rating = rating;
    this->price = price;
}

bool GameRecommender::gameExists(const std::string& name) const {
    bool exists = gameDatabase.find(name) != gameDatabase.end();
    return exists;
}

float GameRecommender::calcGameSimilarity(const Game &game1, const Game &game2) {

    //Genre Simularity
    unordered_set<string> game1Genres(game1.genres.begin(), game1.genres.end());
    unordered_set<string> game2Genres(game2.genres.begin(), game2.genres.end());
    unordered_set<string> genreIntersection;
    for (const auto& genre : game1Genres) {
        if (game2Genres.count(genre)) {
            genreIntersection.insert(genre);
        }
    }
    unordered_set<string> genreUnion = game1Genres;
    genreUnion.insert(game2Genres.begin(), game2Genres.end());
    float genreSimilarity = genreUnion.empty() ? 0.0f :                                         //if empty then 0, else
                                static_cast<float>(genreIntersection.size()) / genreUnion.size(); //∣A∩B∣ / ∣AUB∣

    // Feature similarity
    unordered_set<string> game1Features(game1.features.begin(), game1.features.end());
    unordered_set<string> game2Features(game2.features.begin(), game2.features.end());
    unordered_set<string> featureIntersection;
    for (const auto& feature : game1Features) {
        if (game2Features.count(feature)) {
            featureIntersection.insert(feature);
        }
    }

    unordered_set<string> featureUnion = game1Features;
    featureUnion.insert(game2Features.begin(), game2Features.end());
    float featSimilarity = featureUnion.empty() ? 0.0f :
                               static_cast<float>(featureIntersection.size()) / featureUnion.size();


    float totalSimilarity = (0.7f * featSimilarity) + (0.3f * genreSimilarity);

    return totalSimilarity;
}

void GameRecommender::addEdge(const Game &game1, const Game &game2, float similarity) {

    adjList[game1.name].emplace_back(make_pair(game2.name, similarity));
    adjList[game2.name].emplace_back(make_pair(game1.name, similarity));
}

void GameRecommender::addGame(const string& name, vector<string>& genre, vector<string>& features, float rating, float price) {
    Game game(name, genre, features, rating, price);
    gameDatabase.emplace(game.name, game);
}

void GameRecommender::buildAdjList() {
    cout << "=== BUILDING ADJACENCY LIST ===" << endl;
    int edgeCount = 0;

    // Convert gameDatabase to a vector for random access
    vector<const Game*> games;
    games.reserve(gameDatabase.size());
    for (const auto& entry : gameDatabase) {
        games.push_back(&entry.second);
    }

    // Thread-local storage for edges to avoid locks
    vector<vector<tuple<string, string, float>>> threadEdges(omp_get_max_threads());

#pragma omp parallel for
    // Loop through each game in the list
    // compare every pair of games
    for (size_t i = 0; i < games.size(); ++i) {
        const Game* gameA = games[i];
        int thread_id = omp_get_thread_num();
        auto& localEdges = threadEdges[thread_id];

        for (size_t j = i + 1; j < games.size(); ++j) {
            const Game* gameB = games[j];
            if (gameA->rating < 5 || gameB->rating < 5) continue;

            float similarity = calcGameSimilarity(*gameA, *gameB);
            if (similarity > 0.5f) {
                localEdges.emplace_back(gameA->name, gameB->name, similarity);
            }
        }
    }

    // Merge edges into adjList sequentially
    for (const auto& edges : threadEdges) {
        for (const auto& edge : edges) {
            const string& nameA = get<0>(edge);
            const string& nameB = get<1>(edge);
            float sim = get<2>(edge);
            adjList[nameA].emplace_back(nameB, sim);
            adjList[nameB].emplace_back(nameA, sim);
            edgeCount++;
        }
    }

    cout << "\n=== ADJACENCY LIST COMPLETE ===" << endl;
    cout << "Total edges created: " << edgeCount << endl;
    cout << "Total nodes in graph: " << adjList.size() << endl;
    cout << "===============================\n" << endl;
}

vector<pair<string, float>> GameRecommender::getSimilarGames(Game game, int count) {

    vector<pair<string, float>> similarGames = adjList.at(game.name);

    sort(similarGames.begin(), similarGames.end(),
         [](const auto& a, const auto& b) { return a.second > b.second; });

    if (similarGames.size() > count) {
        similarGames.resize(count);
    }
    else {
        cout << "Returning all " << similarGames.size() << " matches" << endl;
    }

    cout << "Similar games for " << game.name << ":" << endl;
    for (const auto& match : similarGames) {
        cout << "- " << match.first << " (" << match.second << ")" << endl;
    }

    return similarGames;
}

vector<string> GameRecommender::depthFirstSearch(string src, string dest) {
    vector<string> dfs;
    set<string> visited;
    stack<string> s;

    visited.insert(src);
    s.push(src);

    while(!s.empty()) {
        string u = s.top();
        s.pop();
        dfs.push_back(u);
        vector<pair<string,float>> neighbors = adjList[u];

        for (auto& v : neighbors) {

            if (visited.count(v.first) == 0) {
                visited.insert(v.first);
                s.push(v.first);
            }
        }
    }
    return dfs;
}

vector<string> GameRecommender::breadthFirstSearch(string src, string dest) {
    vector<string> bfs;
    set<string> visited;
    queue<string> q;

    visited.insert(src);
    q.push(src);

    while(!q.empty()) {
        string u = q.front();
        q.pop();
        bfs.push_back(u);

        vector<pair<string,float>> neighbors = adjList[u];
        sort(neighbors.begin(), neighbors.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });


        for (auto& v : neighbors) {

            if (visited.count(v.first) == 0) {
                visited.insert(v.first);
                q.push(v.first);
            }
        }
    }
    return bfs;
}
