#include "dsavisualizerwidget.h"
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QFont>
#include <QScrollArea>
#include <QGroupBox>
#include <QDateTime>

#include <queue>
#include <algorithm>
#include <limits>
#include <functional>

// =====================================================
// VISUALIZATION CANVAS
// =====================================================

class DSAVisualizationCanvas : public QWidget
{
public:

    QString mode = "QUEUE";

    QVector<int> values;
    QVector<QPair<int, int>> edges;

    QVector<int> highlighted;
    QVector<int> traversal;

    explicit DSAVisualizationCanvas(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setMinimumHeight(330);
        setSizePolicy(QSizePolicy::Expanding,
                      QSizePolicy::Expanding);
    }

    void refresh()
    {
        update();
    }

protected:

    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);

        p.setRenderHint(QPainter::Antialiasing);

        p.fillRect(rect(), QColor("#0F172A"));

        p.setPen(QColor("#64748B"));

        p.setFont(QFont("Segoe UI", 11));

        p.drawText(
            20, 28,
            mode + " VISUALIZATION"
            );

        if (mode == "QUEUE")
            drawLinear(p, true);

        else if (mode == "STACK")
            drawStack(p);

        else if (mode == "LINKED LIST")
            drawLinkedList(p);

        else if (mode == "PRIORITY QUEUE")
            drawPriority(p);

        else if (mode == "GRAPH")
            drawGraph(p);
    }

