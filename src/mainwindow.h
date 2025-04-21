#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include "GameRecommender.h"

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // UI Elements
    QLineEdit *gameNameInput;
    QCheckBox *fpsLooterCheck;
    QCheckBox *rpgCheck;
    QCheckBox *openWorldCheck;
    QLabel *depthFirstTime;
    QLabel *breadthFirstTime;
    QListWidget *similarGamesList;
    GameRecommender m_recommender;

    void setupUI();

private slots:
    void onSearchClicked();
};

#endif // MAINWINDOW_H