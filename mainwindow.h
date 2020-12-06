#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static int counterSet1;
    static int counterSet2;

    static bool animationStateRpm;
    static bool animationStateBoost;

private slots:
    void on_toolButton_clicked();

    void on_pushButton_clicked();

    void set();

    void run();

    void on_actionAbout_triggered();

    void on_OK_clicked();

    void on_actionReadme_triggered();

    void on_runButton_clicked();

    void on_Back_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