private:

    void drawNode(
        QPainter &p,
        int x,
        int y,
        int value,
        bool active = false)
    {
        QColor fill = active
                          ? QColor("#047857")
                          : QColor("#1E3A5F");

        p.setPen(QPen(
            active ? QColor("#34D399")
                   : QColor("#38BDF8"),
            2
            ));

        p.setBrush(fill);

        p.drawRoundedRect(
            x, y, 64, 48, 9, 9
            );

        p.setPen(Qt::white);

        p.setFont(QFont("Segoe UI", 12, QFont::Bold));

        p.drawText(
            QRect(x, y, 64, 48),
            Qt::AlignCenter,
            QString::number(value)
            );
    }


    void drawLinear(QPainter &p, bool isQueue)
    {
        if (values.isEmpty())
        {
            p.setPen(QColor("#94A3B8"));

            p.drawText(
                rect(),
                Qt::AlignCenter,
                "Structure is empty"
                );

            return;
        }

        int totalWidth = values.size() * 90;

        int startX = qMax(
            25,
            (width() - totalWidth) / 2
            );

        int y = height() / 2 - 24;

        for (int i = 0; i < values.size(); i++)
        {
            int x = startX + i * 90;

            drawNode(
                p,
                x,
                y,
                values[i],
                highlighted.contains(i)
                );

            if (i < values.size() - 1)
            {
                p.setPen(QPen(QColor("#64748B"), 2));

                p.drawLine(
                    x + 64,
                    y + 24,
                    x + 88,
                    y + 24
                    );

                p.drawLine(
                    x + 82, y + 18,
                    x + 88, y + 24
                    );

                p.drawLine(
                    x + 82, y + 30,
                    x + 88, y + 24
                    );
            }
        }

        p.setPen(QColor("#38BDF8"));

        p.setFont(QFont("Segoe UI", 10, QFont::Bold));

        if (isQueue)
        {
            p.drawText(
                startX,
                y - 18,
                "FRONT"
                );

            p.drawText(
                startX + (values.size() - 1) * 90,
                y + 70,
                "REAR"
                );
        }
    }


    void drawStack(QPainter &p)
    {
        if (values.isEmpty())
        {
            p.setPen(QColor("#94A3B8"));

            p.drawText(
                rect(),
                Qt::AlignCenter,
                "Stack is empty"
                );

            return;
        }

        int x = width() / 2 - 32;

        int startY = height() - 70;

        for (int i = 0; i < values.size(); i++)
        {
            int y = startY - i * 58;

            drawNode(
                p,
                x,
                y,
                values[i],
                i == values.size() - 1
                );
        }

        p.setPen(QColor("#34D399"));

        p.drawText(
            x + 85,
            startY - (values.size() - 1) * 58 + 25,
            "TOP"
            );
    }


    void drawLinkedList(QPainter &p)
    {
        if (values.isEmpty())
        {
            p.setPen(QColor("#94A3B8"));

            p.drawText(
                rect(),
                Qt::AlignCenter,
                "Linked List is empty"
                );

            return;
        }

        int totalWidth = values.size() * 95;

        int startX = qMax(
            20,
            (width() - totalWidth) / 2
            );

        int y = height() / 2 - 24;

        for (int i = 0; i < values.size(); i++)
        {
            int x = startX + i * 95;

            drawNode(
                p,
                x,
                y,
                values[i]
                );

            if (i < values.size() - 1)
            {
                p.setPen(QPen(QColor("#F59E0B"), 2));

                p.drawLine(
                    x + 64,
                    y + 24,
                    x + 93,
                    y + 24
                    );

                p.drawLine(
                    x + 87, y + 18,
                    x + 93, y + 24
                    );

                p.drawLine(
                    x + 87, y + 30,
                    x + 93, y + 24
                    );
            }
        }

        p.setPen(QColor("#94A3B8"));

        p.drawText(
            startX,
            y + 70,
            "HEAD"
            );

        p.drawText(
            startX + values.size() * 95,
            y + 70,
            "NULL"
            );
    }


    void drawPriority(QPainter &p)
    {
        if (values.isEmpty())
        {
            p.setPen(QColor("#94A3B8"));

            p.drawText(
                rect(),
                Qt::AlignCenter,
                "Priority Queue is empty"
                );

            return;
        }

        int totalWidth = values.size() * 90;

        int startX = qMax(
            20,
            (width() - totalWidth) / 2
            );

        int y = height() / 2 - 24;

        for (int i = 0; i < values.size(); i++)
        {
            int x = startX + i * 90;

            drawNode(
                p,
                x,
                y,
                values[i],
                i == 0
                );
        }

        p.setPen(QColor("#34D399"));

        p.drawText(
            startX,
            y - 20,
            "HIGHEST PRIORITY"
            );
    }


    void drawGraph(QPainter &p)
    {
        QVector<QPointF> positions = {
            {width() * 0.50, height() * 0.20},
            {width() * 0.22, height() * 0.42},
            {width() * 0.78, height() * 0.42},
            {width() * 0.32, height() * 0.78},
            {width() * 0.68, height() * 0.78},
            {width() * 0.50, height() * 0.52}
        };

        for (const auto &edge : edges)
        {
            QPointF a = positions[edge.first];
            QPointF b = positions[edge.second];

            p.setPen(QPen(QColor("#475569"), 2));

            p.drawLine(a, b);
        }

        for (int i = 0; i < positions.size(); i++)
        {
            QPointF point = positions[i];

            bool active = highlighted.contains(i);

            p.setPen(QPen(
                QColor(active ? "#34D399" : "#38BDF8"),
                2
                ));

            p.setBrush(
                active ? QColor("#047857")
                       : QColor("#1E3A5F")
                );

            p.drawEllipse(
                point,
                24,
                24
                );

            p.setPen(Qt::white);

            p.setFont(QFont("Segoe UI", 11, QFont::Bold));

            p.drawText(
                QRectF(
                    point.x() - 24,
                    point.y() - 24,
                    48,
                    48
                    ),
                Qt::AlignCenter,
                QString::number(i)
                );
        }
    }
};


// =====================================================
// CONSTRUCTOR
// =====================================================

DSAVisualizerWidget::DSAVisualizerWidget(QWidget *parent)
    : QWidget(parent)
{
    initializeGraph();

    setupUI();

    setupStyles();

    updateInterface();
}


