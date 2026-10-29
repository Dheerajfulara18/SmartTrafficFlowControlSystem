
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QSizePolicy>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Smart Traffic Control System");
    setMinimumSize(1100, 720);
    resize(1400, 850);

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

    // ---------------- SIDEBAR ----------------

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
        "letter-spacing: 2px;"
        "color: #64748B;"
        "border: none;"
        "padding-bottom: 20px;"
        );

    sideLayout->addWidget(brand);
    sideLayout->addWidget(subtitle);

    auto createNavButton = [](const QString &text, bool active = false)
    {
        QPushButton *button = new QPushButton(text);
        button->setMinimumHeight(46);
        button->setCursor(Qt::PointingHandCursor);

        QString background = active ? "#12314A" : "transparent";
        QString foreground = active ? "#38BDF8" : "#94A3B8";
        QString border = active ? "#0EA5E9" : "transparent";

        button->setStyleSheet(
            QString(
                "QPushButton {"
                "text-align: left;"
                "padding-left: 15px;"
                "border-radius: 9px;"
                "border-left: 3px solid %1;"
                "background-color: %2;"
                "color: %3;"
                "font-size: 13px;"
                "font-weight: 600;"
                "}"
                "QPushButton:hover {"
                "background-color: #1E293B;"
                "color: #F8FAFC;"
                "}"
                ).arg(border, background, foreground)
            );

        return button;
    };

    QPushButton *dashboardBtn = createNavButton("  ◈   Dashboard", true);
    QPushButton *simulationBtn = createNavButton("  ◉   Traffic Simulation");
    QPushButton *networkBtn = createNavButton("  ⌘   Road Network");
    QPushButton *routingBtn = createNavButton("  ➤   Emergency Routing");
    QPushButton *analyticsBtn = createNavButton("  ▥   Analytics");
    QPushButton *algorithmsBtn = createNavButton("  ⬡   DSA Visualizer");

    sideLayout->addWidget(dashboardBtn);
    sideLayout->addWidget(simulationBtn);
    sideLayout->addWidget(networkBtn);
    sideLayout->addWidget(routingBtn);
    sideLayout->addWidget(analyticsBtn);
    sideLayout->addWidget(algorithmsBtn);

    sideLayout->addStretch();

    QLabel *systemLabel = new QLabel(
        "SYSTEM STATUS\n\n"
        "●  Simulation engine ready\n"
        "●  Road network initialized"
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

    QLabel *versionLabel = new QLabel("TRAFFIC CORE  •  v1.0");
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet(
        "color: #475569;"
        "font-size: 10px;"
        "padding-top: 15px;"
        "border: none;"
        );

    sideLayout->addWidget(versionLabel);

    root->addWidget(sidebar);

    // ---------------- MAIN CONTENT ----------------

    QWidget *content = new QWidget;

    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(30, 24, 30, 24);
    contentLayout->setSpacing(22);

    root->addWidget(content, 1);

    // ---------------- HEADER ----------------

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

    // ---------------- STATISTICS CARDS ----------------

    QGridLayout *statsGrid = new QGridLayout;
    statsGrid->setSpacing(15);

    auto createStatCard = [](const QString &title,
                             const QString &value,
                             const QString &description,
                             const QString &accent)
    {
        QFrame *card = new QFrame;
        card->setMinimumHeight(125);
        card->setStyleSheet(
            "QFrame {"
            "background-color: #111827;"
            "border: 1px solid #253047;"
            "border-radius: 13px;"
            "}"
            );

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(17, 14, 17, 14);
        layout->setSpacing(5);

        QLabel *heading = new QLabel(title);
        heading->setStyleSheet(
            "color: #94A3B8;"
            "font-size: 12px;"
            "border: none;"
            );

        QLabel *number = new QLabel(value);
        number->setStyleSheet(
            QString(
                "color: %1;"
                "font-size: 27px;"
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
        createStatCard("ACTIVE VEHICLES", "0",
                       "Waiting for simulation", "#38BDF8"),
        0, 0
        );

    statsGrid->addWidget(
        createStatCard("AVG. WAIT TIME", "0 s",
                       "No simulation data yet", "#A78BFA"),
        0, 1
        );

    statsGrid->addWidget(
        createStatCard("CONGESTION", "0%",
                       "Awaiting traffic analysis", "#FBBF24"),
        0, 2
        );

    statsGrid->addWidget(
        createStatCard("EMERGENCIES", "0",
                       "No active emergency", "#FB7185"),
        0, 3
        );

    contentLayout->addLayout(statsGrid);

    // ---------------- INTERSECTION PANEL ----------------

    QFrame *intersectionPanel = new QFrame;
    intersectionPanel->setMinimumHeight(300);
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

    intersectionLayout->setContentsMargins(20, 16, 20, 20);
    intersectionLayout->setSpacing(12);

    QHBoxLayout *intersectionHeader = new QHBoxLayout;

    QLabel *intersectionTitle =
        new QLabel("INTERSECTION CONTROL");

    intersectionTitle->setStyleSheet(
        "font-size: 13px;"
        "font-weight: 800;"
        "letter-spacing: 1px;"
        "color: #E2E8F0;"
        "border: none;"
        );

    QLabel *simulationStatus =
        new QLabel("●  SIMULATION STOPPED");

    simulationStatus->setObjectName("simulationStatus");

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
    roadCanvas->setMinimumHeight(175);
    roadCanvas->setStyleSheet(
        "background-color: #0B1220;"
        "border: 1px solid #263349;"
        "border-radius: 10px;"
        );

    QGridLayout *roadGrid = new QGridLayout(roadCanvas);
    roadGrid->setContentsMargins(10, 10, 10, 10);
    roadGrid->setSpacing(0);

    auto makeRoadLabel = [](const QString &text)
    {
        QLabel *label = new QLabel(text);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(
            "background-color: #273244;"
            "color: #E2E8F0;"
            "border: 1px solid #3A4A60;"
            "padding: 10px;"
            "font-size: 11px;"
            "font-weight: 700;"
            );
        return label;
    };

    QLabel *northRoad = makeRoadLabel("NORTH  ↑");
    QLabel *southRoad = makeRoadLabel("SOUTH  ↓");
    QLabel *westRoad  = makeRoadLabel("←  WEST");
    QLabel *eastRoad  = makeRoadLabel("EAST  →");

    QLabel *intersection = new QLabel("TRAFFIC\nINTERSECTION");
    intersection->setAlignment(Qt::AlignCenter);
    intersection->setMinimumSize(130, 90);
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

    contentLayout->addWidget(intersectionPanel, 1);

    // ---------------- SIMULATION CONTROLS ----------------

    QHBoxLayout *controls = new QHBoxLayout;
    controls->setSpacing(12);

    QLabel *controlHint = new QLabel(
        "Simulation controls"
        );

    controlHint->setStyleSheet(
        "color: #94A3B8;"
        "font-size: 12px;"
        "border: none;"
        );

    controls->addWidget(controlHint);
    controls->addStretch();

    auto makeControlButton = [](const QString &text,
                                const QString &background)
    {
        QPushButton *button = new QPushButton(text);
        button->setMinimumSize(125, 42);
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

    startButton->setObjectName("startButton");

    controls->addWidget(startButton);
    controls->addWidget(pauseButton);
    controls->addWidget(resetButton);

    contentLayout->addLayout(controls);

    // ---------------- BASIC SIMULATION STATE ----------------

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

    connect(startButton, &QPushButton::clicked, this,
            [setSimulationStatus]()
            {
                setSimulationStatus("●  SIMULATION RUNNING", "#34D399");
            });

    connect(pauseButton, &QPushButton::clicked, this,
            [setSimulationStatus]()
            {
                setSimulationStatus("Ⅱ  SIMULATION PAUSED", "#FBBF24");
            });

    connect(resetButton, &QPushButton::clicked, this,
            [setSimulationStatus]()
            {
                setSimulationStatus("●  SIMULATION STOPPED", "#FBBF24");
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

