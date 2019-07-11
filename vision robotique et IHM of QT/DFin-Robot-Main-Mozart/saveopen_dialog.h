#ifndef SAVEOPEN_DIALOG_H
#define SAVEOPEN_DIALOG_H

#include <QDialog>
#include <QString>
#include <QMouseEvent>
#include <QPainter>
//#define _Debug_SaveOpen_Dialog

namespace Ui {
    class SaveOpen_Dialog;
}

class SaveOpen_Dialog : public QDialog
{
    Q_OBJECT
//---------Interface----------
public:
    explicit SaveOpen_Dialog(QWidget *parent = 0, uchar * _image_data=0, int _rows=0, int _columns=0);
    ~SaveOpen_Dialog();
    QString OpenImage();
    QString GetSavedImagePath();

//---------UI Control---------
private:
    Ui::SaveOpen_Dialog *ui;
    QImage              *qImg;
    QImage               qImgSub;
    QImage               qImgSub_Scaled;
    uchar               *subImage;
    int                  x1,x2,y1,y2;
    bool                 is_capturing;
    bool                 has_got_valid_capture;
    QString              save_image_path;
    int SaveToFile(QImage &_qImg);
    void paintEvent(QPaintEvent* e);
    void GetSubImage(int x1, int y1, int x2, int y2);

//---------Image-------------
private:
    uchar               *image_data;
    int                  rows;
    int                  columns;
    bool    PrepareImageData(uchar * _image_data, int _rows, int _columns);

//---------Event--------------
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_pushButton_Up_clicked();
    void on_pushButton_Down_clicked();
    void on_pushButton_Left_clicked();
    void on_pushButton_Right_clicked();
    void on_buttonBox_accepted();
    void on_pushButton_WidthMinus_clicked();
    void on_pushButton_WidthPlus_clicked();
    void on_pushButton_HeightPlus_clicked();
    void on_pushButton_HeightMinus_clicked();
};

#endif // SAVEOPEN_DIALOG_H