DSAVisualizerWidget::~DSAVisualizerWidget()
{
    destroyLinkedList();
}


// =====================================================
// USER INTERFACE
// =====================================================

void DSAVisualizerWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(22, 20, 22, 20);

    mainLayout->setSpacing(16);


    // HEADER

    QLabel *heading = new QLabel("DSA VISUALIZER");

    heading->setStyleSheet(
        "font-size: 25px;"
        "font-weight: bold;"
        "color: #38BDF8;"
        );

    QLabel *subtitle = new QLabel(
        "Explore data structures through interactive operations"
        );

    subtitle->setStyleSheet(
        "color: #94A3B8;"
        "font-size: 12px;"
        );

    mainLayout->addWidget(heading);
    mainLayout->addWidget(subtitle);


    // STRUCTURE SELECTOR

    QHBoxLayout *selectorLayout = new QHBoxLayout;

    QLabel *selectLabel = new QLabel("SELECT DATA STRUCTURE");

    structureSelector = new QComboBox();

    structureSelector->addItems({
        "Queue",
        "Stack",
        "Linked List",
        "Priority Queue",
        "Graph"
    });

    structureSelector->setMinimumWidth(220);

    selectorLayout->addWidget(selectLabel);
    selectorLayout->addWidget(structureSelector);

    selectorLayout->addStretch();

    mainLayout->addLayout(selectorLayout);


    // MAIN CONTENT

    QHBoxLayout *contentLayout = new QHBoxLayout;

    contentLayout->setSpacing(16);


    // LEFT VISUALIZATION AREA

    QVBoxLayout *visualLayout = new QVBoxLayout;

    QFrame *visualCard = new QFrame();

    visualCard->setObjectName("visualCard");

    QVBoxLayout *visualCardLayout = new QVBoxLayout(visualCard);

    visualCardLayout->setContentsMargins(18, 18, 18, 18);

    moduleTitle = new QLabel();

    moduleTitle->setStyleSheet(
        "font-size: 19px;"
        "font-weight: bold;"
        "color: white;"
        );

    moduleDescription = new QLabel();

    moduleDescription->setWordWrap(true);

    moduleDescription->setStyleSheet(
        "color: #94A3B8;"
        );

    canvas = new DSAVisualizationCanvas();

    visualCardLayout->addWidget(moduleTitle);

    visualCardLayout->addWidget(moduleDescription);

    visualCardLayout->addWidget(canvas, 1);

    visualLayout->addWidget(visualCard, 1);


    // RIGHT CONTROL PANEL

    QFrame *controlCard = new QFrame();

    controlCard->setObjectName("controlCard");

    controlCard->setFixedWidth(245);

    QVBoxLayout *controlLayout = new QVBoxLayout(controlCard);

    controlLayout->setContentsMargins(16, 18, 16, 18);

    controlLayout->setSpacing(13);


    QLabel *controlTitle = new QLabel("OPERATIONS");

    controlTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #38BDF8;"
        );

    controlLayout->addWidget(controlTitle);


    QLabel *inputLabel = new QLabel("Value");

    valueInput = new QLineEdit();

    valueInput->setPlaceholderText("Enter integer value");

    controlLayout->addWidget(inputLabel);

    controlLayout->addWidget(valueInput);


    QLabel *vertexLabel = new QLabel("Source Vertex");

    vertexSelector = new QComboBox();

    for (int i = 0; i < 6; i++)
        vertexSelector->addItem("Vertex " + QString::number(i));


    QLabel *destinationLabel = new QLabel("Destination");

    destinationSelector = new QComboBox();

    for (int i = 0; i < 6; i++)
        destinationSelector->addItem("Vertex " + QString::number(i));


    controlLayout->addWidget(vertexLabel);

    controlLayout->addWidget(vertexSelector);

    controlLayout->addWidget(destinationLabel);

    controlLayout->addWidget(destinationSelector);


    primaryButton = new QPushButton();

    secondaryButton = new QPushButton();

    thirdButton = new QPushButton();

    clearButton = new QPushButton("CLEAR STRUCTURE");


    controlLayout->addSpacing(10);

    controlLayout->addWidget(primaryButton);

    controlLayout->addWidget(secondaryButton);

    controlLayout->addWidget(thirdButton);

    controlLayout->addWidget(clearButton);

    controlLayout->addStretch();


    // COMPLEXITY

    complexityLabel = new QLabel();

    complexityLabel->setWordWrap(true);

    complexityLabel->setStyleSheet(
        "color: #94A3B8;"
        "font-size: 11px;"
        );

    controlLayout->addWidget(complexityLabel);


    contentLayout->addLayout(visualLayout, 1);

    contentLayout->addWidget(controlCard);

    mainLayout->addLayout(contentLayout, 1);


    // BOTTOM INFORMATION

    QHBoxLayout *infoLayout = new QHBoxLayout();

    sizeLabel = new QLabel();

    statusLabel = new QLabel();

    infoLayout->addWidget(sizeLabel);

    infoLayout->addStretch();

    infoLayout->addWidget(statusLabel);

    mainLayout->addLayout(infoLayout);


    // OPERATION LOG

    QLabel *logHeading = new QLabel("OPERATION HISTORY");

    logHeading->setStyleSheet(
        "font-weight: bold;"
        "color: #38BDF8;"
        );

    logTable = new QTableWidget();

    logTable->setColumnCount(3);

    logTable->setHorizontalHeaderLabels({
        "Time",
        "Operation",
        "Result"
    });

    logTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    logTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    logTable->setMaximumHeight(145);

    mainLayout->addWidget(logHeading);

    mainLayout->addWidget(logTable);


    // CONNECTIONS

    connect(
        structureSelector,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        &DSAVisualizerWidget::changeStructure
        );

    connect(
        primaryButton,
        &QPushButton::clicked,
        this,
        &DSAVisualizerWidget::performPrimaryOperation
        );

    connect(
        secondaryButton,
        &QPushButton::clicked,
        this,
        &DSAVisualizerWidget::performSecondaryOperation
        );

    connect(
        thirdButton,
        &QPushButton::clicked,
        this,
        &DSAVisualizerWidget::performThirdOperation
        );

    connect(
        clearButton,
        &QPushButton::clicked,
        this,
        &DSAVisualizerWidget::clearStructure
        );
}


