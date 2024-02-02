#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//使用无向图+邻接表  最短路径使用floyd
#include <QMainWindow>
#include <QDir>
#include <QVector>
#include <QTabBar>
#include <QMouseEvent>
#include <QFile>
#include <QPainter>
#include <QMessageBox>
#include <QStack>
#include <cstring>
#include <QQueue>

#define pointPath QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + QString("point.txt"))
#define edgePath QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + QString("edge.txt"))

//MAX设置为比任何一条最短路径都长
#define VERMAX 100
#define MAX 9999
extern int times;

//无向图
typedef struct EdgeNode {
    int index;
    int weight;
    EdgeNode* next;
}EdgeNode;

//含头结点的单链表
typedef struct {
    int id;
    QString name;
    QString description;
    EdgeNode* first;
}VerNode, VerArray[VERMAX];

typedef struct {
    VerArray arr;
    int vercount, edgecount;
}Graph;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

    void saveClickInfo(const QPoint &point);
    bool eventFilter(QObject *obj, QEvent *event);
    void readAndDraw();
    bool isOverlapping(QPixmap& pixmap, QPoint& newPoint);
    void drawLine();
    void deleteLine();
    void deletePoint();
    void changePart();
    void getPart();

    int seekLocation(int id);
    EdgeNode* createEdgeNode(int index, int weight);
    void createGraph();
    void printPath(int index1, int index2, QString& path);

    void init();
    void floyd();
    QString seekShort(int id1, int id2);
    void DfsAllpath(int s, int e, QStack<int>& path, int& dis);
    void DFSKeyNode(int i);
    void DFS(int j);

    bool isConnected;
    int node;

    QVector<QString> allPath;
    int visited[VERMAX];

    QVector<int> keyNode;
    QVector<QPair<int, int>> bridgeVector;
    int dfn[VERMAX];
    int low[VERMAX];
    int father[VERMAX];

    Graph graph;
    int index;
    Ui::MainWindow *ui;
    int matrix[VERMAX][VERMAX];
    int transitNode[VERMAX][VERMAX];
};
#endif // MAINWINDOW_H
