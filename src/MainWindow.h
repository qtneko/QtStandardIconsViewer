#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScrollArea>

class QSystemTrayIcon;
class QStyle;

class MainWindow : public QScrollArea
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QStyle              *_style = nullptr;
    QSystemTrayIcon     *_trayWidget = nullptr;

    void setupWidgets();
    void setupTrayWidget();
    void processBtnClick();
};
#endif // MAINWINDOW_H