// =====================================================
// STYLES
// =====================================================

void DSAVisualizerWidget::setupStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #0B1120;
            color: #E2E8F0;
            font-family: Segoe UI;
            font-size: 12px;
        }

        QFrame#visualCard,
        QFrame#controlCard {
            background-color: #111C30;
            border: 1px solid #26364F;
            border-radius: 12px;
        }

        QComboBox,
        QLineEdit {
            background-color: #17243A;
            border: 1px solid #334155;
            border-radius: 7px;
            padding: 9px;
            color: white;
        }

        QPushButton {
            background-color: #1D4ED8;
            border: none;
            border-radius: 7px;
            padding: 11px;
            color: white;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #2563EB;
        }

        QPushButton:pressed {
            background-color: #1E40AF;
        }

        QTableWidget {
            background-color: #111C30;
            border: 1px solid #26364F;
            gridline-color: #334155;
        }

        QHeaderView::section {
            background-color: #17243A;
            color: #38BDF8;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
    )");
}


// =====================================================
// STRUCTURE SELECTION
// =====================================================

void DSAVisualizerWidget::changeStructure(int index)
{
    currentStructure = static_cast<Structure>(index);

    updateInterface();

    updateVisualization();

    setStatus("Selected " + structureSelector->currentText());
}


// =====================================================
// UPDATE INTERFACE
// =====================================================

