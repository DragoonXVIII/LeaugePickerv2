#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <QTextStream>
#include <QDebug>
#include <random>
#include <windows.h>
#include <time.h>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <QProcess>
#include <QSslCertificate>
#include <QSslSocket>



//GOALS:
// - pick for any number of players (changed for position)                           yes
// - gui                                                                             yes
// - runki (bez tych samych run na losowanie)                                        yes
// - zczytywanie z txt                                                               yes
// - custom zestawy tworzone z gui                                                   to do
// - dodac funkcje na zczytywanie z txt to updaty bedzie mozna recznie robic es      yes
// - animacja losowania                                                              to do
// - full runki                                                                      yes
// - porobić akcje na plikach jakoi globalne funkcje                                 yes (not sure so work in progress)
// - skrócić te nazwy cringeowe fest strasznie serio xd ( bo sie w labelach lanka nie mieszczom) xd


//+=== Global functions that for some reason have to be here ===+



QList<QString> chosenoption(QString path)
{
    QList<QString> champs_to_rand;
    QFile chosen_set(path);
    if (chosen_set.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&chosen_set);
        while (!stream.atEnd())
        {
            champs_to_rand.append(stream.readLine());
        }
    }
    chosen_set.close();
return champs_to_rand;
}



QList<QString> directories_of_secondary_rune_tab(int i)
{

    QList<QString> list_of_tabs_directories;

    int Randompickv8_4(int length);

    switch(i)
    {

    case 0://Domination
        list_of_tabs_directories = {":/Keystone/Domination/tab_1/Runes/Basic_runes/Domination/malice/",
                                    ":/Keystone/Domination/tab_2/Runes/Basic_runes/Domination/tracking/",
                                    ":/Keystone/Domination/tab_3/Runes/Basic_runes/Domination/hunter/"};
    break;

    case 1://Inspiration
        list_of_tabs_directories = {":/Keystone/Inspiration/tab_1/Runes/Basic_runes/Inspiration/contraptions/",
                                    ":/Keystone/Inspiration/tab_2/Runes/Basic_runes/Inspiration/tomorrow/",
                                    ":/Keystone/Inspiration/tab_3/Runes/Basic_runes/Inspiration/beyond/"};
    break;

    case 2://Precision
        list_of_tabs_directories = {":/Keystone/Precision/tab_1/Runes/Basic_runes/Precision/heroism/",
                                    ":/Keystone/Precision/tab_2/Runes/Basic_runes/Precision/legend/",
                                    ":/Keystone/Precision/tab_3/Runes/Basic_runes/Precision/combat/"};
    break;

    case 3://Resolve
        list_of_tabs_directories = {":/Keystone/Resolve/tab_1/Runes/Basic_runes/Resolve/strength/",
                                    ":/Keystone/Resolve/tab_2/Runes/Basic_runes/Resolve/resistance/",
                                    ":/Keystone/Resolve/tab_3/Runes/Basic_runes/Resolve/vitality/"};
    break;

    case 4://Sorcery
        list_of_tabs_directories = {":/Keystone/Sorcery/tab_1/Runes/Basic_runes/Sorcery/artifact/",
                                    ":/Keystone/Sorcery/tab_2/Runes/Basic_runes/Sorcery/excellence/",
                                    ":/Keystone/Sorcery/tab_3/Runes/Basic_runes/Sorcery/power/"};
    break;

    }

    return list_of_tabs_directories;
}





//+=== Global Variables ===+



int ii,iii;                                                         //Randompickv8() non-repeat variable
int tree_of_runes,secondary_tree_of_runes_tab,tab_anty_repeat;      //& runes non-reapeat variable
int index_of_page=4,index_of_previous_page;

int id_of_runes[10]; // 0 - main rune, 1 - m 1, 2 - m 2, 3 - m 3, 4 - secondary rune, 5 - s 1, 6 - s 2, 7 - a 1, 8 - a 2, 9 - a 3;




//+=== QT class ===+



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QSslSocket::setLocalCertificate(QSslCertificate::fromPath(":/sets/sets/riotgames.pem"));



    ui->stackedWidget->setCurrentIndex(3);
    MainWindow::check_files();
    MainWindow::pick_champions_andor_keystone_andor_secondary_rune();
    MainWindow::pick_keystone_and_keystone_runes();
    MainWindow::pick_secondary_keystone_and_keystone_runes();
    MainWindow::pick_adaptive();
    MainWindow::on_comboBox_2_currentIndexChanged();

    QPixmap pix(":/Icons/Icons/Logo.png");
    ui->label_5->setPixmap(pix);
    ui->label_5->setScaledContents(true);



    mManager = new QNetworkAccessManager(this);
    connect(mManager, &QNetworkAccessManager::finished, this, [&](QNetworkReply *reply)
    {
        QByteArray data = reply->readAll();
        QString dataString = QString(data);
        qDebug()<<dataString;

        QRegularExpression regex = QRegularExpression("\"id\":(.*?),");
        QString id = regex.match(dataString).captured(1);
        qDebug() << id;

        // post request


    });

}

MainWindow::~MainWindow()
{

    delete ui;
}


