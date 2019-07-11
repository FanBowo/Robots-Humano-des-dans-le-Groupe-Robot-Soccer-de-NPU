#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->strategy_type->addItem(tr("zap"));
    ui->strategy_type->addItem(tr("15点球"));
    ui->strategy_type->addItem(tr("3v3守门"));
    ui->strategy_type->addItem(tr("3v3大前锋"));
    //=====================initialization=======================
    //green_label and red_label
    Mat ma_temp(30,16,CV_8UC3);
    ma_temp = Scalar(0,255,0);
    QImage qi_temp = QImage((const uchar*)ma_temp.data,
                            ma_temp.cols, ma_temp.rows,
                            QImage::Format_RGB888).rgbSwapped();
    green_label = QPixmap::fromImage(qi_temp);
    ma_temp = Scalar(0,0,255);
    capture = true;
    qi_temp = QImage((const uchar*)ma_temp.data,
                     ma_temp.cols, ma_temp.rows,
                     QImage::Format_RGB888).rgbSwapped();
    red_label = QPixmap::fromImage(qi_temp);

    //capture and capture checker
    cap.open(0);
    if (cap.isOpened())
    {
        ui->camStatus->setPixmap(green_label);
        cap_open = true;
    }
    else
    {
        cap_open = false;
        ui->camStatus->setPixmap(red_label);
    }
    save_count = 0;
    check_cap_timer = new QTimer();
    connect(check_cap_timer,SIGNAL(timeout()),this,SLOT(capStatusCheck()));
    check_cap_timer->start(500);

    //camera display
    display_pause = false;
    ui->displayContinue->setEnabled(false);
    display_timer = new QTimer();
    connect(display_timer,SIGNAL(timeout()),this,SLOT(camDisplay()));
    display_timer->start(40);

    //=====================excution part=====================

    //port stuff
    port = new QSerialPort();
    port_open = false;
    ui->portStatus->setPixmap(red_label);
    ui->portClose->setEnabled(false);

    is_action = false;
    //add 2012-7-21 ***************************
    action_timeout = new QTimer;
    action_timeout->setSingleShot(true);
    connect(action_timeout,SIGNAL(timeout()),this,SLOT(excution_finish()));
    //add 2012-7-21 ***************************

    connect(port,SIGNAL(readyRead()),this,SLOT(excution_finish()));

    //=============dispose programs and strategists===========

    dr_status.resize(DISPOSE_RESULT_COUNT);
    dr_status2.resize(DISPOSE_RESULT_COUNT);
    dispose_results.resize(DISPOSE_RESULT_COUNT);
    dispose_results2.resize(DISPOSE_RESULT_COUNT);
    disposes.resize(DISPOSE_RESULT_COUNT);
    disposes2.resize(DISPOSE_RESULT_COUNT);

    vector<DisResStatus>::iterator d_iter;
    for ( d_iter=dr_status.begin(); d_iter!=dr_status.end();
          d_iter++)
        (*d_iter).start_using = false;
    ui->obj1_usebox->setEnabled(false);
    ui->obj2_usebox->setEnabled(false);
    ui->obj3_usebox->setEnabled(false);
    ui->obj4_usebox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//===============public properties=====================
void MainWindow::capStatusCheck()
{
    if (cap.isOpened())
    {
        if (!cap_open)
        {
            cap_open = true;
            ui->camStatus->setPixmap(green_label);
        }
    }
    else
    {
        if (cap_open)
        {
            cap_open = false;
            ui->camStatus->setPixmap(red_label);
        }
    }
}

