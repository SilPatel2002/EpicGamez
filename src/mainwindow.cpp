#include "mainwindow.h"
#include "CSVReader.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    setupUI();
    readCSV("data/steam_games.csv", m_recommender); // Load CSV data

    // Add test games with sufficient similarity
    // vector<string> genres1 = {"Action"};
    // vector<string> features1 = {"Multiplayer"};
    // m_recommender.addGame("Game1", genres1, features1, 4.5f, 29.99f);

    // vector<string> genres2 = {"Action", "RPG"};
    // vector<string> features2 = {"Multiplayer"};
    // m_recommender.addGame("Game2", genres2, features2, 4.8f, 39.99f);

    // vector<string> genres3 = {"RPG", "Open World"};
    // vector<string> features3 = {"Multiplayer", "Singleplayer"};
    // m_recommender.addGame("Game3", genres3, features3, 4.7f, 49.99f);

    m_recommender.buildAdjList();
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Game Name Input
    gameNameInput = new QLineEdit(centralWidget);
    gameNameInput->setPlaceholderText("Enter favorite game name here....");
    mainLayout->addWidget(gameNameInput);

    // Search Button
    QPushButton *searchButton = new QPushButton("Search", centralWidget);
    mainLayout->addWidget(searchButton);

    // Algorithms Section
    QHBoxLayout *algoLayout = new QHBoxLayout;

    // Algorithm 1 - DFS
    QGroupBox *algo1Group = new QGroupBox("Algorithm 1 - Depth-First", centralWidget);
    QVBoxLayout *algo1Layout = new QVBoxLayout;
    depthFirstTime = new QLabel("DFS Time: 0 ms", centralWidget);
    algo1Layout->addWidget(depthFirstTime);
    algo1Group->setLayout(algo1Layout);
    algoLayout->addWidget(algo1Group);

    // Algorithm 2 - BFS
    QGroupBox *algo2Group = new QGroupBox("Algorithm 2 - Breadth-First", centralWidget);
    QVBoxLayout *algo2Layout = new QVBoxLayout;
    breadthFirstTime = new QLabel("BFS Time: 0 ms", centralWidget);
    algo2Layout->addWidget(breadthFirstTime);
    algo2Group->setLayout(algo2Layout);
    algoLayout->addWidget(algo2Group);

    mainLayout->addLayout(algoLayout);

    // Similar Games List
    QGroupBox *similarGroup = new QGroupBox("Similar Games", centralWidget);
    QVBoxLayout *similarLayout = new QVBoxLayout;
    similarGamesList = new QListWidget(centralWidget);
    similarLayout->addWidget(similarGamesList);
    similarGroup->setLayout(similarLayout);
    mainLayout->addWidget(similarGroup);

    // Connect button
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    setCentralWidget(centralWidget);
    setWindowTitle("Game Analysis Tool");
    setMinimumSize(600, 400);
}

void MainWindow::onSearchClicked() {
    QString inputName = gameNameInput->text();
    if (inputName.isEmpty()) return;

    std::string gameName = inputName.toStdString();

    if (!m_recommender.gameExists(gameName)) {
        similarGamesList->clear();
        similarGamesList->addItem("Game not found in database");
        depthFirstTime->setText("DFS Time: N/A");
        breadthFirstTime->setText("BFS Time: N/A");
        return;
    }

    // Measure DFS time
    QElapsedTimer dfsTimer;
    dfsTimer.start();
    auto dfsResult = m_recommender.depthFirstSearch(gameName, "");
    double dfsElapsed = dfsTimer.nsecsElapsed() / 1'000'000.0;
    depthFirstTime->setText(QString("DFS Time: %1 ms").arg(dfsElapsed, 0, 'f', 4));

    // Measure BFS time
    QElapsedTimer bfsTimer;
    bfsTimer.start();
    auto bfsResult = m_recommender.breadthFirstSearch(gameName, "");
    double bfsElapsed = bfsTimer.nsecsElapsed() / 1'000'000.0; // Convert nanoseconds to milliseconds
    breadthFirstTime->setText(QString("BFS Time: %1 ms").arg(bfsElapsed, 0, 'f', 4));

    // Get similar games
    Game targetGame;
    targetGame.name = gameName;
    auto similarGames = m_recommender.getSimilarGames(targetGame, 10);

    // Display similar games
    similarGamesList->clear();
    for (const auto& game : similarGames) {
        QString entry = QString("%1 (%2%)")
                .arg(QString::fromStdString(game.first))
                .arg(game.second * 100, 0, 'f', 1);
        similarGamesList->addItem(entry);
    }
}

MainWindow::~MainWindow() {}
