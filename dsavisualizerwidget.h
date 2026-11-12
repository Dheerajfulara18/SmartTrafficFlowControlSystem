#ifndef DSAVISUALIZERWIDGET_H
#define DSAVISUALIZERWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPair>

class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
class QTableWidget;
class QPaintEvent;
class DSAVisualizationCanvas;

class DSAVisualizerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DSAVisualizerWidget(QWidget *parent = nullptr);
    ~DSAVisualizerWidget();

private:
    enum Structure
    {
        Queue,
        Stack,
        LinkedList,
        PriorityQueue,
        Graph
    };

    struct ListNode
    {
        int data;
        ListNode *next;

        explicit ListNode(int value)
            : data(value), next(nullptr) {}
    };

    // Current selected structure
    Structure currentStructure = Queue;

    // Actual data structures
    QVector<int> queueData;
    QVector<int> stackData;
    QVector<int> priorityData;

    ListNode *head = nullptr;

    QVector<QVector<int>> graph;

    // Graph traversal
    QVector<int> traversalResult;
    QVector<int> highlightedNodes;

    // UI
    QComboBox *structureSelector;
    QLineEdit *valueInput;
    QComboBox *vertexSelector;
    QComboBox *destinationSelector;

    QLabel *moduleTitle;
    QLabel *moduleDescription;
    QLabel *statusLabel;
    QLabel *complexityLabel;
    QLabel *sizeLabel;

    QPushButton *primaryButton;
    QPushButton *secondaryButton;
    QPushButton *thirdButton;
    QPushButton *clearButton;

    DSAVisualizationCanvas *canvas;

    QTableWidget *logTable;

    // UI functions
    void setupUI();
    void setupStyles();
    void updateInterface();
    void updateVisualization();
    void addLog(QString operation, QString result);

    // Structure selection
    void changeStructure(int index);

    // Operations
    void performPrimaryOperation();
    void performSecondaryOperation();
    void performThirdOperation();
    void clearStructure();

    // Queue
    void enqueue();
    void dequeue();
    void peekQueue();

    // Stack
    void pushStack();
    void popStack();
    void peekStack();

    // Linked List
    void insertLinkedList();
    void deleteLinkedList();
    void searchLinkedList();
    void reverseLinkedList();

    // Priority Queue
    void insertPriority();
    void extractPriority();
    void peekPriority();

    // Graph
    void initializeGraph();
    void addGraphEdge(int u, int v, int weight);
    void performBFS();
    void performDFS();
    void performDijkstra();

    // Utilities
    int inputValue() const;
    void setStatus(QString message, bool success = true);
    void destroyLinkedList();
};

#endif