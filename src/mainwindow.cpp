#include "mainwindow.h"
#include "./ui_mainwindow.h"

int times = 1;

//折半查找
int MainWindow::seekLocation(int id) {
    int low = 0;
    int high = graph.vercount - 1;
    for (; low <= high;) {
        int mid = (low + high) / 2;//向下取整
        if (id < graph.arr[mid].id) {
            high = mid - 1;
        }
        else if (id > graph.arr[mid].id) {
            low = mid + 1;
        }
        else {
            return mid;
        }
    }
    //未找到返回-1
    return -1;
}

// int MainWindow::seekLocation(int id) {
//     for (int i = 0; i < graph.vercount; i++) {
//         if (id == graph.arr[i].id) {
//             return i;
//         }
//     }
//     return -1;
// }

EdgeNode* MainWindow::createEdgeNode(int index, int weight) {
    EdgeNode* e = new EdgeNode{};
    if (!e) {
        qDebug() << "overflow";
        exit(1);
    }
    e->index = index;
    e->weight = weight;
    e->next = nullptr;
    return e;
}

void MainWindow::createGraph() {

    graph.vercount = 0;
    graph.edgecount = 0;
    QVector<QStringList> vectorNode;
    QVector<QStringList> vectorEdge;

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        exit(1);
    }

    QTextStream in(&file);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        graph.vercount++;
        vectorNode.append(fields);
    }
    file.close();

    QFile file1(edgePath);
    if (!file1.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        exit(1);
    }

    QTextStream in1(&file1);
    while (!in1.atEnd()) {

        QString line = in1.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        graph.edgecount++;
        vectorEdge.append(fields);
    }
    file1.close();

    for (int i = 0; i < graph.vercount; i++) {

        graph.arr[i].first = new EdgeNode;
        if (!graph.arr[i].first) {
            qDebug() << "overflow";
            return;
        }
        graph.arr[i].first->next = nullptr;
        graph.arr[i].id = vectorNode[i][0].toInt();
        graph.arr[i].name = vectorNode[i][3];
        graph.arr[i].description = vectorNode[i][4];
    }

    int loc1, loc2;
    for (int i = 0; i < graph.edgecount; ) {

        loc1 = seekLocation(vectorEdge[i][0].toInt());
        loc2 = seekLocation(vectorEdge[i][3].toInt());
        if (loc1 != -1 && loc2 != -1) {//使用头插,形成单链表的顺序完全取决于你输入的结点的顺序和头，尾插的方式
            EdgeNode* e = createEdgeNode(loc2, vectorEdge[i][6].toInt());
            e->next = graph.arr[loc1].first->next;
            graph.arr[loc1].first->next = e;

            e = createEdgeNode(loc1, vectorEdge[i][6].toInt());
            e->next = graph.arr[loc2].first->next;
            graph.arr[loc2].first->next = e;

            i++;
        }
    }
}


//当你第一次调用 printPath(g, matrix, transit, 0, 4); 时，static int node 被初始化为 4，并在整个递归过程中保持为 4。
//即使在后续递归调用中，node 的值不会发生变化，因为它是静态的
//静态变量在程序的生命周期内只会被初始化一次

//下标
void MainWindow::printPath(int index1, int index2, QString& path) {
    if (transitNode[index1][index2] == -1 && matrix[index1][index2] != MAX) {
        path += (QString::number(graph.arr[index1].id) + "->");
        if (node == index2) {
            path += (QString::number(graph.arr[node].id));
        }
    }
    else if(transitNode[index1][index2] == -1 && matrix[index1][index2] == MAX){
        return;
    }
    else {
        printPath(index1, transitNode[index1][index2], path);
        printPath(transitNode[index1][index2],index2, path);
    }
}

QString MainWindow::seekShort(int id1, int id2){
    int loc1 = seekLocation(id1);
    int loc2 = seekLocation(id2);

    QString string = "";
    printPath(loc1, loc2, string);
    return string;
}

