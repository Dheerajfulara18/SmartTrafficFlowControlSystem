#include "roadnetworkwidget.h"

#include <QPainter>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QPen>
#include <QBrush>
#include <QFont>

// Constructor
RoadNetworkWidget::RoadNetworkWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(750, 550);

    // Initialize graph
    initializeGraph();

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Heading
    QLabel *heading = new QLabel("ROAD NETWORK GRAPH");

    heading->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 800;"
        "color: #38BDF8;"
        );

    // Source and Destination
    QHBoxLayout *selectionLayout = new QHBoxLayout;

    sourceCombo = new QComboBox;
    destinationCombo = new QComboBox;

    QStringList locations = {
        "North Gate",
        "South Gate",
        "East Gate",
        "West Gate",
        "Central Junction",
        "Airport",
        "Hospital",
        "City Center"
    };

    sourceCombo->addItems(locations);
    destinationCombo->addItems(locations);

    sourceCombo->setCurrentIndex(0);
    destinationCombo->setCurrentIndex(6);

    sourceCombo->setStyleSheet(
        "padding: 8px;"
        "background-color: #1E293B;"
        "color: white;"
        );

    destinationCombo->setStyleSheet(
        "padding: 8px;"
        "background-color: #1E293B;"
        "color: white;"
        );

    selectionLayout->addWidget(new QLabel("SOURCE:"));
    selectionLayout->addWidget(sourceCombo);

    selectionLayout->addWidget(new QLabel("DESTINATION:"));
    selectionLayout->addWidget(destinationCombo);

    // Find Route Button
    findRouteButton = new QPushButton("FIND SHORTEST ROUTE");

    findRouteButton->setStyleSheet(
        "QPushButton {"
        "background-color: #0369A1;"
        "color: white;"
        "padding: 12px;"
        "border-radius: 7px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #0284C7;"
        "}"
        );

    // Result Label
    resultLabel = new QLabel("Select source and destination.");

    resultLabel->setStyleSheet(
        "color: #34D399;"
        "font-size: 13px;"
        "font-weight: bold;"
        );

    // Add widgets
    mainLayout->addWidget(heading);
    mainLayout->addLayout(selectionLayout);
    mainLayout->addWidget(findRouteButton);
    mainLayout->addWidget(resultLabel);

    mainLayout->addStretch();

    // Button Connection
    connect(
        findRouteButton,
        &QPushButton::clicked,
        this,
        &RoadNetworkWidget::calculateShortestPath
        );
}


// Initialize Graph
void RoadNetworkWidget::initializeGraph()
{
    const int N = 8;

    graph = QVector<QVector<int>>(
        N, QVector<int>(N, 0)
        );

    // Undirected weighted graph

    auto addRoad = [this](int u, int v, int distance)
    {
        graph[u][v] = distance;
        graph[v][u] = distance;
    };

    addRoad(0, 1, 10);
    addRoad(0, 4, 5);
    addRoad(0, 3, 15);

    addRoad(1, 2, 8);
    addRoad(1, 4, 12);

    addRoad(2, 4, 7);
    addRoad(2, 5, 10);

    addRoad(3, 4, 6);
    addRoad(3, 7, 9);

    addRoad(4, 5, 4);
    addRoad(4, 6, 3);

    addRoad(5, 6, 8);
    addRoad(6, 7, 5);

    // Node positions
    nodePositions = {
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0),
        QPointF(0, 0)
    };

    shortestPath.clear();
}