//+===================OTHER THAN MAINWINDOW XD===================+
void MainWindow::check_files()
{
    QList<QString> sets = {"ranked_champs.txt", "troll_champs.txt", "custom_set_1.txt", "custom_set_2.txt"};
    for(int i=0;i<4;i++)
    {
        bool does_exist=TRUE;
        QFile set_txt(sets[i]);

        if(!set_txt.exists())
        {
            does_exist=FALSE;
        }

        if(set_txt.open(QIODevice::ReadWrite))
        {
            if(!does_exist)
            {
                QTextStream stream(&set_txt);
                stream<<"Yasuo.png\n"
                        "Yone.png\n"
                        "MasterYi.png\n"
                        "Twitch.png\n";
            }

        }
        set_txt.close();
    }

    //qCritical()<<ranked_champs_txt.errorString();

}



/*void MainWindow::on_draw_full_set_released()
{

    MainWindow::pick_adaptive();

    MainWindow::pick_keystone_and_keystone_runes();

    MainWindow::pick_secondary_keystone_and_keystone_runes();

} */



void MainWindow::on_pushButton_6_clicked()
{

    //qDebug()<<(ui->stackedWidget->currentIndex());
    if(ui->stackedWidget->currentIndex()==0)
    {
        MainWindow::pick_champions_andor_keystone_andor_secondary_rune();
    }
    if(ui->stackedWidget->currentIndex()==1)
    {
        MainWindow::pick_adaptive();

        MainWindow::pick_keystone_and_keystone_runes();

        MainWindow::pick_secondary_keystone_and_keystone_runes();
    }
    //MainWindow::pick_champions_andor_keystone_andor_secondary_rune();

}



void MainWindow::pick_adaptive()
{

    int adaptive_1,adaptive_2,adaptive_3;
    int Randompickv8(int chosenarraylength);

    //int id_of_runes[10]; // 0 - main rune, 1 - m 1, 2 - m 2, 3 - m 3, 4 - secondary rune, 5 - s 1, 6 - s 2, 7 - a 1, 8 - a 2, 9 - a 3;

    QDir directory_of_adaptive_1(":/Adaptive/tab1/Runes/Adaptive/1tab/");
    QDir directory_of_adaptive_2(":/Adaptive/tab2/Runes/Adaptive/2tab/");
    QDir directory_of_adaptive_3(":/Adaptive/tab3/Runes/Adaptive/3tab/");

    QList<QString> adaptives_to_chose_from_1 = directory_of_adaptive_1.entryList(QStringList(),QDir::Files);
    QList<QString> adaptives_to_chose_from_2 = directory_of_adaptive_2.entryList(QStringList(),QDir::Files);
    QList<QString> adaptives_to_chose_from_3 = directory_of_adaptive_3.entryList(QStringList(),QDir::Files);

    adaptive_1 = Randompickv8(adaptives_to_chose_from_1.length());
    adaptive_2 = Randompickv8(adaptives_to_chose_from_2.length());
    adaptive_3 = Randompickv8(adaptives_to_chose_from_3.length());

    //qDebug()<<adaptive_1;
    //qDebug()<<adaptive_2;
    //qDebug()<<adaptive_3;

    QString selected_adaptive_1_name = (adaptives_to_chose_from_1[adaptive_1]);
    QString selected_adaptive_2_name = (adaptives_to_chose_from_2[adaptive_2]);
    QString selected_adaptive_3_name = (adaptives_to_chose_from_3[adaptive_3]);

    QString selected_adaptive_1 = (":/Adaptive/tab1/Runes/Adaptive/1tab/"+selected_adaptive_1_name);
    QString selected_adaptive_2 = (":/Adaptive/tab2/Runes/Adaptive/2tab/"+selected_adaptive_2_name);
    QString selected_adaptive_3 = (":/Adaptive/tab3/Runes/Adaptive/3tab/"+selected_adaptive_3_name);

    QImage adaptive_image_1;
    QImage adaptive_image_2;
    QImage adaptive_image_3;

    adaptive_image_1.load(selected_adaptive_1);
    adaptive_image_2.load(selected_adaptive_2);
    adaptive_image_3.load(selected_adaptive_3);

    ui->label_31->setPixmap(QPixmap::fromImage(adaptive_image_1));
    ui->label_29->setPixmap(QPixmap::fromImage(adaptive_image_2));
    ui->label_27->setPixmap(QPixmap::fromImage(adaptive_image_3));

    ui->label_31->setScaledContents(true);
    ui->label_29->setScaledContents(true);
    ui->label_27->setScaledContents(true);

    selected_adaptive_1_name.remove(selected_adaptive_1_name.length()-8,8);
    selected_adaptive_2_name.remove(selected_adaptive_2_name.length()-8,8);
    selected_adaptive_3_name.remove(selected_adaptive_3_name.length()-8,8);

    ui->label_30->setText(selected_adaptive_1_name);
    ui->label_28->setText(selected_adaptive_2_name);
    ui->label_26->setText(selected_adaptive_3_name);
}



