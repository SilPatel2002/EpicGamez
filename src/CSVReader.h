#ifndef CSVREADER_H
#define CSVREADER_H
#pragma once
#include "GameRecommender.h"

int readCSV(const std::string& filename, GameRecommender& recommender);

#endif // CSVREADER_H
