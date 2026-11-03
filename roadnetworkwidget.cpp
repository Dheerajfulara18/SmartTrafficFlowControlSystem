#include "roadnetworkwidget.h"

#include <QPainter>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QFrame>
#include <QSizePolicy>

#include <functional>

// =====================================================
// SEPARATE GRAPH CANVAS
// =====================================================

class RoadGraphCanvas : public QWidget
{
public:

    explicit RoadGraphCanvas(
        std::function<void(QPainter &)> callback,
        QWidget *parent = nullptr
        )
        : QWidget(parent),
        drawCallback(callback)
    {
        setMinimumSize(450, 380);

        setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
            );

        setStyleSheet(
            "background-color: #0B1120;"
            "border: 1px solid #334155;"
            "border-radius: 12px;"
            );
    }

protected:

    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.fillRect(
            rect(),
            QColor("#0B1120")
            );

        if (drawCallback)
            drawCallback(painter);
    }

private:

    std::function<void(QPainter &)> drawCallback;
};


// =====================================================
// CONSTRUCTOR
// =====================================================

RoadNetworkWidget::RoadNetworkWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(750, 550);

    initializeGraph();

    // MAIN LAYOUT

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(20, 18, 20, 18);
    mainLayout->setSpacing(14);


    // =================================================
    // HEADING
    // =================================================

    QLabel *heading = new QLabel("ROAD NETWORK MANAGEMENT");

    heading->setStyleSheet(
        "font-size: 23px;"
        "font-weight: 800;"
        "color: #38BDF8;"
        "border: none;"
        );

    mainLayout->addWidget(heading);


    // =================================================
    // SHORTEST ROUTE SECTION
    // =================================================

    QGroupBox *routeBox = new QGroupBox("SHORTEST ROUTE");

    routeBox->setStyleSheet(
        "QGroupBox {"
        "color: #38BDF8;"
        "font-weight: bold;"
        "border: 1px solid #334155;"
        "border-radius: 9px;"
        "margin-top: 10px;"
        "padding-top: 12px;"
        "}"
        "QGroupBox::title {"
        "subcontrol-origin: margin;"
        "left: 12px;"
        "padding: 0 5px;"
        "}"
        );

    QGridLayout *routeLayout = new QGridLayout(routeBox);

    routeLayout->setSpacing(10);

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

    sourceCombo = new QComboBox;
    destinationCombo = new QComboBox;

    sourceCombo->addItems(locations);
    destinationCombo->addItems(locations);

    sourceCombo->setCurrentIndex(0);
    destinationCombo->setCurrentIndex(6);

    QString comboStyle =
        "QComboBox {"
        "background-color: #1E293B;"
        "color: white;"
        "padding: 8px;"
        "border: 1px solid #475569;"
        "border-radius: 6px;"
        "}"
        "QComboBox QAbstractItemView {"
        "background-color: #1E293B;"
        "color: white;"
        "selection-background-color: #0369A1;"
        "}";

    sourceCombo->setStyleSheet(comboStyle);
    destinationCombo->setStyleSheet(comboStyle);

    routeLayout->addWidget(
        new QLabel("SOURCE LOCATION"),
        0, 0
        );

    routeLayout->addWidget(
        sourceCombo,
        1, 0
        );

    routeLayout->addWidget(
        new QLabel("DESTINATION"),
        0, 1
        );

    routeLayout->addWidget(
        destinationCombo,
        1, 1
        );

    findRouteButton = new QPushButton(
        "FIND SHORTEST ROUTE"
        );

    findRouteButton->setStyleSheet(
        "QPushButton {"
        "background-color: #0369A1;"
        "color: white;"
        "padding: 11px;"
        "border-radius: 7px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #0284C7;"
        "}"
        );

    routeLayout->addWidget(
        findRouteButton,
        2, 0, 1, 2
        );

    mainLayout->addWidget(routeBox);


    // =================================================
    // MIDDLE SECTION
    // =================================================

    QHBoxLayout *middleLayout = new QHBoxLayout;

    middleLayout->setSpacing(15);


    // =================================================
    // ADD ROAD PANEL
    // =================================================

    QGroupBox *addRoadBox = new QGroupBox("ADD NEW ROAD");

    addRoadBox->setMinimumWidth(190);
    addRoadBox->setMaximumWidth(260);

    addRoadBox->setStyleSheet(
        "QGroupBox {"
        "color: #22C55E;"
        "font-weight: bold;"
        "border: 1px solid #334155;"
        "border-radius: 9px;"
        "margin-top: 10px;"
        "padding-top: 12px;"
        "}"
        "QGroupBox::title {"
        "subcontrol-origin: margin;"
        "left: 12px;"
        "padding: 0 5px;"
        "}"
        );

    QVBoxLayout *addRoadLayout = new QVBoxLayout(addRoadBox);

    addRoadLayout->setSpacing(12);

    QLabel *fromLabel = new QLabel("FROM LOCATION");

    addFromCombo = new QComboBox;
    addFromCombo->addItems(locations);
    addFromCombo->setStyleSheet(comboStyle);

    QLabel *toLabel = new QLabel("TO LOCATION");

    addToCombo = new QComboBox;
    addToCombo->addItems(locations);
    addToCombo->setCurrentIndex(1);
    addToCombo->setStyleSheet(comboStyle);

    QLabel *distanceLabel = new QLabel("ROAD DISTANCE");

    distanceInput = new QSpinBox;

    distanceInput->setRange(1, 1000);
    distanceInput->setValue(5);
    distanceInput->setSuffix(" km");

    distanceInput->setStyleSheet(
        "QSpinBox {"
        "background-color: #1E293B;"
        "color: white;"
        "padding: 8px;"
        "border: 1px solid #475569;"
        "border-radius: 6px;"
        "}"
        );

    addRoadButton = new QPushButton("ADD ROAD");

    addRoadButton->setStyleSheet(
        "QPushButton {"
        "background-color: #15803D;"
        "color: white;"
        "padding: 11px;"
        "border-radius: 7px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #16A34A;"
        "}"
        );

    addRoadLayout->addWidget(fromLabel);
    addRoadLayout->addWidget(addFromCombo);

    addRoadLayout->addWidget(toLabel);
    addRoadLayout->addWidget(addToCombo);

    addRoadLayout->addWidget(distanceLabel);
    addRoadLayout->addWidget(distanceInput);

    addRoadLayout->addSpacing(10);

    addRoadLayout->addWidget(addRoadButton);

    addRoadLayout->addStretch();


    // =================================================
    // GRAPH CANVAS
    // =================================================

    graphCanvas = new RoadGraphCanvas(
        [this](QPainter &painter)
        {
            drawGraph(painter);
        }
        );

    middleLayout->addWidget(addRoadBox, 0);
    middleLayout->addWidget(graphCanvas, 1);

    mainLayout->addLayout(middleLayout, 1);


    // =================================================
    // RESULT LABEL
    // =================================================

    resultLabel = new QLabel(
        "Select locations to calculate the shortest route."
        );

    resultLabel->setWordWrap(true);

    resultLabel->setStyleSheet(
        "background-color: #172554;"
        "color: #34D399;"
        "font-size: 13px;"
        "font-weight: bold;"
        "padding: 12px;"
        "border-radius: 7px;"
        );

    mainLayout->addWidget(resultLabel);


    // =================================================
    // BUTTON CONNECTIONS
    // =================================================

    connect(
        findRouteButton,
        &QPushButton::clicked,
        this,
        &RoadNetworkWidget::calculateShortestPath
        );

    connect(
        addRoadButton,
        &QPushButton::clicked,
        this,
        &RoadNetworkWidget::addRoad
        );
}