void MainWindow::init(){
    for (int i = 0; i < graph.vercount; i++) {
        for (int j = 0; j < graph.vercount; j++) {
            matrix[i][j] = MAX;
            if (i == j) {
                matrix[i][i] = 0;
            }
        }
    }
    for (int i = 0; i < graph.vercount; i++) {
        for (EdgeNode* p = graph.arr[i].first->next; p; p = p->next) {
            matrix[i][p->index] = p->weight;
        }
    }

    for (int i = 0; i < graph.vercount; i++) {
        for (int j = 0; j < graph.vercount; j++) {
            transitNode[i][j] = -1;
        }
    }
}

void MainWindow::floyd() {
    for (int i = 0; i < graph.vercount; i++) {//中转点
        for (int j = 0; j < graph.vercount; j++) {
            for (int k = 0; k < graph.vercount; k++) {
                if (matrix[j][k] > matrix[j][i] + matrix[i][k]) {
                    matrix[j][k] = matrix[j][i] + matrix[i][k];
                    transitNode[j][k] = i;
                }
            }
        }
    }

}


//startIndex,finishIndex为下标
void MainWindow::DfsAllpath(int startIndex, int finishIndex, QStack<int>& path, int& dis) {

    path.push(startIndex);
    visited[startIndex] = 1;

    EdgeNode* p = graph.arr[startIndex].first->next;

    while (p) {
        int i = p->index;
        if (!visited[i]) {
            dis += p->weight;

            if (i == finishIndex) {
                QString strPath;
                int count = allPath.size();
                count++;
                strPath += "第"+ QString::number(count) + "条路:";
                QStack<int> tempPath;
                while (!path.isEmpty()) {
                    tempPath.push(path.top());
                    path.pop();
                }
                while (!tempPath.empty()) {
                    strPath += QString::number(graph.arr[tempPath.top()].id);
                    path.push(tempPath.top());
                    tempPath.pop();
                    if (!tempPath.empty()) {
                        strPath += "->";
                    }
                }
                strPath += "->" + QString::number(graph.arr[i].id);
                strPath += " length:" + QString::number(dis);
                allPath.append(strPath);
            } else {
                DfsAllpath(i, finishIndex, path, dis);
            }
            dis -= p->weight; // 回溯时减去路径长度
        }
        p = p->next;
    }
    path.pop();
    visited[startIndex] = 0;
}

//i为索引
void MainWindow::DFSKeyNode(int i){

    dfn[i] = low[i] = times++;
    int child = 0;

    for(EdgeNode* edge = graph.arr[i].first->next; edge; edge = edge->next){
        if(!dfn[edge->index]){
            child++;
            father[edge->index] = i;
            DFSKeyNode(edge->index);
            bool isRepeat = false;
            if(-1 == father[i] && child >= 2){
                for(int n = 0; n < keyNode.size(); n++){
                    if(graph.arr[i].id == keyNode[n]){
                        isRepeat = true;
                        break;
                    }
                }
                if(!isRepeat){
                    keyNode.push_back(graph.arr[i].id);
                }
            }

            isRepeat = false;
            if(-1 != father[i] && low[edge->index] >= dfn[i]){
                for(int n = 0; n < keyNode.size(); n++){
                    if(graph.arr[i].id == keyNode[n]){
                        isRepeat = true;
                        break;
                    }
                }
                if(!isRepeat){
                    keyNode.push_back(graph.arr[i].id);
                }
            }
            isRepeat = false;
            if(low[edge->index] > dfn[i]){
                for(int k = 0; k < bridgeVector.size(); k++){
                    if(graph.arr[i].id == bridgeVector[k].first && graph.arr[edge->index].id == bridgeVector[k].second){
                        isRepeat = true;
                        break;
                    }
                }
                if(!isRepeat){
                    bridgeVector.push_back(qMakePair(graph.arr[i].id, graph.arr[edge->index].id));
                }
            }
            isRepeat = false;
            low[i] = qMin(low[i], low[edge->index]);
        }
        else if(edge->index != father[i]){
            low[i] = qMin(low[i], low[edge->index]);
        }
    }
}


