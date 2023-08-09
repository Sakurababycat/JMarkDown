#include "about_dialog.h"
#include "ui_about_dialog.h"
#include "QWidget"
#include "QPainter"

about_dialog::about_dialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::about_dialog )
{
    ui->setupUi( this );
    this->setFixedSize( this->size() );
}


about_dialog::~about_dialog()
{
    delete ui;
}

