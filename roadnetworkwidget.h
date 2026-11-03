#ifndef ROADNETWORKWIDGET_H
#define ROADNETWORKWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

class QComboBox;
class QLabel;
class QPushButton;
class QPaintEvent;
class QPainter;

class RoadNetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoadNetworkWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void calculateShortestPath();

private:
    // Graph Data Structure
    QVector<QVector<int>> graph;

    // Node positions for visualization
    QVector<QPointF> nodePositions;

    // Shortest path
    QVector<int> shortestPath;

    // UI Components
    QComboBox *sourceCombo;
    QComboBox *destinationCombo;

    QLabel *resultLabel;
    QPushButton *findRouteButton;

    int routeDistance = 0;

    void initializeGraph();
    void drawGraph(QPainter &painter);
};

#endif // ROADNETWORKWIDGET_H