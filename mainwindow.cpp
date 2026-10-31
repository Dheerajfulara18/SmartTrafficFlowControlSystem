
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "traffic_engine.h"

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QRandomGenerator>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Smart Traffic Control System");
    setMinimumSize(1100, 720);
    resize(1400, 850);

    // ================= MAIN WINDOW =================

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    central->setStyleSheet(
        "QWidget {"
        "background-color: #0B1120;"
        "color: #E5E7EB;"
        "font-family: 'Segoe UI';"
        "}"
        );

    QHBoxLayout *root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ================= SIDEBAR =================

    QFrame *sidebar = new QFrame;
    sidebar->setFixedWidth(230);

    sidebar->setStyleSheet(
        "QFrame {"
        "background-color: #111827;"
        "border-right: 1px solid #253047;"
        "}"
        );

    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(18, 24, 18, 20);
    sideLayout->setSpacing(12);

    QLabel *brand = new QLabel("◈ TRAFFIC");

    brand->setStyleSheet(
        "font-size: 24px;"
        "font-weight: 800;"
        "color: #38BDF8;"
        "padding: 10px 4px 22px 4px;"
        "border: none;"
        );

    QLabel *subtitle = new QLabel("INTELLIGENT MOBILITY");

    subtitle->setStyleSheet(
        "font-size: 10px;"
        "color: #64748B;"
        "border: none;"
        "padding-bottom: 20px;"
        );

    sideLayout->addWidget(brand);
    sideLayout->addWidget(subtitle);

    auto createNavButton = [](const QString &text,
                              bool active = false)
    {
        QPushButton *button = new QPushButton(text);

        button->setMinimumHeight(46);
        button->setCursor(Qt::PointingHandCursor);

        QString background =
            active ? "#12314A" : "transparent";

        QString foreground =
            active ? "#38BDF8" : "#94A3B8";

        button->setStyleSheet(
            QString(
                "QPushButton {"
                "text-align: left;"
                "padding-left: 15px;"
                "border-radius: 9px;"
                "border: none;"
                "background-color: %1;"
                "color: %2;"
                "font-size: 13px;"
                "font-weight: 600;"
                "}"
                "QPushButton:hover {"
                "background-color: #1E293B;"
                "color: #F8FAFC;"
                "}"
                ).arg(background, foreground)
            );

        return button;
    };

    QPushButton *dashboardBtn =
        createNavButton("  ◈   Dashboard", true);

    QPushButton *simulationBtn =
        createNavButton("  ◉   Traffic Simulation");

    QPushButton *networkBtn =
        createNavButton("  ⌘   Road Network");

    QPushButton *routingBtn =
        createNavButton("  ➤   Emergency Routing");

    QPushButton *analyticsBtn =
        createNavButton("  ▥   Analytics");

    QPushButton *algorithmsBtn =
        createNavButton("  ⬡   DSA Visualizer");

    sideLayout->addWidget(dashboardBtn);
    sideLayout->addWidget(simulationBtn);
    sideLayout->addWidget(networkBtn);
    sideLayout->addWidget(routingBtn);
    sideLayout->addWidget(analyticsBtn);
    sideLayout->addWidget(algorithmsBtn);

    sideLayout->addStretch();

    QLabel *systemLabel = new QLabel(
        "SYSTEM STATUS\n\n"
        "●  C engine initialized\n"
        "●  Circular queues ready"
        );

    systemLabel->setStyleSheet(
        "background-color: #172033;"
        "color: #94A3B8;"
        "font-size: 11px;"
        "padding: 14px;"
        "border: 1px solid #29364B;"
        "border-radius: 10px;"
        );

    sideLayout->addWidget(systemLabel);

    QLabel *versionLabel =
        new QLabel("TRAFFIC CORE  •  v1.1");

    versionLabel->setAlignment(Qt::AlignCenter);

    versionLabel->setStyleSheet(
        "color: #475569;"
        "font-size: 10px;"
        "padding-top: 15px;"
        "border: none;"
        );

    sideLayout->addWidget(versionLabel);

    root->addWidget(sidebar);

    // ================= MAIN CONTENT =================

    QWidget *content = new QWidget;

    QVBoxLayout *contentLayout = new QVBoxLayout(content);

    contentLayout->setContentsMargins(28, 22, 28, 22);
    contentLayout->setSpacing(18);

    root->addWidget(content, 1);

    // ================= HEADER =================

    QHBoxLayout *header = new QHBoxLayout;

    QVBoxLayout *titleLayout = new QVBoxLayout;
    titleLayout->setSpacing(5);

    QLabel *pageTitle = new QLabel("Traffic Overview");

    pageTitle->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 800;"
        "color: #F8FAFC;"
        "border: none;"
        );

    QLabel *pageSubtitle = new QLabel(
        "Monitor, analyze and optimize urban traffic flow."
        );

    pageSubtitle->setStyleSheet(
        "font-size: 12px;"
        "color: #94A3B8;"
        "border: none;"
        );

    titleLayout->addWidget(pageTitle);
    titleLayout->addWidget(pageSubtitle);

    header->addLayout(titleLayout);
    header->addStretch();

    QLabel *liveBadge = new QLabel("●  SYSTEM ONLINE");

    liveBadge->setAlignment(Qt::AlignCenter);
    liveBadge->setMinimumHeight(38);

    liveBadge->setStyleSheet(
        "background-color: #12352E;"
        "color: #34D399;"
        "border: 1px solid #16634D;"
        "border-radius: 8px;"
        "padding: 0 14px;"
        "font-size: 11px;"
        "font-weight: 700;"
        );

    header->addWidget(liveBadge);

    contentLayout->addLayout(header);

    // ================= TRAFFIC ENGINE =================
    for (int i = 0; i < MAX_LANES; ++i) {
        queueInit(&lanes[i]);
    }

    tick = 0;
    nextVehicleId = 1;

    totalWaitTime = 0;
    completedVehicles = 0;

    verticalGreen = true;

    // ================= STATISTICS CARDS =================

    QGridLayout *statsGrid = new QGridLayout;
    statsGrid->setSpacing(14);

    auto createStatCard =
        [](const QString &title,
           const QString &value,
           const QString &description,
           const QString &accent,
           QLabel **valueLabel)
    {
        QFrame *card = new QFrame;

        card->setMinimumHeight(120);

        card->setStyleSheet(
            "QFrame {"
            "background-color: #111827;"
            "border: 1px solid #253047;"
            "border-radius: 13px;"
            "}"
            );

        QVBoxLayout *layout = new QVBoxLayout(card);

        layout->setContentsMargins(15, 12, 15, 12);
        layout->setSpacing(5);

        QLabel *heading = new QLabel(title);

        heading->setStyleSheet(
            "color: #94A3B8;"
            "font-size: 11px;"
            "border: none;"
            );

        QLabel *number = new QLabel(value);

        if (valueLabel != nullptr) {
            *valueLabel = number;
        }

        number->setStyleSheet(
            QString(
                "color: %1;"
                "font-size: 26px;"
                "font-weight: 800;"
                "border: none;"
                ).arg(accent)
            );

        QLabel *detail = new QLabel(description);

        detail->setStyleSheet(
            "color: #64748B;"
            "font-size: 10px;"
            "border: none;"
            );

        layout->addWidget(heading);
        layout->addWidget(number);
        layout->addWidget(detail);

        return card;
    };

    statsGrid->addWidget(
        createStatCard(
            "ACTIVE VEHICLES",
            "0",
            "Vehicles in queues",
            "#38BDF8",
            &activeVehiclesValue
            ),
        0, 0
        );

    statsGrid->addWidget(
        createStatCard(
            "AVG. WAIT TIME",
            "0 s",
            "Waiting vehicles",
            "#A78BFA",
            &avgWaitValue
            ),
        0, 1
        );

    statsGrid->addWidget(
        createStatCard(
            "CONGESTION",
            "0%",
            "Queue occupancy estimate",
            "#FBBF24",
            &congestionValue
            ),
        0, 2
        );

    statsGrid->addWidget(
        createStatCard(
            "EMERGENCIES",
            "0",
            "Emergency vehicles waiting",
            "#FB7185",
            &emergenciesValue
            ),
        0, 3
        );

    contentLayout->addLayout(statsGrid);

    // ================= INTERSECTION PANEL =================

    QFrame *intersectionPanel = new QFrame;

    intersectionPanel->setMinimumHeight(280);

    intersectionPanel->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    intersectionPanel->setStyleSheet(
        "QFrame {"
        "background-color: #111827;"
        "border: 1px solid #253047;"
        "border-radius: 14px;"
        "}"
        );

    QVBoxLayout *intersectionLayout =
        new QVBoxLayout(intersectionPanel);

    intersectionLayout->setContentsMargins(
        18, 14, 18, 16
        );

    intersectionLayout->setSpacing(10);

    QHBoxLayout *intersectionHeader =
        new QHBoxLayout;

    QLabel *intersectionTitle =
        new QLabel("INTERSECTION CONTROL");

    intersectionTitle->setStyleSheet(
        "font-size: 13px;"
        "font-weight: 800;"
        "color: #E2E8F0;"
        "border: none;"
        );

    QLabel *simulationStatus =
        new QLabel("●  SIMULATION STOPPED");

    simulationStatus->setStyleSheet(
        "color: #FBBF24;"
        "font-size: 10px;"
        "font-weight: 700;"
        "border: none;"
        );

    intersectionHeader->addWidget(intersectionTitle);
    intersectionHeader->addStretch();
    intersectionHeader->addWidget(simulationStatus);

    intersectionLayout->addLayout(intersectionHeader);

    QFrame *roadCanvas = new QFrame;

    roadCanvas->setMinimumHeight(170);

    roadCanvas->setStyleSheet(
        "background-color: #0B1220;"
        "border: 1px solid #263349;"
        "border-radius: 10px;"
        );

    QGridLayout *roadGrid = new QGridLayout(roadCanvas);

    roadGrid->setContentsMargins(10, 10, 10, 10);
    roadGrid->setSpacing(5);

    auto makeRoadLabel = [](const QString &text)
    {
        QLabel *label = new QLabel(text);

        label->setAlignment(Qt::AlignCenter);

        label->setStyleSheet(
            "background-color: #273244;"
            "color: #E2E8F0;"
            "border: 1px solid #3A4A60;"
            "border-radius: 5px;"
            "padding: 10px;"
            "font-size: 11px;"
            "font-weight: 700;"
            );

        return label;
    };

    QLabel *northRoad = makeRoadLabel("NORTH  ↑  RED");
    QLabel *southRoad = makeRoadLabel("SOUTH  ↓  RED");
    QLabel *westRoad = makeRoadLabel("←  WEST  RED");
    QLabel *eastRoad = makeRoadLabel("EAST  →  RED");

    QLabel *intersection =
        new QLabel("TRAFFIC\nINTERSECTION");

    intersection->setAlignment(Qt::AlignCenter);
    intersection->setMinimumSize(120, 85);

    intersection->setStyleSheet(
        "background-color: #334155;"
        "color: #38BDF8;"
        "border: 2px solid #475569;"
        "border-radius: 8px;"
        "font-size: 12px;"
        "font-weight: 800;"
        );

    roadGrid->addWidget(northRoad, 0, 1);
    roadGrid->addWidget(westRoad, 1, 0);
    roadGrid->addWidget(intersection, 1, 1);
    roadGrid->addWidget(eastRoad, 1, 2);
    roadGrid->addWidget(southRoad, 2, 1);

    intersectionLayout->addWidget(roadCanvas, 1);
    // NEW: Add animated vehicle intersection
    roadCanvas->hide();

    vehicleWidget = new VehicleWidget(intersectionPanel);
    intersectionLayout->addWidget(vehicleWidget, 1);

    contentLayout->addWidget(intersectionPanel, 1);

    // ================= SIMULATION CONTROLS =================

    QHBoxLayout *controls = new QHBoxLayout;

    controls->setSpacing(12);

    QLabel *controlHint =
        new QLabel("Simulation controls");

    controlHint->setStyleSheet(
        "color: #94A3B8;"
        "font-size: 12px;"
        "border: none;"
        );

    controls->addWidget(controlHint);
    controls->addStretch();

    auto makeControlButton =
        [](const QString &text,
           const QString &background)
    {
        QPushButton *button = new QPushButton(text);

        button->setMinimumSize(115, 42);
        button->setCursor(Qt::PointingHandCursor);

        button->setStyleSheet(
            QString(
                "QPushButton {"
                "background-color: %1;"
                "color: #F8FAFC;"
                "border: 1px solid #334155;"
                "border-radius: 8px;"
                "font-size: 12px;"
                "font-weight: 700;"
                "padding: 8px;"
                "}"
                "QPushButton:hover {"
                "border: 1px solid #38BDF8;"
                "}"
                "QPushButton:pressed {"
                "background-color: #334155;"
                "}"
                ).arg(background)
            );

        return button;
    };

    QPushButton *startButton =
        makeControlButton("▶  Start", "#0369A1");

    QPushButton *pauseButton =
        makeControlButton("Ⅱ  Pause", "#253047");

    QPushButton *resetButton =
        makeControlButton("↻  Reset", "#253047");

    controls->addWidget(startButton);
    controls->addWidget(pauseButton);
    controls->addWidget(resetButton);

    contentLayout->addLayout(controls);

    // ================= SIMULATION TIMER =================

    simulationTimer = new QTimer(this);

    simulationTimer->setInterval(1000);

    auto setSimulationStatus =
        [simulationStatus](const QString &text,
                           const QString &color)
    {
        simulationStatus->setText(text);

        simulationStatus->setStyleSheet(
            QString(
                "color: %1;"
                "font-size: 10px;"
                "font-weight: 700;"
                "border: none;"
                ).arg(color)
            );
    };

    auto setRoadSignal =
        [](QLabel *road,
           const QString &direction,
           bool green)
    {
        QString state = green ? "GREEN" : "RED";

        road->setText(direction + "  " + state);

        road->setStyleSheet(
            QString(
                "background-color: %1;"
                "color: #F8FAFC;"
                "border: 1px solid %2;"
                "border-radius: 5px;"
                "padding: 10px;"
                "font-size: 11px;"
                "font-weight: 700;"
                ).arg(
                    green ? "#065F46" : "#7F1D1D",
                    green ? "#10B981" : "#EF4444"
                    )
            );
    };

    auto updateStatistics = [&]()
    {
        int totalVehicles = 0;
        int emergencyVehicles = 0;
        long long waitingTime = 0;

        for (int lane = 0; lane < MAX_LANES; ++lane) {

            totalVehicles += queueSize(&lanes[lane]);

            for (int j = 0; j < lanes[lane].count; ++j) {

                int index =
                    (lanes[lane].front + j) % MAX_VEHICLES;

                const Vehicle &vehicle =
                    lanes[lane].items[index];

                waitingTime += vehicle.waitTime;

                if (vehicle.type == VEHICLE_AMBULANCE ||
                    vehicle.type == VEHICLE_FIRE_TRUCK) {
                    emergencyVehicles++;
                }
            }
        }

        activeVehiclesValue->setText(
            QString::number(totalVehicles)
            );

        double averageWait = 0.0;

        if (totalVehicles > 0) {
            averageWait =
                static_cast<double>(waitingTime) /
                totalVehicles;
        }

        avgWaitValue->setText(
            QString::number(averageWait, 'f', 1) + " s"
            );

        int capacity = MAX_LANES * MAX_VEHICLES;

        int congestion =
            capacity > 0
                ? totalVehicles * 100 / capacity
                : 0;

        congestionValue->setText(
            QString::number(congestion) + "%"
            );

        emergenciesValue->setText(
            QString::number(emergencyVehicles)
            );
    };
    // NEW: Synchronize vehicle signals with traffic engine
    connect(
        simulationTimer,
        &QTimer::timeout,
        this,
        [this]()
        {
            vehicleWidget->setTrafficSignals(verticalGreen);
        }
        );

    // ================= SIMULATION STEP =================

    connect(
        simulationTimer,
        &QTimer::timeout,
        this,
        [this, northRoad, southRoad, westRoad, eastRoad,
         updateStatistics, setRoadSignal]()
        {
            tick++;

            // Generate vehicles in each lane.
            for (int lane = 0; lane < MAX_LANES; ++lane) {
                if (QRandomGenerator::global()->bounded(100) < 35 &&
                    !queueIsFull(&lanes[lane])) {

                    Vehicle vehicle{};

                    vehicle.id = nextVehicleId++;
                    vehicle.lane = lane;
                    vehicle.arrivalTime = tick;
                    vehicle.waitTime = 0;

                    int type =
                        QRandomGenerator::global()->bounded(100);

                    if (type < 3) {
                        vehicle.type = VEHICLE_AMBULANCE;
                    } else if (type < 5) {
                        vehicle.type = VEHICLE_FIRE_TRUCK;
                    } else if (type < 30) {
                        vehicle.type = VEHICLE_BIKE;
                    } else if (type < 40) {
                        vehicle.type = VEHICLE_BUS;
                    } else {
                        vehicle.type = VEHICLE_CAR;
                    }

                    enqueue(&lanes[lane], vehicle);
                }
            }

            // Increase waiting time.
            for (int lane = 0; lane < MAX_LANES; ++lane) {
                for (int j = 0; j < lanes[lane].count; ++j) {
                    int index =
                        (lanes[lane].front + j) % MAX_VEHICLES;

                    lanes[lane].items[index].waitTime++;
                }
            }

            // Alternate traffic signal phases.
            verticalGreen = (tick % 10) < 5;

            setRoadSignal(northRoad, "NORTH ↑", verticalGreen);
            setRoadSignal(southRoad, "SOUTH ↓", verticalGreen);
            setRoadSignal(westRoad, "← WEST", !verticalGreen);
            setRoadSignal(eastRoad, "EAST →", !verticalGreen);

            // Serve one vehicle from each green lane.
            for (int lane = 0; lane < MAX_LANES; ++lane) {
                bool laneIsGreen =
                    verticalGreen
                        ? (lane == 0 || lane == 2)
                        : (lane == 1 || lane == 3);

                if (laneIsGreen && !queueIsEmpty(&lanes[lane])) {
                    Vehicle vehicle{};

                    if (dequeue(&lanes[lane], &vehicle)) {
                        totalWaitTime += vehicle.waitTime;
                        completedVehicles++;
                    }
                }
            }

            updateStatistics();
        });
    // ================= BUTTON CONNECTIONS =================

    connect(
        startButton,
        &QPushButton::clicked,
        this,
        [this, setSimulationStatus]()
        {
            if (this->simulationTimer != nullptr &&
                !this->simulationTimer->isActive()) {
                this->simulationTimer->start();
            }

            setSimulationStatus(
                "●  SIMULATION RUNNING",
                "#34D399"
                );
        }
        );
    // NEW: Start vehicle animation
    connect(
        startButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            vehicleWidget->startAnimation();
        }
        );

    connect(
        pauseButton,
        &QPushButton::clicked,
        this,
        [this, setSimulationStatus]()
        {
            if (this->simulationTimer != nullptr) {
                this->simulationTimer->stop();
            }

            setSimulationStatus(
                "Ⅱ  SIMULATION PAUSED",
                "#FBBF24"
                );
        }
        );
    // NEW: Pause vehicle animation
    connect(
        pauseButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            vehicleWidget->pauseAnimation();
        }
        );

    connect(
        resetButton,
        &QPushButton::clicked,
        this,
        [this, northRoad, southRoad, westRoad, eastRoad,
         setSimulationStatus, setRoadSignal, updateStatistics]()
        {
            if (this->simulationTimer != nullptr) {
                this->simulationTimer->stop();
            }

            for (int lane = 0; lane < MAX_LANES; ++lane) {
                queueInit(&lanes[lane]);
            }

            tick = 0;
            nextVehicleId = 1;
            totalWaitTime = 0;
            completedVehicles = 0;
            verticalGreen = true;

            setRoadSignal(northRoad, "NORTH ↑", true);
            setRoadSignal(southRoad, "SOUTH ↓", true);
            setRoadSignal(westRoad, "← WEST", false);
            setRoadSignal(eastRoad, "EAST →", false);

            updateStatistics();

            setSimulationStatus(
                "●  SIMULATION STOPPED",
                "#FBBF24"
                );
        }
        );
    // NEW: Reset vehicle animation
    connect(
        resetButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            vehicleWidget->pauseAnimation();
            vehicleWidget->resetVehicles();
            vehicleWidget->setTrafficSignals(true);
        }
        );

    // Initialize display.
    updateStatistics();

    // Initialize display.
    updateStatistics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