void DSAVisualizerWidget::updateInterface()
{
    QString mode = structureSelector->currentText();

    moduleTitle->setText(mode);

    valueInput->setVisible(currentStructure != Graph);

    bool graphMode = currentStructure == Graph;

    vertexSelector->setVisible(graphMode);

    destinationSelector->setVisible(graphMode);

    primaryButton->setVisible(true);

    secondaryButton->setVisible(true);

    thirdButton->setVisible(true);

    clearButton->setText("CLEAR STRUCTURE");

    if (currentStructure == Queue)
    {
        moduleDescription->setText(
            "FIFO: First In, First Out"
            );

        primaryButton->setText("ENQUEUE");

        secondaryButton->setText("DEQUEUE");

        thirdButton->setText("PEEK");

        complexityLabel->setText(
            "Enqueue: O(1)\nDequeue: O(n) in this simple array implementation\nPeek: O(1)"
            );
    }

    else if (currentStructure == Stack)
    {
        moduleDescription->setText(
            "LIFO: Last In, First Out"
            );

        primaryButton->setText("PUSH");

        secondaryButton->setText("POP");

        thirdButton->setText("PEEK");

        complexityLabel->setText(
            "Push: O(1)\nPop: O(1)\nPeek: O(1)"
            );
    }

    else if (currentStructure == LinkedList)
    {
        moduleDescription->setText(
            "Dynamic singly linked list using pointers"
            );

        primaryButton->setText("INSERT AT END");

        secondaryButton->setText("DELETE VALUE");

        thirdButton->setText("SEARCH VALUE");

        complexityLabel->setText(
            "Insert at end: O(n)\nDelete: O(n)\nSearch: O(n)"
            );
    }

    else if (currentStructure == PriorityQueue)
    {
        moduleDescription->setText(
            "Max Heap: Highest value has highest priority"
            );

        primaryButton->setText("INSERT");

        secondaryButton->setText("EXTRACT MAX");

        thirdButton->setText("PEEK MAX");

        complexityLabel->setText(
            "Insert: O(log n)\nExtract Max: O(log n)\nPeek: O(1)"
            );
    }

    else
    {
        moduleDescription->setText(
            "Graph represented using an adjacency list"
            );

        primaryButton->setText("BFS");

        secondaryButton->setText("DFS");

        thirdButton->setText("DIJKSTRA");

        complexityLabel->setText(
            "BFS: O(V + E)\nDFS: O(V + E)\nDijkstra: O((V + E) log V)"
            );
    }
}


// =====================================================
// OPERATION DISPATCH
// =====================================================

void DSAVisualizerWidget::performPrimaryOperation()
{
    if (currentStructure == Queue)
        enqueue();

    else if (currentStructure == Stack)
        pushStack();

    else if (currentStructure == LinkedList)
        insertLinkedList();

    else if (currentStructure == PriorityQueue)
        insertPriority();

    else
        performBFS();
}


void DSAVisualizerWidget::performSecondaryOperation()
{
    if (currentStructure == Queue)
        dequeue();

    else if (currentStructure == Stack)
        popStack();

    else if (currentStructure == LinkedList)
        deleteLinkedList();

    else if (currentStructure == PriorityQueue)
        extractPriority();

    else
        performDFS();
}


void DSAVisualizerWidget::performThirdOperation()
{
    if (currentStructure == Queue)
        peekQueue();

    else if (currentStructure == Stack)
        peekStack();

    else if (currentStructure == LinkedList)
        searchLinkedList();

    else if (currentStructure == PriorityQueue)
        peekPriority();

    else
        performDijkstra();
}


// =====================================================
// INPUT AND STATUS
// =====================================================

int DSAVisualizerWidget::inputValue() const
{
    return valueInput->text().toInt();
}


void DSAVisualizerWidget::setStatus(
    QString message,
    bool success)
{
    statusLabel->setText(message);

    statusLabel->setStyleSheet(
        success
            ? "color: #34D399;"
            : "color: #F87171;"
        );
}


