#include "emergencyroutingwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QFrame>
#include <QFont>
#include <QTableWidgetItem>

#include <limits>
#include <algorithm>

// ================= CONSTRUCTOR =================

EmergencyRoutingWidget::EmergencyRoutingWidget(QWidget *parent)
    : QWidget(parent)
{
    locations = {
        "North Gate",
        "South Gate",
        "East Gate",
        "West Gate",
        "Central Junction",
        "Airport",
        "Hospital",
        "City Center"
    };

    setupGraph();
    setupUI();
    applyStyle();

    updateDashboard();
    updateWaitingTable();
}


// ================= GRAPH =================

void EmergencyRoutingWidget::setupGraph()
{
    graph.resize(locations.size());

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
}


void EmergencyRoutingWidget::addRoad(
    int from,
    int to,
    int distance)
{
    graph[from].push_back({to, distance});
    graph[to].push_back({from, distance});
}


// ================= DIJKSTRA ALGORITHM =================

bool EmergencyRoutingWidget::findShortestRoute(
    int source,
    int destination,
    int &totalDistance,
    QString &route)
{
    const int INF = std::numeric_limits<int>::max();

    int n = locations.size();

    QVector<int> distance(n, INF);
    QVector<int> parent(n, -1);

    using Node = std::pair<int, int>;

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
        > pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty())
    {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;

        pq.pop();

        if (currentDistance > distance[currentNode])
            continue;

        if (currentNode == destination)
            break;

        for (const auto &edge : graph[currentNode])
        {
            int neighbour = edge.first;
            int weight = edge.second;

            if (distance[currentNode] + weight <
                distance[neighbour])
            {
                distance[neighbour] =
                    distance[currentNode] + weight;

                parent[neighbour] = currentNode;

                pq.push({
                    distance[neighbour],
                    neighbour
                });
            }
        }
    }

    if (distance[destination] == INF)
        return false;

    totalDistance = distance[destination];

    QVector<int> path;

    int current = destination;

    while (current != -1)
    {
        path.prepend(current);
        current = parent[current];
    }

    route.clear();

    for (int i = 0; i < path.size(); i++)
    {
        route += locations[path[i]];

        if (i != path.size() - 1)
            route += "  →  ";
    }

    return true;
}


// ================= USER INTERFACE =================

void EmergencyRoutingWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(25, 20, 25, 20);
    mainLayout->setSpacing(18);

    // ================= HEADER =================

    QLabel *title = new QLabel("Emergency Routing Center");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: bold;"
        "color: white;"
        );

    QLabel *subtitle = new QLabel(
        "Priority-based emergency vehicle dispatch system"
        );

    subtitle->setStyleSheet(
        "font-size: 13px;"
        "color: #94A3B8;"
        );

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);


    // ================= DASHBOARD CARDS =================

    QHBoxLayout *cardsLayout = new QHBoxLayout();

    auto createCard = [](QString heading, QLabel **value)
    {
        QFrame *card = new QFrame();

        card->setObjectName("emergencyCard");

        QVBoxLayout *layout = new QVBoxLayout(card);

        QLabel *label = new QLabel(heading);

        label->setStyleSheet(
            "color: #94A3B8;"
            "font-size: 12px;"
            );

        *value = new QLabel("0");

        (*value)->setStyleSheet(
            "color: white;"
            "font-size: 25px;"
            "font-weight: bold;"
            );

        layout->addWidget(label);
        layout->addWidget(*value);

        return card;
    };

    cardsLayout->addWidget(
        createCard("Waiting Emergencies", &waitingValue)
        );

    cardsLayout->addWidget(
        createCard("Total Dispatched", &dispatchedValue)
        );

    cardsLayout->addWidget(
        createCard("Critical Emergencies", &criticalValue)
        );

    cardsLayout->addWidget(
        createCard("Next Priority", &nextEmergencyValue)
        );

    mainLayout->addLayout(cardsLayout);


    // ================= EMERGENCY REQUEST FORM =================

    QGroupBox *requestGroup =
        new QGroupBox("Create Emergency Request");

    QGridLayout *formLayout = new QGridLayout(requestGroup);

    vehicleCombo = new QComboBox();

    vehicleCombo->addItems({
        "Ambulance",
        "Fire Truck"
    });

    sourceCombo = new QComboBox();
    destinationCombo = new QComboBox();

    sourceCombo->addItems(locations);
    destinationCombo->addItems(locations);

    destinationCombo->setCurrentIndex(6);

    priorityCombo = new QComboBox();

    priorityCombo->addItems({
        "Critical",
        "High"
    });

    formLayout->addWidget(new QLabel("Vehicle Type"), 0, 0);
    formLayout->addWidget(vehicleCombo, 0, 1);

    formLayout->addWidget(new QLabel("Source"), 1, 0);
    formLayout->addWidget(sourceCombo, 1, 1);

    formLayout->addWidget(new QLabel("Destination"), 2, 0);
    formLayout->addWidget(destinationCombo, 2, 1);

    formLayout->addWidget(new QLabel("Priority"), 3, 0);
    formLayout->addWidget(priorityCombo, 3, 1);


    // ================= BUTTONS =================

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    addEmergencyButton =
        new QPushButton("Add Emergency");

    dispatchButton =
        new QPushButton("Dispatch Next");

    clearHistoryButton =
        new QPushButton("Clear History");

    addEmergencyButton->setObjectName("addButton");
    dispatchButton->setObjectName("dispatchButton");
    clearHistoryButton->setObjectName("clearButton");

    buttonLayout->addWidget(addEmergencyButton);
    buttonLayout->addWidget(dispatchButton);
    buttonLayout->addWidget(clearHistoryButton);

    formLayout->addLayout(buttonLayout, 4, 0, 1, 2);

    mainLayout->addWidget(requestGroup);


    // ================= ROUTE RESULT =================

    QGroupBox *routeGroup =
        new QGroupBox("Shortest Route Information");

    QVBoxLayout *routeLayout = new QVBoxLayout(routeGroup);

    routeResultLabel = new QLabel(
        "Add an emergency request to calculate its shortest route."
        );

    routeResultLabel->setWordWrap(true);

    routeResultLabel->setStyleSheet(
        "color: #38BDF8;"
        "font-size: 13px;"
        "padding: 8px;"
        );

    routeLayout->addWidget(routeResultLabel);

    mainLayout->addWidget(routeGroup);


    // ================= WAITING LIST =================

    QGroupBox *waitingGroup =
        new QGroupBox("Emergency Priority Queue");

    QVBoxLayout *waitingLayout =
        new QVBoxLayout(waitingGroup);

    waitingTable = new QTableWidget();

    waitingTable->setColumnCount(6);

    waitingTable->setHorizontalHeaderLabels({
        "ID",
        "Vehicle",
        "Source",
        "Destination",
        "Priority",
        "Distance"
    });

    waitingTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    waitingTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    waitingTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    waitingTable->setMinimumHeight(180);

    waitingLayout->addWidget(waitingTable);

    mainLayout->addWidget(waitingGroup);


    // ================= DISPATCH HISTORY =================

    QGroupBox *historyGroup =
        new QGroupBox("Dispatch History");

    QVBoxLayout *historyLayout =
        new QVBoxLayout(historyGroup);

    historyTable = new QTableWidget();

    historyTable->setColumnCount(5);

    historyTable->setHorizontalHeaderLabels({
        "ID",
        "Vehicle",
        "Route",
        "Priority",
        "Status"
    });

    historyTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    historyTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    historyTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    historyTable->setMinimumHeight(150);

    historyLayout->addWidget(historyTable);

    mainLayout->addWidget(historyGroup);


    // ================= STATUS =================

    statusLabel = new QLabel(
        "System Ready | Waiting for emergency requests"
        );

    statusLabel->setStyleSheet(
        "color: #22C55E;"
        "font-size: 12px;"
        "padding: 5px;"
        );

    mainLayout->addWidget(statusLabel);


    // ================= CONNECTIONS =================

    connect(
        addEmergencyButton,
        &QPushButton::clicked,
        this,
        &EmergencyRoutingWidget::addEmergency
        );

    connect(
        dispatchButton,
        &QPushButton::clicked,
        this,
        &EmergencyRoutingWidget::dispatchNextEmergency
        );

    connect(
        clearHistoryButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            historyTable->setRowCount(0);

            statusLabel->setText(
                "Dispatch history cleared."
                );
        }
        );
}


// ================= ADD EMERGENCY =================

void EmergencyRoutingWidget::addEmergency()
{
    int source = sourceCombo->currentIndex();
    int destination = destinationCombo->currentIndex();

    if (source == destination)
    {
        QMessageBox::warning(
            this,
            "Invalid Route",
            "Source and destination cannot be the same."
            );

        return;
    }

    EmergencyRequest request;

    request.id = nextRequestId++;

    request.vehicleType = vehicleCombo->currentText();

    request.source = source;
    request.destination = destination;

    request.priority = priorityCombo->currentText();

    request.priorityRank =
        (request.priority == "Critical") ? 0 : 1;

    request.sequence = sequenceNumber++;

    if (!findShortestRoute(
            source,
            destination,
            request.distance,
            request.route))
    {
        QMessageBox::warning(
            this,
            "Route Error",
            "No route found between selected locations."
            );

        return;
    }

    emergencyQueue.push(request);

    routeResultLabel->setText(
        "<b>Vehicle:</b> " + request.vehicleType +
        "<br><b>Route:</b> " + request.route +
        "<br><b>Distance:</b> " +
        QString::number(request.distance) +
        " units"
        );

    statusLabel->setText(
        request.priority + " emergency added successfully."
        );

    updateWaitingTable();
    updateDashboard();
}


// ================= DISPATCH NEXT EMERGENCY =================

