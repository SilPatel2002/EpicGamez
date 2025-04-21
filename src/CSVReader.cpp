#include "CSVReader.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <sstream>

int readCSV(const std::string& filename, GameRecommender& gamer) {
    QString qFileName = QString::fromStdString(filename);
    QFileInfo fileInfo(qFileName);

    qDebug() << "=== FILE OPENING ATTEMPT ===";
    qDebug() << "Requested path:" << qFileName;
    qDebug() << "Absolute path:" << fileInfo.absoluteFilePath();
    qDebug() << "File exists?" << fileInfo.exists();
    qDebug() << "Is readable?" << fileInfo.isReadable();

    QFile file(qFileName);

    if (!file.exists()) {
        qCritical() << "File not found";
        return 1;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Error: Failed to open file";
        return 1;
    }

    QTextStream in(&file);
    qDebug() << "\n=== STARTING CSV READ ===";

    // Read and skip header
    QString header = in.readLine();

    while (!in.atEnd()) {
        QString qline = in.readLine();
        std::string line = qline.toStdString();

        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;

        // Parse CSV line
        for (char c : line) {
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
        fields.push_back(field);

        if (fields.size() != 18) {
            qDebug() << "! Skipping invalid entry, expected 18 fields, got" << fields.size();
            continue;
        }

        std::string name = fields[1];
        std::string genre = fields[9];
        std::string features = fields[8];
        std::string strPositive = fields[12];  // positive_ratings
        std::string strNegative = fields[13];  // negative_ratings
        std::string strPrice = fields[17];

        // Process genres
        std::vector<std::string> genreVector;
        std::istringstream genreStream(genre);
        std::string token;
        while (std::getline(genreStream, token, ';')) {
            if (!token.empty()) genreVector.push_back(token);
        }

        // Process features (categories)
        std::vector<std::string> featureVector;
        std::istringstream featureStream(features);
        while (std::getline(featureStream, token, ';')) {
            if (!token.empty()) featureVector.push_back(token);
        }

        // Calculate rating percentage
        float rating = -1.0f;
        try {
            int positive = std::stoi(strPositive);                                                          //if there are no ratings available then ignore game
            int negative = std::stoi(strNegative);
            int total = positive + negative;
            if (total > 0) {
                rating = (static_cast<float>(positive) / total) * 100.0f;
            } else {
                rating = 0.0f;  // No ratings available
            }
        } catch (...) {
            qDebug() << "Invalid rating values for" << QString::fromStdString(name);
        }

        // Convert price
        QString qPrice = QString::fromStdString(strPrice);
        if (qPrice.startsWith("$")) qPrice = qPrice.mid(1);
        bool priceOK;
        float price = qPrice.toFloat(&priceOK);
        if (!priceOK) price = -1.0f;

        // Add game to recommender
        gamer.addGame(name, genreVector, featureVector, rating, price);
    }

    qDebug() << "=== CSV READ COMPLETE ===\n";
    file.close();

    return 0;
}