void DSAVisualizerWidget::addLog(
    QString operation,
    QString result)
{
    int row = logTable->rowCount();

    logTable->insertRow(row);

    logTable->setItem(
        row, 0,
        new QTableWidgetItem(
            QDateTime::currentDateTime().toString("HH:mm:ss")
            )
        );

    logTable->setItem(
        row, 1,
        new QTableWidgetItem(operation)
        );

    logTable->setItem(
        row, 2,
        new QTableWidgetItem(result)
        );

    logTable->scrollToBottom();

    if (logTable->rowCount() > 100)
        logTable->removeRow(0);
}


// =====================================================
// QUEUE OPERATIONS
// =====================================================

void DSAVisualizerWidget::enqueue()
{
    if (queueData.size() >= 8)
    {
        setStatus("Queue Overflow!", false);

        addLog("Enqueue", "Queue is full");

        return;
    }

    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    queueData.append(value);

    setStatus("Element inserted successfully.");

    addLog("Enqueue", QString::number(value));

    updateVisualization();
}


void DSAVisualizerWidget::dequeue()
{
    if (queueData.isEmpty())
    {
        setStatus("Queue Underflow!", false);

        addLog("Dequeue", "Queue is empty");

        return;
    }

    int removed = queueData.takeFirst();

    setStatus("Removed: " + QString::number(removed));

    addLog("Dequeue", QString::number(removed));

    updateVisualization();
}


void DSAVisualizerWidget::peekQueue()
{
    if (queueData.isEmpty())
    {
        setStatus("Queue is empty.", false);

        return;
    }

    setStatus(
        "Front element: " +
        QString::number(queueData.first())
        );

    addLog(
        "Peek",
        QString::number(queueData.first())
        );
}


// =====================================================
// STACK OPERATIONS
// =====================================================

void DSAVisualizerWidget::pushStack()
{
    if (stackData.size() >= 8)
    {
        setStatus("Stack Overflow!", false);

        addLog("Push", "Stack is full");

        return;
    }

    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    stackData.append(value);

    setStatus("Pushed: " + QString::number(value));

    addLog("Push", QString::number(value));

    updateVisualization();
}


void DSAVisualizerWidget::popStack()
{
    if (stackData.isEmpty())
    {
        setStatus("Stack Underflow!", false);

        addLog("Pop", "Stack is empty");

        return;
    }

    int removed = stackData.takeLast();

    setStatus("Popped: " + QString::number(removed));

    addLog("Pop", QString::number(removed));

    updateVisualization();
}


void DSAVisualizerWidget::peekStack()
{
    if (stackData.isEmpty())
    {
        setStatus("Stack is empty.", false);

        return;
    }

    setStatus(
        "Top element: " +
        QString::number(stackData.last())
        );

    addLog(
        "Peek",
        QString::number(stackData.last())
        );
}


// =====================================================
// LINKED LIST OPERATIONS
// =====================================================

void DSAVisualizerWidget::insertLinkedList()
{
    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    ListNode *newNode = new ListNode(value);

    if (!head)
    {
        head = newNode;
    }
    else
    {
        ListNode *temp = head;

        while (temp->next)
            temp = temp->next;

        temp->next = newNode;
    }

    setStatus("Node inserted successfully.");

    addLog("Insert", QString::number(value));

    updateVisualization();
}


void DSAVisualizerWidget::deleteLinkedList()
{
    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    if (!head)
    {
        setStatus("Linked List is empty.", false);

        return;
    }

    ListNode *temp = head;

    ListNode *previous = nullptr;

    while (temp && temp->data != value)
    {
        previous = temp;

        temp = temp->next;
    }

    if (!temp)
    {
        setStatus("Value not found.", false);

        return;
    }

    if (!previous)
        head = temp->next;

    else
        previous->next = temp->next;

    delete temp;

    setStatus("Node deleted successfully.");

    addLog("Delete", QString::number(value));

    updateVisualization();
}