void MainWindow::pick_keystone_and_keystone_runes()
{

    int Randompickv8(int chosenarraylength);
    int Randompickv8_2(int chosenarraylength);

    int picked_tree_of_runes_no = Randompickv8_2(5);



    QString QDir_directory_of_keystone_tab_1;
    QString QDir_directory_of_keystone_tab_2;
    QString QDir_directory_of_keystone_tab_3;
    QString QDir_directory_of_keystone;
    QString QDir_directory_of_rune_tree;
    QString selected_rune_tree;



    switch(picked_tree_of_runes_no)
    {

        case 0://Domination
        selected_rune_tree = "Domination";
        tree_of_runes = 0;

        QDir_directory_of_keystone_tab_1=":/Keystone/Domination/tab_1/Runes/Basic_runes/Domination/malice/";
        QDir_directory_of_keystone_tab_2=":/Keystone/Domination/tab_2/Runes/Basic_runes/Domination/tracking/";
        QDir_directory_of_keystone_tab_3=":/Keystone/Domination/tab_3/Runes/Basic_runes/Domination/hunter/";
        QDir_directory_of_keystone=":/Keystone/Domination/keystone/Runes/Basic_runes/Domination/keystone/";
        QDir_directory_of_rune_tree=":/Keystone/Domination/Runes/Basic_runes/Domination/Domination.png";
        break;

        case 1://Inspiration
        selected_rune_tree = "Inspiration";
        tree_of_runes = 1;

        QDir_directory_of_keystone_tab_1=":/Keystone/Inspiration/tab_1/Runes/Basic_runes/Inspiration/contraptions/";
        QDir_directory_of_keystone_tab_2=":/Keystone/Inspiration/tab_2/Runes/Basic_runes/Inspiration/tomorrow/";
        QDir_directory_of_keystone_tab_3=":/Keystone/Inspiration/tab_3/Runes/Basic_runes/Inspiration/beyond/";
        QDir_directory_of_keystone=":/Keystone/Inspiration/keystone/Runes/Basic_runes/Inspiration/keystone/";
        QDir_directory_of_rune_tree=":/Keystone/Inspiration/Runes/Basic_runes/Inspiration/Whimsy.png";
        break;

        case 2://Precision
        selected_rune_tree = "Precision";
        tree_of_runes = 2;

        QDir_directory_of_keystone_tab_1=":/Keystone/Precision/tab_1/Runes/Basic_runes/Precision/heroism/";
        QDir_directory_of_keystone_tab_2=":/Keystone/Precision/tab_2/Runes/Basic_runes/Precision/legend/";
        QDir_directory_of_keystone_tab_3=":/Keystone/Precision/tab_3/Runes/Basic_runes/Precision/combat/";
        QDir_directory_of_keystone=":/Keystone/Precision/keystone/Runes/Basic_runes/Precision/keystone/";
        QDir_directory_of_rune_tree=":/Keystone/Precision/Runes/Basic_runes/Precision/Precision.png";
        break;

        case 3://Resolve
        selected_rune_tree = "Resolve";
        tree_of_runes = 3;

        QDir_directory_of_keystone_tab_1=":/Keystone/Resolve/tab_1/Runes/Basic_runes/Resolve/strength/";
        QDir_directory_of_keystone_tab_2=":/Keystone/Resolve/tab_2/Runes/Basic_runes/Resolve/resistance/";
        QDir_directory_of_keystone_tab_3=":/Keystone/Resolve/tab_3/Runes/Basic_runes/Resolve/vitality/";
        QDir_directory_of_keystone=":/Keystone/Resolve/keystone/Runes/Basic_runes/Resolve/keystone/";
        QDir_directory_of_rune_tree=":/Keystone/Resolve/Runes/Basic_runes/Resolve/Resolve.png";
        break;

        case 4://Sorcery
        selected_rune_tree = "Sorcery";
        tree_of_runes = 4;

        QDir_directory_of_keystone_tab_1=":/Keystone/Sorcery/tab_1/Runes/Basic_runes/Sorcery/artifact/";
        QDir_directory_of_keystone_tab_2=":/Keystone/Sorcery/tab_2/Runes/Basic_runes/Sorcery/excellence/";
        QDir_directory_of_keystone_tab_3=":/Keystone/Sorcery/tab_3/Runes/Basic_runes/Sorcery/power/";
        QDir_directory_of_keystone=":/Keystone/Sorcery/keystone/Runes/Basic_runes/Sorcery/keystone/";
        QDir_directory_of_rune_tree=":/Keystone/Sorcery/Runes/Basic_runes/Sorcery/Sorcery.png";
        break;

    }

    QDir directory_of_keystone_tab_1(QDir_directory_of_keystone_tab_1);
    QDir directory_of_keystone_tab_2(QDir_directory_of_keystone_tab_2);
    QDir directory_of_keystone_tab_3(QDir_directory_of_keystone_tab_3);
    QDir directory_of_keystones(QDir_directory_of_keystone);

    QList<QString> keystone_tab_1_to_chose_from = directory_of_keystone_tab_1.entryList(QStringList(),QDir::Files);
    QList<QString> keystone_tab_2_to_chose_from = directory_of_keystone_tab_2.entryList(QStringList(),QDir::Files);
    QList<QString> keystone_tab_3_to_chose_from = directory_of_keystone_tab_3.entryList(QStringList(),QDir::Files);
    QList<QString> keystones_to_chose_from = directory_of_keystones.entryList(QStringList(),QDir::Files);

    QString selected_keystone_tab_1_name = (keystone_tab_1_to_chose_from[Randompickv8(keystone_tab_1_to_chose_from.length())]);
    QString selected_keystone_tab_2_name = (keystone_tab_2_to_chose_from[Randompickv8(keystone_tab_2_to_chose_from.length())]);
    QString selected_keystone_tab_3_name = (keystone_tab_3_to_chose_from[Randompickv8(keystone_tab_3_to_chose_from.length())]);
    QString selected_keystone_name = (keystones_to_chose_from[Randompickv8(keystones_to_chose_from.length())]);

    QString selected_keystone_tab_1 = (QDir_directory_of_keystone_tab_1+selected_keystone_tab_1_name);
    QString selected_keystone_tab_2 = (QDir_directory_of_keystone_tab_2+selected_keystone_tab_2_name);
    QString selected_keystone_tab_3 = (QDir_directory_of_keystone_tab_3+selected_keystone_tab_3_name);
    QString selected_keystone_tab = (QDir_directory_of_keystone+selected_keystone_name);

    QImage keystone_tab_1_image;
    QImage keystone_tab_2_image;
    QImage keystone_tab_3_image;

    QImage rune_tree_image;
    QImage keystone_image;

    keystone_tab_1_image.load(selected_keystone_tab_1);
    keystone_tab_2_image.load(selected_keystone_tab_2);
    keystone_tab_3_image.load(selected_keystone_tab_3);

    rune_tree_image.load(QDir_directory_of_rune_tree);
    keystone_image.load(selected_keystone_tab);

    ui->label_20->setPixmap(QPixmap::fromImage(keystone_tab_1_image));
    ui->label_15->setPixmap(QPixmap::fromImage(keystone_tab_2_image));
    ui->label_22->setPixmap(QPixmap::fromImage(keystone_tab_3_image));

    ui->label_21->setPixmap(QPixmap::fromImage(rune_tree_image));
    ui->label_18->setPixmap(QPixmap::fromImage(keystone_image));

    ui->label_20->setScaledContents(true);
    ui->label_15->setScaledContents(true);
    ui->label_22->setScaledContents(true);

    ui->label_21->setScaledContents(true);
    ui->label_18->setScaledContents(true);

    selected_keystone_tab_1_name.remove(selected_keystone_tab_1_name.length()-4,4);
    selected_keystone_tab_2_name.remove(selected_keystone_tab_2_name.length()-4,4);
    selected_keystone_tab_3_name.remove(selected_keystone_tab_3_name.length()-4,4);

    selected_keystone_name.remove(selected_keystone_name.length()-4,4);

    ui->label_16->setText(selected_rune_tree);
    ui->label_17->setText(selected_keystone_name);

    ui->label_19->setText(selected_keystone_tab_1_name);
    ui->label_14->setText(selected_keystone_tab_2_name);
    ui->label_23->setText(selected_keystone_tab_3_name);


}