void EmergencyRoutingWidget::dispatchNextEmergency()
{
    if (emergencyQueue.empty())
    {
        QMessageBox::information(
            this,
            "No Emergencies",
            "There are no pending emergency requests."
            );

        return;
    }

    EmergencyRequest request = emergencyQueue.top();

    emergencyQueue.pop();

    totalDispatched++;

    int row = historyTable->rowCount();

    historyTable->insertRow(row);

    historyTable->setItem(
        row, 0,
        new QTableWidgetItem(
            QString::number(request.id)
            )
        );

    historyTable->setItem(
        row, 1,
        new QTableWidgetItem(request.vehicleType)
        );

    historyTable->setItem(
        row, 2,
        new QTableWidgetItem(request.route)
        );

    historyTable->setItem(
        row, 3,
        new QTableWidgetItem(request.priority)
        );

    historyTable->setItem(
        row, 4,
        new QTableWidgetItem("Dispatched")
        );

    routeResultLabel->setText(
        "<b>Dispatched Vehicle:</b> " +
        request.vehicleType +
        "<br><b>Route:</b> " +
        request.route +
        "<br><b>Distance:</b> " +
        QString::number(request.distance) +
        " units"
        );

    statusLabel->setText(
        request.vehicleType +
        " dispatched successfully!"
        );

    updateWaitingTable();
    updateDashboard();
}


// ================= UPDATE WAITING TABLE =================

void EmergencyRoutingWidget::updateWaitingTable()
{
    waitingTable->setRowCount(0);

    auto tempQueue = emergencyQueue;

    while (!tempQueue.empty())
    {
        EmergencyRequest request = tempQueue.top();

        tempQueue.pop();

        int row = waitingTable->rowCount();

        waitingTable->insertRow(row);

        waitingTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(request.id)
                )
            );

        waitingTable->setItem(
            row, 1,
            new QTableWidgetItem(request.vehicleType)
            );

        waitingTable->setItem(
            row, 2,
            new QTableWidgetItem(
                locations[request.source]
                )
            );

        waitingTable->setItem(
            row, 3,
            new QTableWidgetItem(
                locations[request.destination]
                )
            );

        waitingTable->setItem(
            row, 4,
            new QTableWidgetItem(request.priority)
            );

        waitingTable->setItem(
            row, 5,
            new QTableWidgetItem(
                QString::number(request.distance)
                )
            );

        if (request.priority == "Critical")
        {
            for (int col = 0; col < 6; col++)
            {
                waitingTable->item(row, col)->setForeground(
                    QColor("#F87171")
                    );
            }
        }
    }
}


// ================= UPDATE DASHBOARD =================

void EmergencyRoutingWidget::updateDashboard()
{
    int waiting = static_cast<int>(emergencyQueue.size());

    int critical = 0;

    auto tempQueue = emergencyQueue;

    while (!tempQueue.empty())
    {
        if (tempQueue.top().priority == "Critical")
            critical++;

        tempQueue.pop();
    }

    waitingValue->setText(QString::number(waiting));

    dispatchedValue->setText(
        QString::number(totalDispatched)
        );

    criticalValue->setText(QString::number(critical));

    if (emergencyQueue.empty())
    {
        nextEmergencyValue->setText("None");
    }
    else
    {
        nextEmergencyValue->setText(
            emergencyQueue.top().priority
            );
    }
}


// ================= STYLING =================

void EmergencyRoutingWidget::applyStyle()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #0B1120;
            color: #E2E8F0;
            font-family: Segoe UI;
            font-size: 13px;
        }

        QGroupBox {
            border: 1px solid #334155;
            border-radius: 9px;
            margin-top: 12px;
            padding: 12px;
            font-weight: bold;
            color: #38BDF8;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 5px;
        }

        QFrame#emergencyCard {
            background-color: #172033;
            border: 1px solid #334155;
            border-radius: 10px;
        }

        QComboBox {
            background-color: #172033;
            border: 1px solid #475569;
            border-radius: 5px;
            padding: 8px;
            color: white;
        }

        QComboBox QAbstractItemView {
            background-color: #172033;
            color: white;
            selection-background-color: #2563EB;
        }

        QPushButton {
            background-color: #334155;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 15px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #475569;
        }

        QPushButton#addButton {
            background-color: #2563EB;
        }

        QPushButton#addButton:hover {
            background-color: #1D4ED8;
        }

        QPushButton#dispatchButton {
            background-color: #059669;
        }

        QPushButton#dispatchButton:hover {
            background-color: #047857;
        }

        QPushButton#clearButton {
            background-color: #B45309;
        }

        QTableWidget {
            background-color: #111827;
            alternate-background-color: #172033;
            border: 1px solid #334155;
            gridline-color: #334155;
            border-radius: 5px;
        }

        QHeaderView::section {
            background-color: #1E293B;
            color: #38BDF8;
            padding: 8px;
            border: none;
            font-weight: bold;
        }

        QTableWidget::item {
            padding: 5px;
        }

        QTableWidget::item:selected {
            background-color: #1D4ED8;
        }
    )");
}