//===============camera display========================
void MainWindow::camDisplay()
{
    if (!display_pause && cap_open)
    {
        cap >> display_frame;
        flip(display_frame,display_frame,0);
        flip(display_frame,display_frame,1);

        //        cap2 >>display_frame2;
        int a,b,c,d,e,f,left,Cleft,right,Cright,buffer1,buffer2;
        int moveX,moveY;

        moveX=10;
        moveY=-5;
        //x:
        a=160+moveX;
        b=260+moveX;
        c=360+moveX;
        d=460+moveX;
        left=200+moveX;
        right=420+moveX;
        Cleft=300+moveX-8;
        Cright=320+moveX+8;
        //y:
        buffer1=410+moveY;
        buffer2=440+moveY;
        e=270;
        f=380;
        //distance:
        left=left-20;
        right=right+40;
        Cleft=Cleft+0;
        Cright=Cright+10;

        line( display_frame, Point(a,0), Point(a,170), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(b,0), Point(b,480), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(c,0), Point(c,480), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(d,0), Point(d,170), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(0,e), Point(640,e), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(b,f), Point(c,f), Scalar(0,0,255), 1, 8 );
        line( display_frame, Point(left,e), Point(left,480), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(right,e), Point(right,480), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(Cleft,e), Point(Cleft,480), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(Cright,e), Point(Cright,480), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(left,buffer1), Point(Cleft,buffer1), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(Cright,buffer1), Point(right,buffer1), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(left,buffer2), Point(Cleft,buffer2), Scalar(0,255,0), 1, 8 );
        line( display_frame, Point(Cright,buffer2), Point(right,buffer2), Scalar(0,255,0), 1, 8 );

        rectangle( display_frame, Point(b,e), Point(c,480), Scalar(250,0,0), 1, 8 );

        imshow("cap",display_frame);
    }
}
//暂停相机显示
void MainWindow::on_displayPause_clicked()
{
    ui->displayContinue->setEnabled(true);
    ui->displayPause->setEnabled(false);
    display_pause = true;
}
//继续显示
void MainWindow::on_displayContinue_clicked()
{
    ui->displayPause->setEnabled(true);
    ui->displayContinue->setEnabled(false);
    display_pause = false;
}

//==================port control===================
//打开串口
void MainWindow::on_portOpen_clicked()
{
    QString _name = ui->portName->text();
    QString _baud = ui->portBaudRate->text();
    port->setPortName(_name);
    if ( port->open(QSerialPort::ReadWrite) )
    {
        port->setBaudRate(_baud.toInt());
        port_open = true;
        ui->portStatus->setPixmap(green_label);
        ui->portOpen->setEnabled(false);
        ui->portClose->setEnabled(true);
        sendOrder(STAND_POSE);
        waitKey(200);
        sendOrder(STAND_POSE);
        waitKey(200);
        sendOrder(STAND_POSE);
        waitKey(200);
        sendOrder(STAND_POSE);
        waitKey(200);
        sendOrder(STAND_POSE);
        waitKey(200);
    }
}
//关闭串口
void MainWindow::on_portClose_clicked()
{
    port_open = false;
    ui->portStatus->setPixmap(red_label);
    ui->portOpen->setEnabled(true);
    ui->portClose->setEnabled(false);
    port->close();

    //关闭串口后,对手动执行板块进行可用赋值
    ui->exec_hand->setEnabled(true);
}

//=================image dispose==================