void MainWindow::pick_secondary_keystone_and_keystone_runes()
{

    int Randompickv8_2(int chosenarraylength);
    int Randompickv8_3(int chosenarraylength);



    int picked_tree_of_runes_no = Randompickv8_2(5);

    // nazwa w txt -> zamiana na numer -> wrzucenie do tablicy

    QString secondary_tree_of_runes_name;

    QString QDir_directory_of_rune_tree;
    QString QDir_directory_of_secondary_keystone_tab_1;
    QString QDir_directory_of_secondary_keystone_tab_2;



    switch(picked_tree_of_runes_no)
    {

    case 0://Domination
    secondary_tree_of_runes_name = "Domination";

    QDir_directory_of_rune_tree = ":/Keystone/Domination/Runes/Basic_runes/Domination/Domination.png";
    QDir_directory_of_secondary_keystone_tab_1 = directories_of_secondary_rune_tab(0).at(Randompickv8_3(3));
    QDir_directory_of_secondary_keystone_tab_2 = directories_of_secondary_rune_tab(0).at(Randompickv8_3(3));
    // 0 - main rune, 1 - m 1, 2 - m 2, 3 - m 3, 4 - secondary rune, 5 - s 1, 6 - s 2, 7 - a 1, 8 - a 2, 9 - a 3;
    break;

    case 1://Inspiration
    secondary_tree_of_runes_name = "Inspiration";

    QDir_directory_of_rune_tree = ":/Keystone/Inspiration/Runes/Basic_runes/Inspiration/Whimsy.png";
    QDir_directory_of_secondary_keystone_tab_1 = directories_of_secondary_rune_tab(1).at(Randompickv8_3(3));
    QDir_directory_of_secondary_keystone_tab_2 = directories_of_secondary_rune_tab(1).at(Randompickv8_3(3));
    break;

    case 2://Precision
    secondary_tree_of_runes_name = "Precision";

    QDir_directory_of_rune_tree = ":/Keystone/Precision/Runes/Basic_runes/Precision/Precision.png";
    QDir_directory_of_secondary_keystone_tab_1 = directories_of_secondary_rune_tab(2).at(Randompickv8_3(3));
    QDir_directory_of_secondary_keystone_tab_2 = directories_of_secondary_rune_tab(2).at(Randompickv8_3(3));
    break;

    case 3://Resolve
    secondary_tree_of_runes_name = "Resolve";

    QDir_directory_of_rune_tree = ":/Keystone/Resolve/Runes/Basic_runes/Resolve/Resolve.png";
    QDir_directory_of_secondary_keystone_tab_1 = directories_of_secondary_rune_tab(3).at(Randompickv8_3(3));
    QDir_directory_of_secondary_keystone_tab_2 = directories_of_secondary_rune_tab(3).at(Randompickv8_3(3));
    break;

    case 4://Sorcery
    secondary_tree_of_runes_name = "Sorcery";

    QDir_directory_of_rune_tree = ":/Keystone/Sorcery/Runes/Basic_runes/Sorcery/Sorcery.png";
    QDir_directory_of_secondary_keystone_tab_1 = directories_of_secondary_rune_tab(4).at(Randompickv8_3(3));
    QDir_directory_of_secondary_keystone_tab_2 = directories_of_secondary_rune_tab(4).at(Randompickv8_3(3));
    break;
    }



    QDir directory_of_tab_1(QDir_directory_of_secondary_keystone_tab_1);
    QDir directory_of_tab_2(QDir_directory_of_secondary_keystone_tab_2);

    QList<QString> secondary_keystone_tab_1_to_chose_from = directory_of_tab_1.entryList(QStringList(),QDir::Files);
    QList<QString> secondary_keystone_tab_2_to_chose_from = directory_of_tab_2.entryList(QStringList(),QDir::Files);

    QString selected_secondary_keystone_tab_1_name = secondary_keystone_tab_1_to_chose_from[Randompickv8_3(secondary_keystone_tab_1_to_chose_from.length())];
    QString selected_secondary_keystone_tab_2_name = secondary_keystone_tab_2_to_chose_from[Randompickv8_3(secondary_keystone_tab_2_to_chose_from.length())];

    QString selected_secondary_keystone_tab_1 = (QDir_directory_of_secondary_keystone_tab_1+selected_secondary_keystone_tab_1_name);
    QString selected_secondary_keystone_tab_2 = (QDir_directory_of_secondary_keystone_tab_2+selected_secondary_keystone_tab_2_name);

    QImage rune_tree_image;

    QImage secondary_keystone_tab_1_image;
    QImage secondary_keystone_tab_2_image;

    rune_tree_image.load(QDir_directory_of_rune_tree);

    secondary_keystone_tab_1_image.load(selected_secondary_keystone_tab_1);
    secondary_keystone_tab_2_image.load(selected_secondary_keystone_tab_2);

    ui->label_25->setPixmap(QPixmap::fromImage(rune_tree_image));

    ui->label_35->setPixmap(QPixmap::fromImage(secondary_keystone_tab_1_image));
    ui->label_33->setPixmap(QPixmap::fromImage(secondary_keystone_tab_2_image));

    ui->label_25->setScaledContents(true);

    ui->label_35->setScaledContents(true);
    ui->label_33->setScaledContents(true);

    selected_secondary_keystone_tab_1_name.remove(selected_secondary_keystone_tab_1_name.length()-4,4);
    selected_secondary_keystone_tab_2_name.remove(selected_secondary_keystone_tab_2_name.length()-4,4);

    ui->label_24->setText(secondary_tree_of_runes_name);
    ui->label_34->setText(selected_secondary_keystone_tab_1_name);
    ui->label_32->setText(selected_secondary_keystone_tab_2_name);

}