void DSAVisualizerWidget::searchLinkedList()
{
    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    ListNode *temp = head;

    int position = 0;

    while (temp)
    {
        if (temp->data == value)
        {
            highlightedNodes = {position};

            setStatus(
                "Found at position " +
                QString::number(position + 1)
                );

            addLog("Search", "Found");

            updateVisualization();

            return;
        }

        temp = temp->next;

        position++;
    }

    highlightedNodes.clear();

    setStatus("Value not found.", false);

    addLog("Search", "Not found");

    updateVisualization();
}


void DSAVisualizerWidget::reverseLinkedList()
{
    ListNode *previous = nullptr;

    ListNode *current = head;

    while (current)
    {
        ListNode *nextNode = current->next;

        current->next = previous;

        previous = current;

        current = nextNode;
    }

    head = previous;

    setStatus("Linked List reversed.");

    addLog("Reverse", "Completed");

    updateVisualization();
}


// =====================================================
// PRIORITY QUEUE OPERATIONS
// =====================================================

void DSAVisualizerWidget::insertPriority()
{
    bool ok;

    int value = valueInput->text().toInt(&ok);

    if (!ok)
    {
        setStatus("Enter a valid integer.", false);

        return;
    }

    if (priorityData.size() >= 15)
    {
        setStatus("Priority Queue is full.", false);

        return;
    }

    priorityData.append(value);

    std::push_heap(
        priorityData.begin(),
        priorityData.end()
        );

    setStatus("Inserted into Max Heap.");

    addLog("Insert", QString::number(value));

    updateVisualization();
}


void DSAVisualizerWidget::extractPriority()
{
    if (priorityData.isEmpty())
    {
        setStatus("Priority Queue is empty.", false);

        return;
    }

    std::pop_heap(
        priorityData.begin(),
        priorityData.end()
        );

    int removed = priorityData.takeLast();

    setStatus("Extracted maximum: " + QString::number(removed));

    addLog("Extract Max", QString::number(removed));

    updateVisualization();
}


void DSAVisualizerWidget::peekPriority()
{
    if (priorityData.isEmpty())
    {
        setStatus("Priority Queue is empty.", false);

        return;
    }

    setStatus(
        "Maximum: " +
        QString::number(priorityData.first())
        );

    addLog(
        "Peek Max",
        QString::number(priorityData.first())
        );
}


// =====================================================
// GRAPH
// =====================================================

void DSAVisualizerWidget::initializeGraph()
{
    graph.resize(6);

    addGraphEdge(0, 1, 4);

    addGraphEdge(0, 2, 2);

    addGraphEdge(1, 3, 5);

    addGraphEdge(2, 3, 8);

    addGraphEdge(2, 4, 3);

    addGraphEdge(3, 5, 6);

    addGraphEdge(4, 5, 4);
}


void DSAVisualizerWidget::addGraphEdge(
    int u,
    int v,
    int weight)
{
    graph[u].append(v);

    graph[v].append(u);

    Q_UNUSED(weight);
}


// =====================================================
// BFS
// =====================================================

void DSAVisualizerWidget::performBFS()
{
    int start = vertexSelector->currentIndex();

    QVector<bool> visited(6, false);

    QVector<int> result;

    std::queue<int> q;

    q.push(start);

    visited[start] = true;

    while (!q.empty())
    {
        int node = q.front();

        q.pop();

        result.append(node);

        for (int neighbour : graph[node])
        {
            if (!visited[neighbour])
            {
                visited[neighbour] = true;

                q.push(neighbour);
            }
        }
    }

    highlightedNodes = result;

    QString output;

    for (int node : result)
        output += QString::number(node) + "  ";

    traversalResult = result;

    setStatus("BFS: " + output);

    addLog("BFS", output);

    updateVisualization();
}


// =====================================================
// DFS
// =====================================================