//启用和关闭四个处理对象
void MainWindow::on_obj1_usecheck_clicked(bool checked)
{
    if (checked)
    {
        (dr_status[0]).start_using = true;
        (dr_status2[0]).start_using = true;
        ui->obj1_usebox->setEnabled(true);
        ui->obj1_lock->setEnabled(false);
    }
    else
    {
        (dr_status[0]).start_using = false;
        (dr_status2[0]).start_using = false;
        ui->obj1_usebox->setEnabled(false);
        if (dispose_results[0] != NULL)
        {
            delete dispose_results[0];
            dispose_results[0] = NULL;
        }
        if (dispose_results2[0] != NULL)
        {
            delete dispose_results2[0];
            dispose_results2[0] = NULL;
        }
        if (disposes[0] != NULL)
        {
            delete disposes[0];
            disposes[0] = NULL;
        }
        if (disposes2[0] != NULL)
        {
            delete disposes2[0];
            disposes2[0] = NULL;
        }
    }
}
void MainWindow::on_obj2_usecheck_clicked(bool checked)
{
    if (checked)
    {
        (dr_status[1]).start_using = true;

        (dr_status2[1]).start_using = true;
        ui->obj2_usebox->setEnabled(true);
        ui->obj2_lock->setEnabled(false);
    }
    else
    {
        (dr_status[1]).start_using = false;
        (dr_status2[1]).start_using = false;
        ui->obj2_usebox->setEnabled(false);
        if (dispose_results[1] != NULL)
        {
            delete dispose_results[1];
            dispose_results[1] = NULL;
        }
        if (dispose_results2[1] != NULL)
        {
            delete dispose_results2[1];
            dispose_results2[1] = NULL;
        }
        if (disposes[1] != NULL)
        {
            delete disposes[1];
            disposes[1] = NULL;
        }
        if (disposes2[1] != NULL)
        {
            delete disposes2[1];
            disposes2[1] = NULL;
        }
    }
}
void MainWindow::on_obj3_usecheck_clicked(bool checked)
{
    if (checked)
    {
        (dr_status[2]).start_using = true;
        (dr_status2[2]).start_using = true;
        ui->obj3_usebox->setEnabled(true);
        ui->obj3_lock->setEnabled(false);
    }
    else
    {
        (dr_status[2]).start_using = false;
        (dr_status2[2]).start_using = false;
        ui->obj3_usebox->setEnabled(false);
        if (dispose_results[2] != NULL)
        {
            delete dispose_results[2];
            dispose_results[2] = NULL;
        }
        if (dispose_results2[2] != NULL)
        {
            delete dispose_results2[2];
            dispose_results2[2] = NULL;
        }
        if (disposes[2] != NULL)
        {
            delete disposes[2];
            disposes[2] = NULL;
        }
        if (disposes2[2] != NULL)
        {
            delete disposes2[2];
            disposes2[2] = NULL;
        }
    }
}
void MainWindow::on_obj4_usecheck_clicked(bool checked)
{
    if (checked)
    {
        (dr_status[3]).start_using = true;
        (dr_status2[3]).start_using = true;
        ui->obj4_usebox->setEnabled(true);
        ui->obj4_lock->setEnabled(false);
    }
    else
    {
        (dr_status[3]).start_using = false;
        (dr_status2[3]).start_using = false;
        ui->obj4_usebox->setEnabled(false);
        if (dispose_results[3] != NULL)
        {
            delete dispose_results[3];
            dispose_results[3] = NULL;
        }
        if (disposes[3] != NULL)
        {
            delete disposes[3];
            disposes[3] = NULL;
        }
        if (disposes2[3] != NULL)
        {
            delete disposes2[3];
            disposes2[3] = NULL;
        }
    }
}

