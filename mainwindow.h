#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QNetworkAccessManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //void on_draw_full_set_released();
    void check_files();

    void pick_adaptive();

    void pick_keystone_and_keystone_runes();

    void pick_secondary_keystone_and_keystone_runes();

    void pick_champions_andor_keystone_andor_secondary_rune();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_2_currentIndexChanged();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *mManager;
};
#endif // MAINWINDOW_H