// =====================================================
// INITIALIZE GRAPH
// =====================================================

void RoadNetworkWidget::initializeGraph()
{
    const int N = 8;

    graph = QVector<QVector<int>>(
        N,
        QVector<int>(N, 0)
        );

    // Existing roads preserved

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

    shortestPath.clear();
}


// =====================================================
// DIJKSTRA ALGORITHM
// =====================================================

void RoadNetworkWidget::calculateShortestPath()
{
    int source = sourceCombo->currentIndex();

    int destination = destinationCombo->currentIndex();

    if (source == destination)
    {
        QMessageBox::warning(
            this,
            "Invalid Route",
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

        graphCanvas->update();

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
        "SHORTEST ROUTE: " + pathText +
        "     |     DISTANCE: " +
        QString::number(routeDistance) + " km"
        );

    graphCanvas->update();
}


// =====================================================
// ADD ROAD
// =====================================================

void RoadNetworkWidget::addRoad()
{
    int source = addFromCombo->currentIndex();

    int destination = addToCombo->currentIndex();

    int distance = distanceInput->value();


    if (source == destination)
    {
        QMessageBox::warning(
            this,
            "Invalid Road",
            "Source and destination cannot be same!"
            );

        return;
    }


    if (graph[source][destination] > 0)
    {
        QMessageBox::warning(
            this,
            "Road Already Exists",
            "A road already exists between these locations!"
            );

        return;
    }


    addRoadToGraph(
        source,
        destination,
        distance
        );

    shortestPath.clear();

    resultLabel->setText(
        "New road successfully added! Calculate shortest route again."
        );

    graphCanvas->update();

    QMessageBox::information(
        this,
        "Road Added",
        "New road has been added successfully!"
        );
}


// =====================================================
// ADD ROAD TO GRAPH
// =====================================================

void RoadNetworkWidget::addRoadToGraph(
    int source,
    int destination,
    int distance
    )
{
    graph[source][destination] = distance;

    graph[destination][source] = distance;
}


// =====================================================
// DRAW GRAPH
// =====================================================

void RoadNetworkWidget::drawGraph(QPainter &painter)
{
    const int N = graph.size();

    const qreal w = graphCanvas->width();

    const qreal h = graphCanvas->height();


    // Responsive node positions

    nodePositions = {

    QPointF(w * 0.50, h * 0.13),  // North

        QPointF(w * 0.50, h * 0.87),  // South

        QPointF(w * 0.89, h * 0.43),  // East

        QPointF(w * 0.11, h * 0.43),  // West

        QPointF(w * 0.50, h * 0.49),  // Central

        QPointF(w * 0.79, h * 0.80),  // Airport

        QPointF(w * 0.64, h * 0.59),  // Hospital

        QPointF(w * 0.19, h * 0.80)   // City Center

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
                if (
                    (shortestPath[k] == i &&
                     shortestPath[k + 1] == j)

                    ||

                    (shortestPath[k] == j &&
                     shortestPath[k + 1] == i)
                    )
                {
                    highlighted = true;
                }
            }


            painter.setPen(
                QPen(
                    highlighted
                        ? QColor("#22C55E")
                        : QColor("#64748B"),

                    highlighted ? 4 : 2
                    )
                );

            painter.drawLine(
                nodePositions[i],
                nodePositions[j]
                );


            // Distance label

            QPointF middle =
                (nodePositions[i] + nodePositions[j]) / 2.0;

            painter.setPen(
                QColor("#FBBF24")
                );

            painter.drawText(
                middle + QPointF(5, -6),

                QString::number(graph[i][j]) + " km"
                );
        }
    }
}


// Draw intersections

for (int i = 0; i < N; i++)
{
    bool onRoute = shortestPath.contains(i);

    painter.setPen(
        QPen(Qt::white, 2)
        );

    painter.setBrush(
        onRoute
            ? QColor("#16A34A")
            : QColor("#0284C7")
        );

    painter.drawEllipse(
        nodePositions[i],
        15,
        15
        );


    // Node labels

    painter.setPen(Qt::white);

    QFont font = painter.font();

    font.setPointSize(8);

    painter.setFont(font);

    painter.drawText(
        QRectF(
            nodePositions[i].x() - 55,
            nodePositions[i].y() + 20,
            110,
            20
            ),

        Qt::AlignCenter,

        names[i]
        );
}
}