#include <QTextEdit>
#include <QTcpSocket>
#include <QMainWindow>
#include <QMap>
#include <QString>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage(QString msg);
    void readMessage();

private:
    QTcpSocket *socket;
    QTextEdit *messageDisplay;
    QMap<int, QString> keyMap;
    bool isConnecting = false;


protected:
    void keyPressEvent(QKeyEvent *event);
};