void MainWindow::pick_champions_andor_keystone_andor_secondary_rune()
{
    int Randompickv8(int chosenarraylength); //champion
    int Randompickv8_1(int chosenarraylength); //runes(both)

    QList<QString> chosen_set(int index);



    //Declarations:

    QList<QString> champions_to_chose_from;
    QList<QString> positions_to_chose_from;

    QList<QString> keystone_to_chose_from;

    QString selected_champ;
    QString selected_keystone;
    QString selected_secondary;
    QString selected_position;

    QString directory_of_selected_champ;
    QString directory_of_selected_position;
    QString directory_of_selected_keystone;
    QString directory_of_selected_secondary;

    QImage selected_champ_image;
    QImage selected_position_image;
    QImage selected_keystone_image;
    QImage selected_secondary_image;



    if(true) //ui->checkBox_2->isChecked()||ui->checkBox->isChecked() //previous version that is not supported now
    {

        switch(Randompickv8_1(4))
        {

        case 0:
        directory_of_selected_keystone = ":/Keystone/Domination/keystone/Runes/Basic_runes/Domination/keystone/";
        break;

        case 1:
        directory_of_selected_keystone = ":/Keystone/Inspiration/keystone/Runes/Basic_runes/Inspiration/keystone/";
        break;

        case 2:
        directory_of_selected_keystone = ":/Keystone/Precision/keystone/Runes/Basic_runes/Precision/keystone/";
        break;

        case 3:
        directory_of_selected_keystone = ":/Keystone/Resolve/keystone/Runes/Basic_runes/Resolve/keystone/";
        break;

        case 4:
        directory_of_selected_keystone = ":/Keystone/Sorcery/keystone/Runes/Basic_runes/Sorcery/keystone/";
        break;
        }

        switch(Randompickv8_1(4))
        {

        case 0:
        directory_of_selected_secondary = ":/Keystone/Domination/Runes/Basic_runes/Domination/Domination.png";
        selected_secondary = "Domination";
        break;

        case 1:
        directory_of_selected_secondary = ":/Keystone/Inspiration/Runes/Basic_runes/Inspiration/Whimsy.png";
        selected_secondary = "Whimsy";
        break;

        case 2:
        directory_of_selected_secondary = ":/Keystone/Precision/Runes/Basic_runes/Precision/Precision.png";
        selected_secondary = "Precision";
        break;

        case 3:
        directory_of_selected_secondary = ":/Keystone/Resolve/Runes/Basic_runes/Resolve/Resolve.png";
        selected_secondary = "Resolve";
        break;

        case 4:
        directory_of_selected_secondary = ":/Keystone/Sorcery/Runes/Basic_runes/Sorcery/Sorcery.png";
        selected_secondary = "Sorcery";
        break;
        }
     }



    //Pick champ:

    if(true)
    {
        champions_to_chose_from = chosen_set(ui->comboBox->currentIndex());
        selected_champ = champions_to_chose_from[Randompickv8(champions_to_chose_from.length())];
        directory_of_selected_champ = (":/Champions/champion/"+selected_champ);
        selected_champ_image.load(directory_of_selected_champ);
    }



    //Pick position:

    if(true) //ui->checkBox_3->isChecked()
    {
        QDir directory_of_positions(":/Position/position/");
        positions_to_chose_from = directory_of_positions.entryList(QStringList(),QDir::Files);
        selected_position = positions_to_chose_from[Randompickv8_1(positions_to_chose_from.length())];
        directory_of_selected_position = (":/Position/position/"+selected_position);
        selected_position_image.load(directory_of_selected_position);
    }
    else //prevoius version, that is not supported now
    {
        selected_position_image.load(NULL);
    }



    //Pick runes:

    if(true)//keystone //ui->checkBox->isChecked()
    {
        QDir directory_of_keystone_to_chose_from(directory_of_selected_keystone);
        keystone_to_chose_from = directory_of_keystone_to_chose_from.entryList(QStringList(),QDir::Files);
        selected_keystone = keystone_to_chose_from[Randompickv8_1(keystone_to_chose_from.length())];
        directory_of_selected_keystone += selected_keystone;
        selected_keystone_image.load(directory_of_selected_keystone);
    }
    else //prevoius version, that is not supported now
    {
        selected_keystone  = NULL;
        selected_keystone_image.load(NULL);
        //selected_keystone_image.load(":/Icons/Icons/vwk964k1e4z51.png");
    }


    if(true)//secondary //ui->checkBox_2->isChecked()
    {
        selected_secondary_image.load(directory_of_selected_secondary);
    }
    else //prevoius version, that is not supported now
    {
        selected_secondary  = NULL;
        selected_secondary_image.load(NULL);
        //selected_secondary_image.load(":/Icons/Icons/vwk964k1e4z51.png");
    }



    //Actions on gui:

    if(ui->checkBox->isChecked())//keystone_UI_checker
    {
        ui->label_2->setVisible(true);
        ui->label_11->setVisible(true);
    }
    else
    {
        ui->label_2->setVisible(false);
        ui->label_11->setVisible(false);
    }

    if(ui->checkBox_2->isChecked())//secondary_tree_UI_checker
    {
        ui->label_4->setVisible(true);
        ui->label_13->setVisible(true);
    }
    else
    {
        ui->label_4->setVisible(false);
        ui->label_13->setVisible(false);
    }

    if(ui->checkBox_3->isChecked())//position_UI_checker
    {
        ui->label_6->setVisible(true);
        ui->label_12->setVisible(true);
    }
    else
    {
        ui->label_6->setVisible(false);
        ui->label_12->setVisible(false);
    }

    ui->label_2->setPixmap(QPixmap::fromImage(selected_keystone_image));
    ui->label_4->setPixmap(QPixmap::fromImage(selected_secondary_image));
    ui->label->setPixmap(QPixmap::fromImage(selected_champ_image));
    ui->label_6->setPixmap(QPixmap::fromImage(selected_position_image));

    selected_champ.remove(selected_champ.length()-4,4);
    selected_position.remove(selected_position.length()-4,4);
    selected_keystone.remove(selected_keystone.length()-4,4);

    ui ->label_37->setText(selected_champ);
    ui ->label_12->setText(selected_position);
    ui ->label_11->setText(selected_keystone);
    ui ->label_13->setText(selected_secondary);

    ui->label_6->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    ui->label_4->setScaledContents(true);
}



