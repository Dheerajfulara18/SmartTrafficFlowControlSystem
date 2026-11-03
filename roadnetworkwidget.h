#ifndef ROADNETWORKWIDGET_H
#define ROADNETWORKWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;
class QPainter;
class RoadGraphCanvas;

class RoadNetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoadNetworkWidget(QWidget *parent = nullptr);

private slots:
    void calculateShortestPath();
    void addRoad();

private:
    // Graph Data Structure
    QVector<QVector<int>> graph;

    // Node positions
    QVector<QPointF> nodePositions;

    // Shortest path
    QVector<int> shortestPath;

    // Route selection
    QComboBox *sourceCombo;
    QComboBox *destinationCombo;

    // Add road selection
    QComboBox *addFromCombo;
    QComboBox *addToCombo;
    QSpinBox *distanceInput;

    // Buttons
    QPushButton *findRouteButton;
    QPushButton *addRoadButton;

    // Result
    QLabel *resultLabel;

    // Graph drawing area
    RoadGraphCanvas *graphCanvas;

    int routeDistance = 0;

    void initializeGraph();

    void addRoadToGraph(
        int source,
        int destination,
        int distance
        );

    void drawGraph(QPainter &painter);
};

#endif // ROADNETWORKWIDGET_H