#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QSerialPort>

#include "ImageDispose/robot_imagedispose.h"
#include "Strategists/robot_strategist.h"
#include "ImageDispose/hawkseye.h"
#include "ImageDispose/hyperpoint.h"
#include "imgdisstuff.h"
#include "strategistsstuff.h"
#include "saveopen_dialog.h"            //Add @ 2012-07-15

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

//==============public properties==============
private:
    QPixmap green_label;
    QPixmap red_label;
    VideoCapture cap;
    VideoCapture cap2;
    bool cap_open,cap2_open;
    QTimer* check_cap_timer;

    QMessageBox warning_box;

private slots:
    void capStatusCheck();

//================camera display===============
private:
    Mat display_frame;
    Mat display_frame2;
    QTimer* display_timer;
    bool display_pause;
    int save_count;
    bool capture;
private slots:
    void camDisplay();
    void on_displayPause_clicked();
    void on_displayContinue_clicked();

//=========image dispose configuration=========
private:
    int bin;
    float range[2];
    Size size;
    Mat obj_sample;

//============ dispose programs ===============
private:
    Mat input_image;

    vector<DisResStatus> dr_status;
     vector<DisResStatus> dr_status2;
    DisFactory dis_factory;
    DisResFactory dr_factory;
    vector<Robot_ImageDispose*> disposes,disposes2;
    vector<ImageDisposeResult*> dispose_results,dispose_results2,dispose_results3;

    void imageDisInitAndTest(int _seq);
    void on_obj4_usecheck_clicked(bool checked);


private slots:
    void on_obj1_usecheck_clicked(bool checked);
    void on_obj2_usecheck_clicked(bool checked);
    void on_obj3_usecheck_clicked(bool checked);
    void on_obj1_test_clicked();
    void on_obj1_lock_clicked();
    void on_obj1_stop_clicked();
    void on_obj2_test_clicked();
    void on_obj2_lock_clicked();
    void on_obj2_stop_clicked();
    void on_obj3_test_clicked();
    void on_obj3_lock_clicked();
    void on_obj3_stop_clicked();
    void on_obj4_test_clicked();
    void on_obj4_lock_clicked();
    void on_obj4_stop_clicked();

//===============strategists===================
private:
    bool is_exec_strategy;

    Robot_Strategist* strategist;
    StrategistType strategy_type;
    StrategistFactory strategist_fact;
    vector<robot_action> actions;


private slots:
    void on_strategy_start_clicked();
    void on_strategy_stop_clicked();
    void on_strategy_clearinfo_clicked();

//===============execution part================
private:
    QSerialPort *port;
    bool port_open;
    //add 2012-7-21 ***************************
    QTimer *action_timeout;
    //add 2012-7-21 ***************************
    QByteArray order;
    bool       is_action;
    void sendOrder(robot_action _action);
    QString show_saveOpenDialog(QString _original_file_name = "");    //Add @ 2012-07-15

private slots:
    void excution_finish();

    void on_portOpen_clicked();
    void on_portClose_clicked();

    void on_send_turnleft_clicked() {sendOrder(TURN_LEFT);}
    void on_send_mvleft_clicked(){sendOrder(MOVE_LEFT);}
    void on_send_kickleft_clicked(){sendOrder(KICK_LEFT);}
    void on_send_mvforward1_clicked(){sendOrder(MOVE_FORWARD);}
    void on_send_mvforward2_clicked(){sendOrder(MOVE_FORWARD_TINY);}
    void on_send_turnright_clicked(){sendOrder(TURN_RIGHT);}
    void on_send_mvright_clicked(){sendOrder(MOVE_RIGHT);}
    void on_send_kickright_clicked(){sendOrder(KICK_RIGHT);}
    void on_send_mvback1_clicked(){sendOrder(MOVE_BACK);}
    void on_send_mvback2_clicked(){sendOrder(MOVE_BACK2);}
    void on_send_wan_clicked(){sendOrder(BOW);}
    void on_send_mvforward3_clicked(){sendOrder(MOVE_FORWARD_LITTLE);}
    void on_send_standpose_clicked(){sendOrder(STAND_POSE);}
    void on_send_turnleft_big_clicked(){sendOrder(TURN_LEFT_big);}
    void on_send_turnright_big_clicked(){sendOrder(TURN_RIGHT_big);}
    void on_head_up_clicked(){sendOrder((HEAD_UP));}
        void on_head_down_clicked(){sendOrder((HEAD_DOWN));}
            void on_head_left_clicked(){sendOrder((HEAD_LEFT));}
                void on_head_right_clicked(){sendOrder((HEAD_RIGHT));}


    void on_Button_obj1_OpenDialog_clicked();           //Add @ 2012-07-15
    void on_Button_obj2_OpenDialog_clicked();
    void on_Button_obj3_OpenDialog_clicked();
    void on_Button_obj4_OpenDialog_clicked();
    void on_pushButton_CaptureSample_clicked();
    void on_pushButton_obj1_CaptureSample_clicked();
    void on_pushButton_obj2_CaptureSample_clicked();
    void on_pushButton_obj3_CaptureSample_clicked();
    void on_pushButton_obj4_CaptureSample_clicked();


//    void on_pushButton_clicked();
//    void on_Button_obj2_OpenDialog_2_clicked();
    void on_obj_door_clicked();
};

#endif // MAINWINDOW_H