void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_40->setText("Champions: ");
    ui->pushButton_6->setText("Click for random champion!");
}



void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->label_40->setText("Settings: ");
}



void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_40->setText("Runes: ");
    ui->pushButton_6->setText("Click for set full of inting!");
}



void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->label_40->setText("Info: ");
}



void MainWindow::on_checkBox_3_clicked()
{
    if(ui->checkBox_3->checkState())
    {
        ui->label_6->show();
        ui->label_12->show();
    }
    else
    {
        ui->label_6->hide();
        ui->label_12->hide();
    }
}



void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->checkState())
    {
        ui->label_2->show();
        ui->label_11->show();
    }
    else
    {
        ui->label_2->hide();
        ui->label_11->hide();
    }
}



void MainWindow::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->checkState())
    {
        ui->label_4->show();
        ui->label_13->show();
    }
    else
    {
        ui->label_4->hide();
        ui->label_13->hide();
    }
}





//+=== Additional Functions ===+



int Randompickv8(int length)
{
    int i;
    while(true)
    {
       std::random_device rd;
       std::mt19937 mt(rd());
       std::uniform_real_distribution <double> dist(0,length);
       i=dist(mt);
       while(i==iii)
       {
           i=dist(mt);
       }
       //std::cout<<i<<std::endl;
       iii=i;
           return i;
    }
}



int Randompickv8_1(int length)
{
    int i;
    while(true)
    {
       std::random_device rd;
       std::mt19937 mt(rd());
       std::uniform_real_distribution <double> dist(0,length);
       i=dist(mt);
       while(i==ii)
       {
           i=dist(mt);
       }
       //std::cout<<i<<std::endl;
       ii=i;
           return i;
    }
}



int Randompickv8_2(int length)
{
    int i;
    while(true)
    {
       std::random_device rd;
       std::mt19937 mt(rd());
       std::uniform_real_distribution <double> dist(0,length);
       i=dist(mt);
       while(i==tree_of_runes)
       {
           i=dist(mt);
       }
       //std::cout<<i<<std::endl;
       tree_of_runes=i;
           return i;
    }
}



int Randompickv8_3(int length)
{
    int i;
    while(true)
    {
       std::random_device rd;
       std::mt19937 mt(rd());
       std::uniform_real_distribution <double> dist(0,length);
       i=dist(mt);
       while(i==secondary_tree_of_runes_tab)
       {
           i=dist(mt);
       }
       //std::cout<<i<<std::endl;
       secondary_tree_of_runes_tab=i;
           return i;
    }
}



