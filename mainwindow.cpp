#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QElapsedTimer>
#include "CSVReader.cpp"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    setupUI();

    // Load CSV data into the recommender
    readCSV("games.csv", m_recommender); // Pass m_recommender by reference

    // Build the adjacency list after loading data
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

    // Genre Selection (unchanged)
    QGroupBox *genreGroup = new QGroupBox("Genre", centralWidget);
    QVBoxLayout *genreLayout = new QVBoxLayout;
    fpsLooterCheck = new QCheckBox("FPS Looter", centralWidget);
    rpgCheck = new QCheckBox("RPG", centralWidget);
    openWorldCheck = new QCheckBox("Open World", centralWidget);
    genreLayout->addWidget(fpsLooterCheck);
    genreLayout->addWidget(rpgCheck);
    genreLayout->addWidget(openWorldCheck);
    genreGroup->setLayout(genreLayout);
    mainLayout->addWidget(genreGroup);

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

    // Measure DFS time
    QElapsedTimer dfsTimer;
    dfsTimer.start();
    auto dfsResult = m_recommender.depthFirstSearch(gameName, "");
    qint64 dfsElapsed = dfsTimer.elapsed();
    depthFirstTime->setText(QString("DFS Time: %1 ms").arg(dfsElapsed));

    // Measure BFS time
    QElapsedTimer bfsTimer;
    bfsTimer.start();
    auto bfsResult = m_recommender.breadthFirstSearch(gameName, "");
    qint64 bfsElapsed = bfsTimer.elapsed();
    breadthFirstTime->setText(QString("BFS Time: %1 ms").arg(bfsElapsed));

    // Get similar games
    Game targetGame;
    targetGame.name = gameName;
    auto similarGames = m_recommender.getSimilarGames(targetGame, 5);

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