void MainWindow::DFS(int j) {
    visited[j] = true;
    for (int i = 0; i < graph.vercount; i++) {
        if (matrix[j][i] != MAX && visited[i] != 1) {//其中j!=i可以不写，因为如果相等的话visited[i] == 1
            DFS(i);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    index = 0;
    memset(visited, 0, sizeof(visited));
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(father, -1, sizeof(father));

    //当 scaledContents 设置为 true 时，QLabel 中的内容会根据 QLabel 的大小进行缩放，以保持内容的宽高比
    //没用UI界面的scaledContents ，应为宽高等比例缩放，可能显示不全
    // 将 ui->labelImg 置于最上层
    ui->labelImg->raise();
    QPixmap img(":new/image/nucMap.png");
    ui->labelImg->setPixmap(img.scaled(ui->labelImg->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->pushButton0, &QPushButton::clicked, [=](){
        ui->tabWidget->setCurrentIndex(0);
    });

    connect(ui->pushButton1, &QPushButton::clicked, [=](){
        ui->tabWidget->setCurrentIndex(1);

        QFile file(pointPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "fail to open the file";
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
            if(fields.size() != 5){
                QMessageBox::warning(this, "Warning", "景点描述不完整!");
                ui->tabWidget->setCurrentIndex(0);
                return;
            }
        }        
        createGraph();
        init();
        floyd();

        memset(visited, 0, sizeof(visited));
        int j = 0;
        for (int i = 0; i < graph.vercount; i++) {
            if (visited[i] != 1) {
                DFS(i);
                j++;
            }
        }
        if(j > 1){
            isConnected = false;
        }
        else{
            isConnected = true;
        }
        memset(visited, 0, sizeof(visited));

    });

    connect(ui->pushButtonSeekShort, &QPushButton::clicked, [=](){
        ui->textEditShort->setReadOnly(false);
        ui->textEditAll->setReadOnly(false);

        ui->textEditShort->clear();
        ui->textEditAll->clear();

        memset(visited, 0, sizeof(visited));
        allPath.clear();

        if(ui->lineEditSPut->text() == "" || ui->lineEditFPut->text() == ""){
            QMessageBox::warning(this, "Warning", "景点未输入!");
            ui->textEditShort->setReadOnly(true);
            ui->textEditAll->setReadOnly(true);
            return;
        }

        if(ui->lineEditSPut->text() == ui->lineEditFPut->text()){
            QMessageBox::warning(this, "Warning", "结点相同");
            ui->textEditShort->setReadOnly(true);
            ui->textEditAll->setReadOnly(true);
            return;
        }

        int id1 = ui->lineEditSPut->text().toInt();
        int id2 = ui->lineEditFPut->text().toInt();

        if(seekLocation(id1) == -1 || seekLocation(id2) == -1){
            QMessageBox::warning(this, "Warning", "景点不存在!");
            ui->textEditShort->setReadOnly(true);
            ui->textEditAll->setReadOnly(true);
            return;
        }

        node = seekLocation(id2);
        QString string = seekShort(id1, id2);
        ui->textEditShort->append(string);

        int dis = 0;
        QStack<int> path;
        DfsAllpath(seekLocation(id1), seekLocation(id2), path, dis);
        for(auto& i : allPath){
            ui->textEditAll->append(i);
        }

        if(allPath.isEmpty()){
            QMessageBox::warning(this, "Warning", "两点不连通");
            ui->textEditShort->setReadOnly(true);
            ui->textEditAll->setReadOnly(true);
            return;
        }

        ui->lineEditSPut->clear();
        ui->lineEditFPut->clear();
        ui->textEditShort->setReadOnly(true);
        ui->textEditAll->setReadOnly(true);


    });

    connect(ui->pushButtonISeek, &QPushButton::clicked, [=](){
        ui->lineEditInSpot->setReadOnly(false);
        ui->textEditInquiry->setReadOnly(false);

        ui->lineEditInSpot->clear();
        ui->textEditInquiry->clear();

        if(ui->lineEdit->text() == ""){
            QMessageBox::warning(this, "Warning", "景点未输入!");
            ui->lineEditInSpot->setReadOnly(true);
            ui->textEditInquiry->setReadOnly(true);
            return;
        }

        int id = ui->lineEdit->text().toInt();
        if(seekLocation(id) == -1){
            QMessageBox::warning(this, "Warning", "景点不存在!");
            ui->lineEditInSpot->setReadOnly(true);
            ui->textEditInquiry->setReadOnly(true);
            return;
        }
        else{
            ui->lineEditInSpot->setText(graph.arr[seekLocation(id)].name);
            ui->textEditInquiry->append(graph.arr[seekLocation(id)].description);
        }

        ui->lineEditInSpot->setReadOnly(true);
        ui->textEditInquiry->setReadOnly(true);
    });

    connect(ui->pushButtonSeekKey, &QPushButton::clicked, [=](){

        if(isConnected == false){
            QMessageBox::warning(this, "Warning", "非连通图无割点和桥!");
            return;
        }

        times = 1;
        keyNode.clear();
        bridgeVector.clear();
        memset(dfn, 0, sizeof(dfn));
        memset(low, 0, sizeof(low));
        memset(father, -1, sizeof(father));

        ui->lineEditKeyPut->setReadOnly(false);
        ui->textEditBridge->setReadOnly(false);

        ui->lineEditKeyPut->clear();
        ui->textEditBridge->clear();

        for (int i = 0; i < graph.vercount; i++) {
            if (!dfn[i]) {
                DFSKeyNode(i);
            }
        }
        QString node;
        for(auto& i : keyNode){
            node += QString::number(i);
            node += " ";
        }
        ui->lineEditKeyPut->setText(node);

        QStringList list;
        for(auto& i : bridgeVector){
            QString string(QString::number(i.first) + "-" + QString::number(i.second));
            list.append(string);
        }
        for(int i = 0; i < list.size(); i++){
            ui->textEditBridge->append(list[i]);
        }

        if(keyNode.size() == 0 || bridgeVector.size() == 0){
            QMessageBox::warning(this, "Warning", "没有关节点或桥!");
            ui->lineEditKeyPut->setReadOnly(true);
            ui->textEditBridge->setReadOnly(true);
            return;
        }

        ui->lineEditKeyPut->setReadOnly(true);
        ui->textEditBridge->setReadOnly(true);
    });

    connect(ui->pushButtonSureBest, &QPushButton::clicked, [=](){

        ui->textEditBest->setReadOnly(false);

        ui->textEditBest->clear();
        QString line = ui->lineEditBestPut->text();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);

        if(fields.size() < 2){
            QMessageBox::warning(this, "Warning", "景点个数小于2个");
            ui->textEditBest->setReadOnly(true);
            return;
        }

        for(int j = 0; j < fields.size() - 1; j++){
            for(int k = j + 1; k < fields.size(); k++){
                if(fields[j] == fields[k]){
                    QMessageBox::warning(this, "Warning", "输入相同结点");
                    ui->textEditBest->setReadOnly(true);
                    return;
                }
            }
        }


        for(int j = 0; j < fields.size(); j++){
            bool isFound = false;
            if(seekLocation(fields[j].toInt()) != -1){
                isFound = true;
            }
            if(!isFound){
                QMessageBox::warning(this, "Warning", "景点不存在");
                ui->textEditBest->setReadOnly(true);
                return;
            }
        }


        QQueue<int> q;
        for(auto& i : fields){
            q.enqueue(i.toInt());
        }

        while (!q.isEmpty() && q.size() >= 2) {
            int id1 = q.dequeue();
            int id2 = q.head();
            node = seekLocation(id2);
            QString string = seekShort(id1, id2);
            if(string == ""){
                string = "无";
            }
            ui->textEditBest->append(string);
        }

        ui->textEditBest->setReadOnly(true);
    });


    connect(ui->pushButton2, &QPushButton::clicked, [=](){
        ui->tabWidget->setCurrentIndex(2);

        QFile file(pointPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "fail to open the file";
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
            if(fields.size() != 5){
                QMessageBox::warning(this, "Warning", "景点描述不完整!");
                ui->tabWidget->setCurrentIndex(0);
                return;
            }
        }
        createGraph();
        init();
        floyd();

        memset(visited, 0, sizeof(visited));
        int j = 0;
        for (int i = 0; i < graph.vercount; i++) {
            if (visited[i] != 1) {
                DFS(i);
                j++;
            }
        }
        if(j > 1){
            isConnected = false;
        }
        else{
            isConnected = true;
        }
        memset(visited, 0, sizeof(visited));
    });

    connect(ui->pushButtonGet, &QPushButton::clicked, this, &MainWindow::getPart);

    connect(ui->pushButtonSure1, &QPushButton::clicked, this, &MainWindow::changePart);

    connect(ui->pushButtonSure2, &QPushButton::clicked, this, &MainWindow::drawLine);

    connect(ui->pushButtonSure3, &QPushButton::clicked, this, &MainWindow::deleteLine);

    connect(ui->pushButtonSure4, &QPushButton::clicked, this, &MainWindow::deletePoint);

    readAndDraw();

    // 安装事件过滤器
    ui->labelImg->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->labelImg && event->type() == QEvent::MouseButtonPress)
    {

        QFile file(pointPath);
        if (file.open(QIODevice::ReadOnly)) {
            if (file.size() == 0) {
                index = 0;
            }
            file.close();
        }
        else {
            qDebug() << "Failed to open the file.";
        }


        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint clickPoint = mouseEvent->pos();

        // 获取 QLabel 上的画布
        QPixmap pixmap = ui->labelImg->pixmap(Qt::ReturnByValue);//这个出来的pixmap的width和height是原始图片大小，因为我把该图片放入qlabel中也是在代码中进行缩放的
        pixmap = pixmap.scaled(ui->labelImg->width(),ui->labelImg->height());


        // 判断是否与已存在的正方形重叠
        if (isOverlapping(pixmap, clickPoint)) {
            // 发出警告或执行其他操作
            QMessageBox::warning(this, "Warning", "与已有景点重叠!");
            return true; // 不再传递给默认的事件处理
        }

        // 创建绘图对象，并将其与画布关联
        QPainter painter(&pixmap);

        // 设置画笔颜色和宽度
        painter.setPen(QPen(Qt::black, 2));

        // 绘制正方形，相对于 QLabel
        painter.drawRect(clickPoint.x() - 10, clickPoint.y() - 10, 20, 20);

        // 在正方形内写入计数信息
        index++;
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawText(clickPoint.x()-10, clickPoint.y()-10, 20, 20, Qt::AlignCenter, QString::number(index));

        // 将绘制后的画布设置回 QLabel
        ui->labelImg->setPixmap(pixmap);

        // 在txt文件中保存坐标信息
        saveClickInfo(clickPoint);

        return true; // 拦截该事件，不再传递给默认的事件处理
    }

    return QObject::eventFilter(obj, event);
}


bool MainWindow::isOverlapping(QPixmap& pixmap, QPoint& newPoint) {
    // 创建绘图对象，并将其与画布关联
    QPainter painter(&pixmap);

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return 1;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {

        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        int index, x, y;
        index = fields[0].toInt();
        x = fields[1].toInt();
        y = fields[2].toInt();

        QRect existingRect(x - 20, y - 20, 40, 40);
        QRect newRect(newPoint.x() - 10, newPoint.y() - 10, 20, 20);

        // intersects 通常用于检测两个图形是否相交
        if (existingRect.intersects(newRect)) {
            return true; // 重叠
        }
    }

    return false; // 不重叠
}

void MainWindow::saveClickInfo(const QPoint &point) {

    // 打开文件，以追加方式写入
    QFile file(pointPath);
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream out(&file);
    out << index <<" " << point.x() << " " << point.y() << "\n";
    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readAndDraw() {

    QPixmap pixmap(":/new/image/nucMap.png");
    pixmap = pixmap.scaled(ui->labelImg->width(), ui->labelImg->height());
    QPainter painter(&pixmap);

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        index = fields[0].toInt();

        painter.setPen(QPen(Qt::black, 2));

        // 绘制正方形，相对于 QLabel
        painter.drawRect(fields[1].toInt() - 10, fields[2].toInt() - 10, 20, 20);

        // 在正方形内写入计数信息
        painter.setPen(QPen(Qt::blue, 2));

        painter.drawText(fields[1].toInt() - 10, fields[2].toInt() - 10, 20, 20, Qt::AlignCenter, QString::number(index));
    }
    file.close();

    QFile file1(edgePath);
    if (!file1.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in1(&file1);
    while (!in1.atEnd()) {
        QString line = in1.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawLine(fields[1].toInt(), fields[2].toInt() + 10, fields[4].toInt(), fields[5].toInt() + 10);
        painter.drawText((fields[1].toInt() + fields[4].toInt())/2, (fields[2].toInt() + fields[5].toInt())/2, fields[6]);
    }
    file1.close();

    // 将绘制后的画布设置回 QLabel
    ui->labelImg->setPixmap(pixmap);
}


void MainWindow::drawLine(){

    QString string1 = ui->lineEditStartAdd->text();
    QString string2 = ui->lineEditFinishAdd->text();
    QString string3 = ui->lineEditLengthPut->text();

    ui->lineEditStartAdd->clear();
    ui->lineEditFinishAdd->clear();
    ui->lineEditLengthPut->clear();

    if((string1 == "") || (string2 == "") || (string3 == "")){
        QMessageBox::warning(this, "Warning", "未完整输入!");
        return;
    }

    if(string1 == string2){
        QMessageBox::warning(this, "Warning", "结点相同!");
        return;
    }

    QFile file0(edgePath);
    if (!file0.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in0(&file0);
    while (!in0.atEnd()) {
        QString line = in0.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if((string1 == fields[0] && string2 == fields[3])||(string2 == fields[0] && string1 == fields[3])){
            QMessageBox::warning(this, "Warning", "重复!");
            return;
        }

    }
    file0.close();

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in(&file);
    int setX, setY, finishX, finishY;
    bool isFoundSet = false;
    bool isFoundFinish = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if(string1 == fields[0]){
            isFoundSet = true;
            setX = fields[1].toInt();
            setY = fields[2].toInt();
        }
        else if(string2 == fields[0]){
            isFoundFinish = true;
            finishX = fields[1].toInt();
            finishY = fields[2].toInt();
        }
    }

    file.close();

    if(!isFoundFinish || !isFoundSet){
        QMessageBox::warning(this, "Warning", "结点不存在!");
        return;
    }

    QPixmap pixmap = ui->labelImg->pixmap(Qt::ReturnByValue);
    pixmap = pixmap.scaled(ui->labelImg->width(), ui->labelImg->height());
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(setX, setY + 10, finishX, finishY + 10);
    painter.drawText((setX + finishX)/2, (setY + finishY)/2, string3);
    ui->labelImg->setPixmap(pixmap);

    QFile file1(edgePath);
    if (!file1.open(QIODevice::Append | QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream out(&file1);
    out << string1 <<" " << setX << " " << setY << " " << string2 << " " << finishX << " " << finishY << " " << string3 << "\n";
    file1.close();

}

void MainWindow::deleteLine(){
    QVector<QStringList> vector;

    QString string1 = ui->lineEditStartDel->text();
    QString string2 = ui->lineEditFinishDel->text();
    ui->lineEditStartDel->clear();
    ui->lineEditFinishDel->clear();

    if((string1 == "") || (string2 == "")){
        QMessageBox::warning(this, "Warning", "未输入");
        return;
    }

    if(string1 == string2){
        QMessageBox::warning(this, "Warning", "节点相同");
        return;
    }

    QFile file(edgePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    bool isFound = false;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if((string1 == fields[0] && string2 == fields[3])||(string2 == fields[0] && string1 == fields[3])){
            isFound = true;
            continue;
        }
        vector.append(fields);
    }
    file.close();

    if(!isFound){
        QMessageBox::warning(this, "Warning", "无该边");
        return;
    }

    QFile file1(edgePath);
    if (!file1.open(QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream out(&file1);
    for(auto& i : vector){
        for(auto& j : i){
            out << j << " ";
        }
        out << "\n";
    }
    file1.close();

    readAndDraw();

}


void MainWindow::deletePoint(){

    QVector<QStringList> vector;

    QString string1 = ui->lineEditDelSpotPut->text();
    ui->lineEditDelSpotPut->clear();

    if(string1 == ""){
        QMessageBox::warning(this, "Warning", "未输入");
        return;
    }


    QFile file(edgePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if((string1 == fields[0]) || (string1 == fields[3])){
            continue;
        }
        vector.append(fields);
    }
    file.close();


    QFile file1(edgePath);
    if (!file1.open(QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream out(&file1);
    for(auto& i : vector){
        for(auto& j : i){
            out << j << " ";
        }
        out << "\n";
    }
    file1.close();

    bool isFound = false;
    QFile file2(pointPath);
    if (!file2.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    vector.clear();
    QTextStream in2(&file2);
    while (!in2.atEnd()) {
        QString line = in2.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if(fields[0] == string1){
            isFound = true;
            continue;
        }
        vector.append(fields);
    }

    file2.close();

    if(!isFound){
        QMessageBox::warning(this, "Warning", "无该点");
        return;
    }

    QFile file3(pointPath);
    if (!file3.open(QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }
    QTextStream out3(&file3);
    for(auto& i : vector){
        for(auto& j : i){
            out3 << j << " ";
        }
        out3 << "\n";
    }

    file3.close();
    readAndDraw();
}


void MainWindow::getPart(){

    QString spot = ui->lineEditChangeSpot->text();

    if(spot == ""){
        QMessageBox::warning(this, "Warning", "未输入");
        return;
    }

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in(&file);
    bool isFound = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        if(fields[0] == spot){
            isFound = true;
            if(fields.size() < 5){
                QMessageBox::warning(this, "tip", "no description");
                return;
            }
            else if(fields.size() == 5){
                ui->lineEditNamePut->setText(fields[3]);
                ui->lineEditDesPut->setText(fields[4]);
                return;
            }
        }
    }
    if(!isFound){
        QMessageBox::warning(this, "warning", "no this node");
        return;
    }

    file.close();
}

void MainWindow::changePart(){

    QVector<QStringList> vector;
    QString id = ui->lineEditChangeSpot->text();
    QString name = ui->lineEditNamePut->text();
    QString des = ui->lineEditDesPut->text();

    ui->lineEditChangeSpot->clear();
    ui->lineEditNamePut->clear();
    ui->lineEditDesPut->clear();

    if(name == "" || des == ""){
        QMessageBox::warning(this, "warning", "未输入");
        return;
    }

    QFile file(pointPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "fail to open the file";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(QChar::Space, Qt::SkipEmptyParts);
        QStringList data;
        if(fields[0] == id){
            data ={fields[0], fields[1], fields[2], name, des};
            vector.append(data);
            continue;
        }
        vector.append(fields);
    }
    file.close();

    QFile file2(pointPath);
    if (!file2.open(QIODevice::WriteOnly)) {
        qDebug() << "fail to open the file";
        return;
    }
    QTextStream out2(&file2);
    for(auto& i : vector){
        for(auto& j : i){
            out2 << j << " ";
        }
        out2 << "\n";
    }
    file2.close();
}