void DSAVisualizerWidget::performDFS()
{
    int start = vertexSelector->currentIndex();

    QVector<bool> visited(6, false);

    QVector<int> result;

    std::function<void(int)> dfs = [&](int node)
    {
        visited[node] = true;

        result.append(node);

        for (int neighbour : graph[node])
        {
            if (!visited[neighbour])
                dfs(neighbour);
        }
    };

    dfs(start);

    highlightedNodes = result;

    QString output;

    for (int node : result)
        output += QString::number(node) + "  ";

    traversalResult = result;

    setStatus("DFS: " + output);

    addLog("DFS", output);

    updateVisualization();
}


// =====================================================
// DIJKSTRA
// =====================================================

void DSAVisualizerWidget::performDijkstra()
{
    int source = vertexSelector->currentIndex();

    int destination = destinationSelector->currentIndex();

    const int INF = std::numeric_limits<int>::max();

    QVector<int> distance(6, INF);

    QVector<int> parent(6, -1);

    using Node = QPair<int, int>;

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

        int current = pq.top().second;

        pq.pop();

        if (currentDistance > distance[current])
            continue;

        for (int neighbour : graph[current])
        {
            int weight = 1;

            if (distance[current] + weight <
                distance[neighbour])
            {
                distance[neighbour] =
                    distance[current] + weight;

                parent[neighbour] = current;

                pq.push({
                    distance[neighbour],
                    neighbour
                });
            }
        }
    }

    if (distance[destination] == INF)
    {
        setStatus("No path found.", false);

        return;
    }

    QVector<int> path;

    int current = destination;

    while (current != -1)
    {
        path.prepend(current);

        current = parent[current];
    }

    highlightedNodes = path;

    QString output;

    for (int node : path)
    {
        output += QString::number(node);

        if (node != destination)
            output += " → ";
    }

    setStatus(
        "Shortest path: " + output +
        " | Cost: " + QString::number(distance[destination])
        );

    addLog("Dijkstra", output);

    updateVisualization();
}


// =====================================================
// CLEAR STRUCTURE
// =====================================================

void DSAVisualizerWidget::clearStructure()
{
    if (currentStructure == Queue)
        queueData.clear();

    else if (currentStructure == Stack)
        stackData.clear();

    else if (currentStructure == PriorityQueue)
        priorityData.clear();

    else if (currentStructure == LinkedList)
        destroyLinkedList();

    else
    {
        highlightedNodes.clear();

        traversalResult.clear();
    }

    highlightedNodes.clear();

    setStatus("Structure cleared.");

    addLog("Clear", "Completed");

    updateVisualization();
}


// =====================================================
// MEMORY MANAGEMENT
// =====================================================

void DSAVisualizerWidget::destroyLinkedList()
{
    while (head)
    {
        ListNode *temp = head;

        head = head->next;

        delete temp;
    }
}


// =====================================================
// UPDATE VISUALIZATION
// =====================================================

void DSAVisualizerWidget::updateVisualization()
{
    QVector<int> display;

    if (currentStructure == Queue)
        display = queueData;

    else if (currentStructure == Stack)
        display = stackData;

    else if (currentStructure == PriorityQueue)
        display = priorityData;

    else if (currentStructure == LinkedList)
    {
        ListNode *temp = head;

        while (temp)
        {
            display.append(temp->data);

            temp = temp->next;
        }
    }

    canvas->values = display;

    canvas->highlighted = highlightedNodes;

    canvas->edges.clear();

    if (currentStructure == Graph)
    {
        canvas->mode = "GRAPH";

        for (int i = 0; i < graph.size(); i++)
        {
            for (int neighbour : graph[i])
            {
                if (i < neighbour)
                    canvas->edges.append({i, neighbour});
            }
        }
    }
    else
    {
        canvas->mode = structureSelector->currentText().toUpper();
    }

    canvas->refresh();

    sizeLabel->setText(
        "Elements: " + QString::number(display.size())
        );
}