//执行Test
void MainWindow::imageDisInitAndTest(int _seq)
{

    //读入dispose模块配置信息
    switch(_seq)
    {
    case 0:
        dr_status[_seq].obj_path = ui->obj1_objpath->text().toStdString();
        dr_status[_seq].res_mod_switcher = ui->obj1_resswitch->currentIndex();
        switch(ui->obj1_distype->currentIndex())
        {
        case 0: dr_status[_seq].type = BALL; break;
        case 1: dr_status[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status[_seq].show = ui->obj1_showres->isChecked();
        break;
    case 1:
        dr_status[_seq].obj_path = ui->obj2_objpath->text().toStdString();
        dr_status[_seq].res_mod_switcher = ui->obj2_resswitch->currentIndex();
        switch(ui->obj2_distype->currentIndex())
        {
        case 0: dr_status[_seq].type = BALL; break;
        case 1: dr_status[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status[_seq].show = ui->obj2_showres->isChecked();
        break;
    case 2:
        dr_status[_seq].obj_path = ui->obj3_objpath->text().toStdString();
        dr_status[_seq].res_mod_switcher = ui->obj3_resswitch->currentIndex();
        switch(ui->obj3_distype->currentIndex())
        {
        case 0: dr_status[_seq].type = BALL; break;
        case 1: dr_status[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status[_seq].show = ui->obj3_showres->isChecked();
        break;
    case 3:
        dr_status[_seq].obj_path = ui->obj4_objpath->text().toStdString();
        dr_status[_seq].res_mod_switcher = ui->obj4_resswitch->currentIndex();
        switch(ui->obj4_distype->currentIndex())
        {
        case 0: dr_status[_seq].type = BALL; break;
        case 1: dr_status[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status[_seq].show = ui->obj4_showres->isChecked();
        break;
    default: break;
    }

    switch(_seq)
    {
    case 0:
        dr_status2[_seq].obj_path_2 = ui->obj1_objpath_2->text().toStdString();
        dr_status2[_seq].res_mod_switcher = ui->obj1_resswitch->currentIndex();
        switch(ui->obj1_distype->currentIndex())
        {
        case 0: dr_status2[_seq].type = BALL; break;
        case 1: dr_status2[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status2[_seq].show = ui->obj1_showres->isChecked();
        break;
    case 1:
        dr_status2[_seq].obj_path = ui->obj2_objpath->text().toStdString();
        dr_status2[_seq].res_mod_switcher = ui->obj2_resswitch->currentIndex();
        switch(ui->obj2_distype->currentIndex())
        {
        case 0: dr_status2[_seq].type = BALL; break;
        case 1: dr_status2[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status2[_seq].show = ui->obj2_showres->isChecked();
        break;
    case 2:
        dr_status2[_seq].obj_path = ui->obj3_objpath->text().toStdString();
        dr_status2[_seq].res_mod_switcher = ui->obj3_resswitch->currentIndex();
        switch(ui->obj3_distype->currentIndex())
        {
        case 0: dr_status2[_seq].type = BALL; break;
        case 1: dr_status2[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status2[_seq].show = ui->obj3_showres->isChecked();
        break;
    case 3:
        dr_status2[_seq].obj_path = ui->obj4_objpath->text().toStdString();
        dr_status2[_seq].res_mod_switcher = ui->obj4_resswitch->currentIndex();
        switch(ui->obj4_distype->currentIndex())
        {
        case 0: dr_status2[_seq].type = BALL; break;
        case 1: dr_status2[_seq].type = HAWK; break;
        default:    break;
        }
        dr_status2[_seq].show = ui->obj4_showres->isChecked();
        break;
    default: break;
    }


    //读取目标图片
    Mat _sample = imread(dr_status[_seq].obj_path);
    Mat _sample2 = imread(dr_status2[_seq].obj_path_2);
    if ((!_sample.data)||(!_sample2.data))
    {
        switch(_seq)
        {
        qDebug()<<"sample data failed" << endl;
        case 0: ui->obj1_objvalid->setPixmap(red_label);break;
        case 1: ui->obj2_objvalid->setPixmap(red_label);break;
        case 2: ui->obj3_objvalid->setPixmap(red_label);break;
        case 3: ui->obj4_objvalid->setPixmap(red_label);break;
        default: break;
        }
        if(!_sample.data) ui->strategy_imgdis->append("ball sample failed");
        if(!_sample2.data) ui->strategy_imgdis->append("door sample failed");
        ui->strategy_imgdis->append("over");
        return;
    }

    else switch(_seq)
    {
    qDebug()<<"sample data success";
    case 0: ui->obj1_objvalid->setPixmap(green_label);break;
    case 1: ui->obj2_objvalid->setPixmap(green_label);break;
    case 2: ui->obj3_objvalid->setPixmap(green_label);break;
    case 3: ui->obj4_objvalid->setPixmap(green_label);break;
    default: break;
    }


    //申请相应对象以及初始化dispose
    if (disposes[_seq]!=NULL)
        delete disposes[_seq];
    if (dispose_results[_seq]!=NULL)
        delete dispose_results[_seq];
    if (disposes2[_seq]!=NULL)
        delete disposes2[_seq];
    if (dispose_results2[_seq]!=NULL)
        delete dispose_results2[_seq];
    disposes[_seq] = dis_factory.newDis(dr_status[_seq].type);
    disposes2[_seq] = dis_factory.newDis(dr_status2[_seq].type);

    if(dr_status[_seq].type==1)
    {
        disposes[_seq]->setseq(0);
        disposes2[_seq]->setseq(1);
    }
    dispose_results[_seq] = dr_factory.newDisRes(dr_status[_seq].type);
    dispose_results2[_seq] = dr_factory.newDisRes(dr_status2[_seq].type);
    if (dispose_results[_seq]==NULL ||
            disposes[_seq]==NULL||dispose_results2[_seq]==NULL ||
            disposes2[_seq]==NULL)
    {
        warning_box.setText("allocate object dispose failed!");
        warning_box.exec();
        return;
    }
    qDebug()<<"begin set";
    disposes[_seq]->setDetectObj(_sample);
    disposes[_seq]->setShow(dr_status[_seq].show);
    disposes[_seq]->setResultSwitcher(dr_status[_seq].res_mod_switcher);


    disposes2[_seq]->setDetectObj(_sample2);
    disposes2[_seq]->setShow(dr_status2[_seq].show);
    disposes2[_seq]->setResultSwitcher(dr_status2[_seq].res_mod_switcher);

    qDebug()<<"set over";
    switch(_seq)
    {
    case 0: disposes[_seq]->setWindowName("display 1");
        disposes2[_seq]->setWindowName("display 1s");break;
    case 1: disposes[_seq]->setWindowName("display 2");
        disposes[_seq]->setWindowName("display 2s");break;
    case 2: disposes[_seq]->setWindowName("display 3");
        disposes[_seq]->setWindowName("display 3s");break;
    case 3: disposes[_seq]->setWindowName("display 4");
        disposes[_seq]->setWindowName("display 4s");break;
    default: break;
    }
    //测试,运行图像处理程序
    dr_status[_seq].test = true;
    dr_status2[_seq].test = true;
    qDebug()<<"before loop";




    //循环显示
    bool keepTest;
    Mat _frame;

    while (1)
    {
        keepTest = false;
        for (int i=0; i<DISPOSE_RESULT_COUNT; i++)
        {

            if ((dr_status[i].test ) &&  (dr_status2[i].test))
            {
                keepTest = true;
                if (cap_open){
                    cap >> _frame;
                    flip(_frame,_frame,0);
                    flip(_frame,_frame,1);

                }
                disposes[i]->getImage(_frame);
                disposes2[i]->getImage(_frame);

            }



        }
        if (!keepTest)
            break;
        waitKey(40);
    }

}


//测试,锁定以及停止
void MainWindow::on_obj1_test_clicked()
{
    ui->obj1_distype->setEnabled(false);
    ui->obj1_objpath->setEnabled(false);
    ui->obj1_objpath_2->setEnabled(false);
    ui->obj1_resswitch->setEnabled(false);
    ui->obj1_showres->setEnabled(false);

    ui->obj1_lock->setEnabled(false);
    ui->obj1_stop->setEnabled(true);
    ui->obj1_test->setEnabled(false);

    ui->obj1_usecheck->setEnabled(false);

    imageDisInitAndTest(0);
}

void MainWindow::on_obj1_lock_clicked()
{
    ui->obj1_distype->setEnabled(false);
    ui->obj1_objpath->setEnabled(false);

    ui->obj1_resswitch->setEnabled(false);
    ui->obj1_showres->setEnabled(false);

    ui->obj1_test->setEnabled(false);
    ui->obj1_stop->setEnabled(true);
    ui->obj1_lock->setEnabled(false);

    ui->obj1_usecheck->setEnabled(false);
}

void MainWindow::on_obj1_stop_clicked()
{
    dr_status[0].test = false;

    ui->obj1_distype->setEnabled(true);
    ui->obj1_objpath_2->setEnabled(true);
    ui->obj1_resswitch->setEnabled(true);
    ui->obj1_showres->setEnabled(true);

    ui->obj1_test->setEnabled(true);
    ui->obj1_lock->setEnabled(true);
    ui->obj1_stop->setEnabled(false);

    ui->obj1_usecheck->setEnabled(true);
}

void MainWindow::on_obj2_test_clicked()
{
    ui->obj2_distype->setEnabled(false);
    ui->obj2_objpath->setEnabled(false);
    ui->obj2_resswitch->setEnabled(false);
    ui->obj2_showres->setEnabled(false);

    ui->obj2_lock->setEnabled(false);
    ui->obj2_stop->setEnabled(true);
    ui->obj2_test->setEnabled(false);

    ui->obj2_usecheck->setEnabled(false);

    imageDisInitAndTest(1);
}
void MainWindow::on_obj2_lock_clicked()
{
    ui->obj2_distype->setEnabled(false);
    ui->obj2_objpath->setEnabled(false);
    ui->obj2_resswitch->setEnabled(false);
    ui->obj2_showres->setEnabled(false);

    ui->obj2_test->setEnabled(false);
    ui->obj2_stop->setEnabled(true);
    ui->obj2_lock->setEnabled(false);

    ui->obj2_usecheck->setEnabled(false);
}
void MainWindow::on_obj2_stop_clicked()
{
    dr_status[1].test = false;

    ui->obj2_distype->setEnabled(true);
    ui->obj2_objpath->setEnabled(true);
    ui->obj2_resswitch->setEnabled(true);
    ui->obj2_showres->setEnabled(true);

    ui->obj2_test->setEnabled(true);
    ui->obj2_lock->setEnabled(true);
    ui->obj2_stop->setEnabled(false);

    ui->obj2_usecheck->setEnabled(true);
}

void MainWindow::on_obj3_test_clicked()
{
    ui->obj3_distype->setEnabled(false);
    ui->obj3_objpath->setEnabled(false);
    ui->obj3_resswitch->setEnabled(false);
    ui->obj3_showres->setEnabled(false);

    ui->obj3_lock->setEnabled(false);
    ui->obj3_stop->setEnabled(true);
    ui->obj3_test->setEnabled(false);

    ui->obj3_usecheck->setEnabled(false);

    imageDisInitAndTest(2);
}
void MainWindow::on_obj3_lock_clicked()
{
    ui->obj3_distype->setEnabled(false);
    ui->obj3_objpath->setEnabled(false);
    ui->obj3_resswitch->setEnabled(false);
    ui->obj3_showres->setEnabled(false);

    ui->obj3_test->setEnabled(false);
    ui->obj3_stop->setEnabled(true);
    ui->obj3_lock->setEnabled(false);

    ui->obj3_usecheck->setEnabled(false);
}
void MainWindow::on_obj3_stop_clicked()
{
    dr_status[2].test = false;

    ui->obj3_distype->setEnabled(true);
    ui->obj3_objpath->setEnabled(true);
    ui->obj3_resswitch->setEnabled(true);
    ui->obj3_showres->setEnabled(true);

    ui->obj3_test->setEnabled(true);
    ui->obj3_lock->setEnabled(true);
    ui->obj3_stop->setEnabled(false);

    ui->obj3_usecheck->setEnabled(true);
}

void MainWindow::on_obj4_test_clicked()
{
    ui->obj4_distype->setEnabled(false);
    ui->obj4_objpath->setEnabled(false);
    ui->obj4_resswitch->setEnabled(false);
    ui->obj4_showres->setEnabled(false);

    ui->obj4_lock->setEnabled(false);
    ui->obj4_stop->setEnabled(true);
    ui->obj4_test->setEnabled(false);

    ui->obj4_usecheck->setEnabled(false);

    imageDisInitAndTest(3);
}
void MainWindow::on_obj4_lock_clicked()
{
    ui->obj4_distype->setEnabled(false);
    ui->obj4_objpath->setEnabled(false);
    ui->obj4_resswitch->setEnabled(false);
    ui->obj4_showres->setEnabled(false);

    ui->obj4_test->setEnabled(false);
    ui->obj4_stop->setEnabled(true);
    ui->obj4_lock->setEnabled(false);

    ui->obj4_usecheck->setEnabled(false);
}
void MainWindow::on_obj4_stop_clicked()
{
    dr_status[3].test = false;

    ui->obj4_distype->setEnabled(true);
    ui->obj4_objpath->setEnabled(true);
    ui->obj4_resswitch->setEnabled(true);
    ui->obj4_showres->setEnabled(true);

    ui->obj4_test->setEnabled(true);
    ui->obj4_lock->setEnabled(true);
    ui->obj4_stop->setEnabled(false);

    ui->obj4_usecheck->setEnabled(true);
}

//=====================excution part====================

void MainWindow::sendOrder(robot_action _action)
{
    if (!port_open)
    {
        warning_box.setText("port not open, can't send action");
        warning_box.exec();
        return;
    }
    port->clear();
    is_action = true;
    //add 2012-7-21 ***************************
    action_timeout->start(2000);
    //add 2012-7-21 ***************************

    ui->send_isaction->setPixmap(green_label);
    ui->exec_hand->setEnabled(false);

    QByteArray act = convertOrder(_action);
    port->write(act);
    ui->strategy_imgdis->append(actionToString(_action));
}

void MainWindow::excution_finish()
{
    //add 2012-7-21 ***************************
    action_timeout->stop();
    //add 2012-7-21 ***************************

    is_action = false;
    ui->send_isaction->setPixmap(red_label);

    if (!is_exec_strategy)
        ui->exec_hand->setEnabled(true);
    //cout<<"zhongduan"<<'\n';
}

//======================strategist======================

void MainWindow::on_strategy_start_clicked()
{
    //prepare...
    is_exec_strategy = true;

    //****ASSIGN STRATEGIST TYPE***
    qDebug() << ui->strategy_type->currentIndex();
    strategy_type = assignStrategistType(ui->strategy_type->currentIndex());

    strategist = strategist_fact.newStrategist(strategy_type);

    if (strategist == NULL)
    {
        warning_box.setText("allocate new strategist failed!");
        warning_box.exec();
        return;
    }

    ui->exec_hand->setEnabled(false);
    ui->strategy_start->setEnabled(false);
    ui->strategy_stop->setEnabled(true);
    ui->strategy_type->setEnabled(false);

    //*****************************MAIN LOOP**********************
    //************************************************************
    Mat _frame;
    while (is_exec_strategy && cap_open)
    {
        //taking image diposes
        cap >> _frame;
        flip(_frame,_frame,0);
        flip(_frame,_frame,1);

        for (int i=0; i<DISPOSE_RESULT_COUNT; i++)
        {
            if (dr_status[i].start_using)
            {
                disposes[i]->getImage(_frame);
                disposes2[i]->getImage(_frame);
                disposes[i]->result(*(dispose_results[i]));
                disposes2[i]->result(*(dispose_results2[i]));
                ui->strategy_imgdis->append(QString::number(i) +
                                            ": " + dispose_results[i]->toString());
                ui->strategy_imgdis->append(QString::number(i) +
                                            ": " + dispose_results2[i]->toString());
                if(strategy_type == 4)
                {
                    dispose_results3.resize(1);
                    HyperPoint *dispose_res= new HyperPoint;
                    dispose_results3[0] = new HyperPoint;
                    dispose_res->hyperres(*dispose_results[0],*dispose_results2[0]);
                    dispose_results3[0] = dispose_res;
                    if (strategist->getDisposeResult(dispose_results3,ui->haveblock->isChecked(),ui->MoveR->isChecked())
                            == false)
                    {
                        warning_box.setText("strategist send error message,\n might vision module setting has some problem");
                        warning_box.exec();
                        on_strategy_stop_clicked();
                        return;
                    }
                }
                else if (strategist->getDisposeResult(dispose_results,ui->haveblock->isChecked(),ui->MoveR->isChecked())
                         == false)
                {
                    qDebug()<<"type != 4";
                    warning_box.setText("strategist send error message,\n might vision module setting has some problem");
                    warning_box.exec();
                    on_strategy_stop_clicked();

                    return;
                }
            }
        }// for DISPOSE_RESULT_COUNT

        actions = strategist->orders();
        //send order
        for (vector<robot_action>::iterator iter_action=actions.begin();
             iter_action!=actions.end(); iter_action++)
        {
            port->clear();
            ui->strategy_exec_orders->append(actionToString(*iter_action));
            if ((*iter_action)==END)
            {
                warning_box.setText("strategy is finished");
                warning_box.exec();
                on_strategy_stop_clicked();
                return;
            }
            else
            {
                //while (true)//bug
                //{
                if (!port_open)
                {
                    warning_box.setText("you must open the serial port first");
                    warning_box.exec();
                    on_strategy_stop_clicked();
                    return;
                }
                if (!is_exec_strategy)
                {
                    warning_box.setText("strategy isn't executing!");
                    warning_box.exec();
                    on_strategy_stop_clicked();
                    return;
                }
                while (is_action)
                {
                    waitKey(40);

                }

                sendOrder(*iter_action);
                cout<<actionToString(*iter_action).toStdString()<<endl;

                if(*iter_action == HEAD_LEFT || *iter_action == HEAD_RIGHT
                        || *iter_action == HEAD_UP || *iter_action == HEAD_DOWN)
                {
                    waitKey(3000);
                }
                else if(*iter_action == MOVE_FORWARD)
                {
                    waitKey(7000);
                }
                else if(*iter_action == MOVE_BACK || *iter_action == MOVE_FORWARD_LITTLE)
                {
                  waitKey(5000);
                }
                else if(*iter_action == MOVE_LEFT || *iter_action == MOVE_RIGHT
                        || *iter_action == MOVE_LEFT_LITTLE || *iter_action == MOVE_RIGHT_LITTLE
                        ||*iter_action == TURN_LEFT || *iter_action == TURN_RIGHT)
                {
                    waitKey(3500);
                }
                else if(*iter_action == KICK_LEFT || *iter_action == KICK_RIGHT)
                {
                    waitKey(3000);;
                }
                else if(*iter_action == STAND_POSE)
                {
                    waitKey(1000);
                }
                port->clear();//清空缓存区！
                waitKey(800);
            }
        }//for iter_action
        port->clear();//清空缓存区！
    }//while strategist is executing

    //*****************************MAIN LOOP**********************
    //************************************************************
    on_strategy_stop_clicked();
}
void MainWindow::on_strategy_stop_clicked()
{
    is_exec_strategy = false;
    if (strategist != NULL)
    {
        delete strategist;
        strategist = NULL;
    }

    ui->exec_hand->setEnabled(true);
    ui->strategy_start->setEnabled(true);
    ui->strategy_stop->setEnabled(false);
    ui->strategy_type->setEnabled(true);
}
void MainWindow::on_strategy_clearinfo_clicked()
{
    ui->strategy_exec_orders->clear();
    ui->strategy_imgdis->clear();
}

void MainWindow::on_Button_obj1_OpenDialog_clicked()
{
    SaveOpen_Dialog save_open_dialog(this);
    ui->obj1_objpath->setText( save_open_dialog.OpenImage() );
}

void MainWindow::on_obj_door_clicked()
{
    SaveOpen_Dialog save_open_dialog(this);
    ui->obj1_objpath_2->setText( save_open_dialog.OpenImage() );
}

void MainWindow::on_Button_obj2_OpenDialog_clicked()
{
    SaveOpen_Dialog save_open_dialog(this);
    ui->obj2_objpath->setText( save_open_dialog.OpenImage() );
}

void MainWindow::on_Button_obj3_OpenDialog_clicked()
{
    SaveOpen_Dialog save_open_dialog(this);
    ui->obj3_objpath->setText( save_open_dialog.OpenImage() );
}

void MainWindow::on_Button_obj4_OpenDialog_clicked()
{
    SaveOpen_Dialog save_open_dialog(this);
    ui->obj4_objpath->setText( save_open_dialog.OpenImage() );
}

QString MainWindow::show_saveOpenDialog(QString _original_file_name)
{
    Mat _frame;
    //   Mat _frame2;
    if ( cap_open )
    {
        cap >> _frame;
        flip(_frame,_frame,0);
        flip(_frame,_frame,1);

        emit on_displayPause_clicked();
        SaveOpen_Dialog save_open_dialog( this, _frame.data, _frame.rows, _frame.cols );

        if (save_open_dialog.exec() == QDialog::Accepted ) {
            return save_open_dialog.GetSavedImagePath();
        }
    }
    return _original_file_name;
}

void MainWindow::on_pushButton_CaptureSample_clicked()
{
    show_saveOpenDialog();
}

void MainWindow::on_pushButton_obj1_CaptureSample_clicked()
{
    ui->obj1_objpath->setText( show_saveOpenDialog( ui->obj1_objpath->text() ) );
    capture = false;
}

void MainWindow::on_pushButton_obj2_CaptureSample_clicked()
{
    ui->obj2_objpath->setText( show_saveOpenDialog( ui->obj2_objpath->text() ) );
}

void MainWindow::on_pushButton_obj3_CaptureSample_clicked()
{
    ui->obj3_objpath->setText( show_saveOpenDialog( ui->obj3_objpath->text() ) );
}

void MainWindow::on_pushButton_obj4_CaptureSample_clicked()
{
    ui->obj4_objpath->setText( show_saveOpenDialog( ui->obj4_objpath->text() ) );
}

