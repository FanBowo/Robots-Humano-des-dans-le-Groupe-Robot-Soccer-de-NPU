
#include "saveopen_dialog.h"
#include "ui_saveopen_dialog.h"
#include <QFileDialog>
#include <QDebug>

//---------Interface----------
SaveOpen_Dialog::SaveOpen_Dialog(QWidget *parent, uchar * _image_data, int _rows, int _columns) :
    QDialog(parent),
    ui(new Ui::SaveOpen_Dialog)
{
    ui->setupUi(this);
    qImg = 0;
    subImage= 0;
    x1 = x2 = y1 = y2 = 0;
    is_capturing            = false;
    has_got_valid_capture   = false;
    save_image_path         = "";
    ui->label_SubImage->setMinimumWidth(235);
    ui->label_SubImage->setMaximumWidth(235);
    ui->label_SubImage->setMaximumHeight(280);
    this->setWindowTitle("Get Sample");

    if(_image_data){
        PrepareImageData( _image_data, _rows, _columns);
    }
}

//---------Interface----------
SaveOpen_Dialog::~SaveOpen_Dialog()
{
    delete ui;
}

//---------Interface----------
QString SaveOpen_Dialog::OpenImage()
{
    QString fileName = QFileDialog::getOpenFileName (this, tr("Open Sample Image"),"../" ,
                                          tr("Images(*.png *.jpg *.jpeg *.bmp);;All (*.*)"));
    if (!fileName.isEmpty()){
        return fileName;
    }
    else{
        return tr("");
    }
}

//---------Interface----------
bool SaveOpen_Dialog::PrepareImageData(uchar * _image_data, int _rows, int _columns)
{
    has_got_valid_capture = false;
    save_image_path       = "";

    //image_data = _image_data;
    rows       = _rows;
    columns    = _columns;
    image_data = new uchar[rows*columns*3];

    //Typeical value: _rows= 480  _columns= 640
    if( qImg ) delete qImg;

    //BGR to RGB:
    uchar tmp;
    for(int x=0; x<_columns; x++){
        for(int y=0; y<_rows; y++){
            //tmp = _image_data[(x+y*_columns)*3+0];
            //_image_data[(x+y*_columns)*3+0] = _image_data[(x+y*_columns)*3+2];
            //_image_data[(x+y*_columns)*3+2] = tmp;
            image_data[(x+y*columns)*3+0] = _image_data[(x+y*columns)*3+2];
            image_data[(x+y*columns)*3+1] = _image_data[(x+y*columns)*3+1];
            image_data[(x+y*columns)*3+2] = _image_data[(x+y*columns)*3+0];
        }
    }

    qImg = new QImage(image_data,columns,rows,QImage::Format_RGB888);
    ui->label_Image->setPixmap(QPixmap::fromImage( *qImg,Qt::AutoColor));
    //this->show();
}

//---------Interface----------
QString SaveOpen_Dialog::GetSavedImagePath()
{
    return save_image_path;
}

int SaveOpen_Dialog::SaveToFile(QImage &_qImg)
{
    if(x2<=x1 || y2<=y1) return -1;
    save_image_path = QFileDialog::getSaveFileName(this, tr("Save Sample Image"),
                               "../",
                               tr("Image (*.jpg *.png *.jpeg *.bmp)"));
    _qImg.save(save_image_path);
    return 0;
}

void SaveOpen_Dialog::paintEvent(QPaintEvent* e)
{
    /*QPainter painter(ui->label_Image);
    QColor color;
    color.setHsv(0, 255, 0);
    painter.setPen(color);

    //const QImage img(qImg);
    //painter.drawImage(QPoint(0,0),qImg);
    //qImg->loadFromData( (uchar *)image_data, columns * rows * 3 * sizeof(char) );
    //ui->label_Image->setPixmap(QPixmap::fromImage( *qImg,Qt::AutoColor));

    painter.drawRect(x1+640,y1+480,(x2-x1),(y2-y1));*/
}

void SaveOpen_Dialog::on_buttonBox_accepted()
{
    if(has_got_valid_capture){
        SaveToFile(qImgSub);
    }
    accept();
}

void SaveOpen_Dialog::mousePressEvent(QMouseEvent *event)
{
    is_capturing = true;
    QPoint pos = event->pos() - ui->label_Image->pos();
    x1 = pos.x();
    y1 = pos.y();
    if( x1 > ui->label_Image->width() || y1 > ui->label_Image->height() ) return;

    #ifdef _Debug_SaveOpen_Dialog
    qDebug()<<"MouseEvent: x="<< pos.x() <<" y="<< pos.y();
    #endif
}
void SaveOpen_Dialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos() - ui->label_Image->pos();
    x2 = pos.x();
    y2 = pos.y();
    if( x1 > ui->label_Image->width() || y1 > ui->label_Image->height() ) return;
    #ifdef _Debug_SaveOpen_Dialog
    qDebug()<<"Mouse Move Event: x="<< pos.x() <<" y="<< pos.y();
    #endif
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    is_capturing = false;
    QPoint pos = event->pos() - ui->label_Image->pos();
    x2 = pos.x();
    y2 = pos.y();
    if( x1 > ui->label_Image->width() || y1 > ui->label_Image->height() ) return;

    #ifdef _Debug_SaveOpen_Dialog
    qDebug()<<"Mouse Release Event: x="<< pos.x() <<" y="<< pos.y();
    #endif

    //SaveToFile(qImgSub);
}

void SaveOpen_Dialog::GetSubImage(int x1, int y1, int x2, int y2){
    if(x2<=x1 || y2<=y1){
        has_got_valid_capture = false;
        return;
    }
    has_got_valid_capture = true;
    //Get Sub Image:
    qImgSub = qImg->copy(x1,y1,(x2-x1),(y2-y1));
    //Resize Sub Image for display:
    int newWidth = ui->label_SubImage->width();
    int newHeight = newWidth * (y2-y1)/(x2-x1);
    if(newHeight > ui->label_SubImage->maximumHeight() ){
        newHeight = ui->label_SubImage->maximumHeight();
    }
    qImgSub_Scaled = qImgSub.scaled(newWidth, newHeight);
    //Display resized one:
    ui->label_SubImage->setPixmap(QPixmap::fromImage( qImgSub_Scaled,Qt::AutoColor));
    this->update();
}


void SaveOpen_Dialog::on_pushButton_Up_clicked()
{
    if(y1>0){
        y1--;
        y2--;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_Down_clicked()
{
    if(y2<(columns-1)){
        y1++;
        y2++;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_Left_clicked()
{
    if(x1>0){
        x1--;
        x2--;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_Right_clicked()
{
    if(x2<(columns-1)){
        x1++;
        x2++;
    }
    GetSubImage(x1,y1,x2,y2);
}



void SaveOpen_Dialog::on_pushButton_WidthMinus_clicked()
{
    if( x2 > (x1+2) ){
        x1++;
        x2--;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_WidthPlus_clicked()
{
    if( x1>0 && x2<(columns-1) ){
        x1--;
        x2++;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_HeightMinus_clicked()
{
    if( y2 > (y1+2) ){
        y1++;
        y2--;
    }
    GetSubImage(x1,y1,x2,y2);
}

void SaveOpen_Dialog::on_pushButton_HeightPlus_clicked()
{
    if( y1>0 && y2<(rows-1) ){
        y1--;
        y2++;
    }
    GetSubImage(x1,y1,x2,y2);
}