// Dijkstra Algorithm
void RoadNetworkWidget::calculateShortestPath()
{
    int source = sourceCombo->currentIndex();
    int destination = destinationCombo->currentIndex();

    if (source == destination)
    {
        resultLabel->setText(
            "Source and destination cannot be same!"
            );

        return;
    }

    const int N = graph.size();
    const int INF = 1000000000;

    QVector<int> distance(N, INF);
    QVector<int> parent(N, -1);
    QVector<bool> visited(N, false);

    distance[source] = 0;

    // Dijkstra
    for (int count = 0; count < N - 1; count++)
    {
        int u = -1;

        for (int i = 0; i < N; i++)
        {
            if (!visited[i] &&
                (u == -1 || distance[i] < distance[u]))
            {
                u = i;
            }
        }

        if (u == -1 || distance[u] == INF)
            break;

        visited[u] = true;

        for (int v = 0; v < N; v++)
        {
            if (graph[u][v] > 0 &&
                !visited[v] &&
                distance[u] + graph[u][v] < distance[v])
            {
                distance[v] =
                    distance[u] + graph[u][v];

                parent[v] = u;
            }
        }
    }

    // Check destination
    if (distance[destination] == INF)
    {
        resultLabel->setText("No route available.");
        shortestPath.clear();
        update();
        return;
    }

    // Reconstruct shortest path
    shortestPath.clear();

    int current = destination;

    while (current != -1)
    {
        shortestPath.prepend(current);
        current = parent[current];
    }

    routeDistance = distance[destination];

    QStringList names = {
        "North Gate",
        "South Gate",
        "East Gate",
        "West Gate",
        "Central Junction",
        "Airport",
        "Hospital",
        "City Center"
    };

    QString pathText;

    for (int i = 0; i < shortestPath.size(); i++)
    {
        pathText += names[shortestPath[i]];

        if (i != shortestPath.size() - 1)
            pathText += " → ";
    }

    resultLabel->setText(
        "Shortest Route: " + pathText +
        " | Distance: " +
        QString::number(routeDistance) + " km"
        );

    update();
}


// Draw Graph
void RoadNetworkWidget::drawGraph(QPainter &painter)
{
    const int N = graph.size();

    // Node positions
    nodePositions = {
        QPointF(width() / 2.0, 230),          // North
        QPointF(width() / 2.0, height() - 65),// South
        QPointF(width() - 100, 310),          // East
        QPointF(100, 310),                    // West
        QPointF(width() / 2.0, height() / 2.0),// Central
        QPointF(width() - 180,  height() - 130), // Airport
        QPointF(width() / 2.0 + 110,  height() / 2.0 + 70), // Hospital
        QPointF(150, height() - 130)           // City Center
    };

    QStringList names = {
        "North Gate",
        "South Gate",
        "East Gate",
        "West Gate",
        "Central",
        "Airport",
        "Hospital",
        "City Center"
    };

    // Draw roads
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (graph[i][j] > 0)
            {
                bool highlighted = false;

                for (int k = 0; k < shortestPath.size() - 1; k++)
                {
                    if ((shortestPath[k] == i &&
                         shortestPath[k + 1] == j) ||
                        (shortestPath[k] == j &&
                         shortestPath[k + 1] == i))
                    {
                        highlighted = true;
                    }
                }

                painter.setPen(
                    QPen(
                        highlighted ? QColor("#22C55E")
                                    : QColor("#64748B"),
                        highlighted ? 5 : 2
                        )
                    );

                painter.drawLine(
                    nodePositions[i],
                    nodePositions[j]
                    );

                QPointF middle =
                    (nodePositions[i] + nodePositions[j]) / 2.0;

                painter.setPen(QColor("#FBBF24"));
                painter.drawText(
                    middle + QPointF(5, -5),
                    QString::number(graph[i][j]) + " km"
                    );
            }
        }
    }

    // Draw intersections
    for (int i = 0; i < N; i++)
    {
        painter.setPen(QPen(Qt::white, 2));

        painter.setBrush(
            shortestPath.contains(i)
                ? QColor("#16A34A")
                : QColor("#0284C7")
            );

        painter.drawEllipse(
            nodePositions[i],
            17,
            17
            );

        painter.setPen(Qt::white);

        painter.drawText(
            nodePositions[i] + QPointF(-35, 35),
            names[i]
            );
    }
}


// Paint Event
void RoadNetworkWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor("#0B1120"));

    drawGraph(painter);
}