int Randompickv8_4(int length)
{
    int i;
    while(true)
    {
       std::random_device rd;
       std::mt19937 mt(rd());
       std::uniform_real_distribution <double> dist(0,length);
       i=dist(mt);
       while(i==tab_anty_repeat)
       {
           i=dist(mt);
       }
       //std::cout<<i<<std::endl;
       tab_anty_repeat=i;
           return i;
    }
}



bool runes_validation_check(QString selected_secondary_runes, QString keystone_runes_image)
{
    if(selected_secondary_runes=="secondary_runes/7200_Domination.png")
    {
        if(keystone_runes_image=="keystone_runes/DarkHarvest.png"||keystone_runes_image=="keystone_runes/Electrocute.png"||keystone_runes_image=="keystone_runes/Predator.png"||keystone_runes_image=="keystone_runes/HailOfBlades.png")
            return false;
        else return true;
    }
    if(selected_secondary_runes=="secondary_runes/7201_Precision.png")
    {
        if(keystone_runes_image=="keystone_runes/Conqueror.png"||keystone_runes_image=="keystone_runes/FleetFootwork.png"||keystone_runes_image=="keystone_runes/LethalTempoTemp.png"||keystone_runes_image=="keystone_runes/PressTheAttack.png")
            return false;
        else return true;
    }
    if(selected_secondary_runes=="secondary_runes/7203_Whimsy.png")
    {
        if(keystone_runes_image=="keystone_runes/UnsealedSpellbook.png"||keystone_runes_image=="FirstStrike.png"||keystone_runes_image=="GlacialAugment.png")
            return false;
        else return true;
    }
    if(selected_secondary_runes=="secondary_runes/7202_Sorcery.png")
    {
        if(keystone_runes_image=="keystone_runes/ArcaneComet.png"||keystone_runes_image=="keystone_runes/PhaseRush.png"||keystone_runes_image=="keystone_runes/SummonAery.png")
            return false;
        else return true;
    }
    if(selected_secondary_runes=="secondary_runes/7204_Resolve.png")
    {
        if(keystone_runes_image=="keystone_runes/Guardian.png"||keystone_runes_image=="keystone_runes/VeteranAftershock.png"||keystone_runes_image=="keystone_runes/GraspOfTheUndying.png")
            return false;
        else return true;
    }
    return false;
}



QList<QString> chosen_set(int index)
{

    QString QDir_directory_of_chosen_list;

    switch(index)
    {

    case 0:
    QDir_directory_of_chosen_list = ":/sets/sets/all_champs.txt";
    break;

    case 1:
    QDir_directory_of_chosen_list = "ranked_champs.txt";
    break;

    case 2:
    QDir_directory_of_chosen_list = "troll_champs.txt";
    break;

    case 3:
    QDir_directory_of_chosen_list = "custom_set_1.txt";
    break;

    case 4:
    QDir_directory_of_chosen_list = "custom_set_2.txt";
    break;

    }

    QList<QString> champs_to_rand;

    QFile inputFile(QDir_directory_of_chosen_list);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          champs_to_rand.append(line);
       }
       inputFile.close();
    }

    return champs_to_rand;

}



bool does_champion_exist(QString Dr_Soskam) //Dr_Soskam => upośledzony gracz ligi legend (był bardzo pijany) ktory po zbanowaniu mi championa //             dostał wylewu i wodogłowia naraz, po czym zaczął mieć jakieś problemy z wyrażaniem //             samego siebie.
{
    QList<QString> set = chosen_set(0);
    for(int i=0;i<set.length();i++)
        qDebug()<<set[i];
    for(int i=0;i<set.length();i++)
    {
        if(set[i]==Dr_Soskam)
            return true;
    }
    return false;
}



//+=============================================TEST_START===================================================+
void MainWindow::on_pushButton_7_clicked()
{
    QDir eo("E:/C++/c--(ale cute)/LeaugePickerv2/champion");
    QList<QString> eooolista = eo.entryList(QStringList(),QDir::Files);
    QFile txt("E:/C++/c--(ale cute)/LeaugePickerv2/sets/all_champs.txt");
    int i=0;
    if(txt.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&txt);
        for(i=0;i<eooolista.length();i++)
        {
            stream<<eooolista[i]<<'\n';
        }

    }
}



void MainWindow::on_pushButton_9_clicked()
{
    QString line;
    QString path = QCoreApplication::applicationDirPath();
    path.append("/test.txt");
    qDebug()<<path;
    QFile txt(path);
    if(!txt.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&txt);
            while(!stream.atEnd())
            {
            stream<<line;
            qDebug()<<line;
            }
        }
}



void MainWindow::on_pushButton_11_clicked() // add clicked
{

}

//+=============================================TEST_END=====================================================+



//+===========================================SETS SETTINGS==================================================+

