#include <QtNetwork>
#include <QPushButton>
#include <QTextEdit>
#include <QKeyEvent>
#include <QString>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isConnecting(false)
{

    keyMap[Qt::Key_W] = "W";
    keyMap[Qt::Key_A] = "A";
    keyMap[Qt::Key_S] = "S";
    keyMap[Qt::Key_D] = "D";
    keyMap[Qt::Key_Shift] = "Shift";

    // 设置主窗口大小
    setFixedSize(500, 160);

    // 显示内容
    messageDisplay = new QTextEdit(this);
    messageDisplay->setGeometry(QRect(QPoint(0, 0), QSize(500, 100)));
    messageDisplay->setReadOnly(true);

    // 创建连接按钮
    QPushButton *connectButton = new QPushButton(this);
    connectButton->setText("Connect");
    connectButton->setGeometry(QRect(QPoint(200, 120), QSize(100, 30)));

    socket = new QTcpSocket(this);

    // 连接按钮点击事件
    connect(connectButton, &QPushButton::clicked, this, [=]() {
        if (!isConnecting)
        {
            isConnecting = true;
            connectButton->setEnabled(false);
            socket->connectToHost("127.0.0.1", 8888);
        }
    });

    connect(socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << "Connected to server.";
        isConnecting = false;
        connectButton->setEnabled(true);
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "Disconnected from server.";
        isConnecting = false;
        connectButton->setEnabled(true);
    });

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readMessage);
}

MainWindow::~MainWindow() {
    // 结束连接
    socket->disconnectFromHost();
    delete socket;
}

void MainWindow::sendMessage(QString msg) {
    QByteArray data = msg.toUtf8();
    socket->write(data);
}

void MainWindow::readMessage() {
    while (socket->bytesAvailable() > 0)
    {
        QByteArray data = socket->readAll();
        messageDisplay->append(QString(data));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // 如果按下的键是WASD或空格键
    if (keyMap.contains(event->key()) && socket->state() == QAbstractSocket::ConnectedState) {
        // 发送消息
        sendMessage(keyMap[event->key()]);
        // 等待数据发送完成
        if (!socket->waitForBytesWritten())
        {
            qDebug() << "Error: " << socket->errorString();
            return;
        }

    }
}