void MainWindow::on_pushButton_2_clicked() //ADD
{

    if(ui->comboBox_2->currentIndex()==0)
    {
        qDebug()<<"nie ma takiego numeru :) (tzn nie mozna edytowac allchamp)";
        ui->lineEdit->clear();
        return;
    }

    QString text_in_line = ui->lineEdit->text(),path;
    QList<QString> current_set_champs = chosen_set(ui->comboBox_2->currentIndex());
    bool hlpvar=FALSE;

    switch(ui->comboBox_2->currentIndex())
    {
    case 1:
        path = "ranked_champs.txt";
        break;
    case 2:
        path = "troll_champs.txt";
        break;
    case 3:
        path = "custom_set_1.txt";
        break;
    case 4:
        path = "custom_set_2.txt";
        break;
    }



    qDebug()<<text_in_line;
    if(does_champion_exist(text_in_line+".png"))
    {
        qDebug()<<"no jest taki champ wiec gituwa :)";
        QFile txt(path);
        if(txt.open(QIODevice::ReadOnly))
        {
            QTextStream in(&txt);
            while (!in.atEnd())
            {
               QString line = in.readLine();
               if(line==text_in_line+".png")
                   hlpvar=TRUE;
            }
            if(hlpvar)
            {
                qDebug()<<"otóż ten chapmion znajduje się już na tej liście";
                return;
            }
        }
        txt.close();

        if(txt.open(QIODevice::WriteOnly | QIODevice::Append))
        {

            QTextStream stream(&txt);
            stream<<text_in_line+".png"+"\n";
            current_set_champs.append(text_in_line+".png");
        }
        txt.close();
    }
    else
    {
        qDebug()<<"niestety nie jest gituwa, bo nie ma takiego champa :(";
        ui->lineEdit->clear();
        return;
    }

    ui->lineEdit->clear();


    //PART II gdzie sie lista pokazuje  wazen! zrobić tak żeby wpisywac bez png i to dodawalo w kodzie

    ui->listWidget->clear();
    for(int i=0;i<current_set_champs.length();i++)
    {
        current_set_champs[i].remove(current_set_champs[i].length()-4,4);
        ui->listWidget->addItem(current_set_champs[i]);
    }
}



void MainWindow::on_pushButton_12_clicked() //DELETE
{

    if(ui->comboBox_2->currentIndex()==0)
    {
        qDebug()<<"nie ma takiego numeru :) (tzn nie mozna edytowac allchamp)";
        ui->lineEdit->clear();
        return;
    }

    QString text_in_line = ui->lineEdit->text(),path;
    QList<QString> current_set_champs = chosen_set(ui->comboBox_2->currentIndex());
    bool hlpvar=FALSE;

    switch(ui->comboBox_2->currentIndex())
    {
    case 1:
        path = "ranked_champs.txt";
        break;
    case 2:
        path = "troll_champs.txt";
        break;
    case 3:
        path = "custom_set_1.txt";
        break;
    case 4:
        path = "custom_set_2.txt";
        break;
    }

    for(int i=0;i<current_set_champs.length();i++)
    {
        if(current_set_champs[i]==text_in_line)
            current_set_champs[i].clear();
    }

}



void MainWindow::on_comboBox_2_currentIndexChanged() //SHOW SETS
{
    QList<QString> current_set_champs = chosen_set(ui->comboBox_2->currentIndex());

    ui->listWidget->clear();
    for(int i=0;i<current_set_champs.length();i++)
    {
        current_set_champs[i].remove(current_set_champs[i].length()-4,4);
        ui->listWidget->addItem(current_set_champs[i]);
    }

}



//+=======================================WEB SERVICE(XDDDDDDDDDDDDDDDDDDDDDDDDDDDDD)========================================+





//"D:\Riot Games\League of Legends\lockfile"

std::string exec(const char* cmd)
{

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


/*
      QString line;
    QFile lockfile("D:/Riot Games/League of Legends/lockfile");
    if(lockfile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
         QTextStream stream(&lockfile);
         while(!stream.atEnd())
         {

         stream<<line;
         qDebug()<<line;
         }
   }

    qDebug()<<lockfile.errorString();
    //std::string xd = "ipconfig";

    //qDebug()<<QString::fromStdString(exec(xd));


 */

void MainWindow::on_pushButton_14_clicked()
{
  QString line;
  QFile lockfile("D:/Riot Games/League of Legends/lockfile"); //D:/Riot Games/League of Legends/lockfile
  lockfile.rename("lockfile.txt");
  if(lockfile.open(QIODevice::ReadOnly|QIODevice::Text))
  {
       QTextStream stream(&lockfile);
       while(!stream.atEnd())
       {
       stream<<line;
       qDebug()<<line;
       }
 }
  qDebug()<<lockfile.errorString();
}



void MainWindow::on_pushButton_13_clicked()
{

    QProcess process;
    QStringList arguments = { "PROCESS", "WHERE", "name='LeagueClientUx.exe'", "GET", "commandline"};
    process.start("wmic",arguments);
    process.waitForFinished(-1); // will wait forever until finished

    QString output(process.readAllStandardOutput());

    QRegularExpression regex("--app-port=([0-9]+)");
    qDebug() << regex.match(output).captured(1);

    QString port = regex.match(output).captured(1);

    regex = QRegularExpression("--remoting-auth-token=(.*?)\"");
    qDebug() << regex.match(output).captured(1);

    QString password = regex.match(output).captured(1);



    //teraz wlasciwe bawienie sie liga legend;

    //https://api.ipify.org/

    QString http = "https://127.0.0.1:" + port + "/lol-perks/v1/currentpage";

    QNetworkRequest request_xd = QNetworkRequest(QUrl(http));
    QString auth = "riot:"+password;
    QByteArray jaktamchce = auth.toLocal8Bit().toBase64();
    QSslConfiguration conf = request_xd.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request_xd.setSslConfiguration(conf);

    request_xd.setRawHeader("Authorization",QString("Basic "+jaktamchce).toLocal8Bit());
    mManager->get(request_xd);


}

// nie usuwac
int jebac_przemka;
//xdxd


