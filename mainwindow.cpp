#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QScrollBar"
#include "about_dialog.h"
#include "QFileDialog"
#include "QException"
#include "QCloseEvent"
#include "QMessageBox"
#include "QAbstractButton"
#include "utils.h"
#include "QShortcut"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow ), filename(), saved( true )
{
    ui->setupUi( this );
    setCentralWidget( ui->centralwidget );


    connect( ui->inputArea->verticalScrollBar(),
         &QScrollBar::valueChanged, this, [this]() {
        this->sync2zone();
    } );
    connect( ui->inputArea, &QTextEdit::textChanged, this,
         [this]() {
        saved = false;
        this->textEdit2display();
    } );
    connect( new QShortcut( QKeySequence( Qt::CTRL | Qt::Key_S ), this ),
         &QShortcut::activated, this, [this]() {
        this->save();
    } );
    connect( new QShortcut( QKeySequence( Qt::CTRL | Qt::Key_N ), this ),
         &QShortcut::activated, this, [this]() {
        this->on_new_action_triggered();
    } );
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_exit_action_triggered()
{
    if ( !checkFileSave() )
        return;
    this->~MainWindow();
    exit( 0 );
}


void MainWindow::closeEvent( QCloseEvent *event )
{
    if ( !checkFileSave() )
        event->ignore();
    else event->accept();
}


void MainWindow::on_about_action_triggered()
{
    about_dialog ab( this );
    this->setEnabled( false );
    ab.raise();
    connect( &ab, &QDialog::finished, this, [this]() {
        this->setEnabled( true );
    } );
    ab.show();
    ab.exec();
}


void MainWindow::on_open_action_triggered()
{
    try {
        if ( !checkFileSave() )
            return;
        auto *inputArea = ui->inputArea;

        QFileDialog fileDialog;
        fileDialog.setWindowTitle( "打开文件" );
        fileDialog.setNameFilter( "MarkDown文件 (*.md);;文本文件 (*.txt)" );
        fileDialog.setFileMode( QFileDialog::ExistingFile );

        if ( fileDialog.exec() )
        {
            filename = fileDialog.selectedFiles().at( 0 );
            QFile file( filename );
            if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) )
            {
                QTextStream	in( &file );
                QString		fileContent = in.readAll();
                file.close();

                inputArea->clear();
                inputArea->setPlainText( fileContent );
                saved = true;
            }else {
                qDebug() << "无法打开文件：" << file.errorString();
            }
        }
    } catch ( const QException &e ) {
        qDebug() << e.what() << '\n';
    }
}


void MainWindow::on_save_action_triggered()
{
    save();
}


bool MainWindow::selectNewFile2Save()
{
    /* 弹文件选框,设置默认文件名 */
    QFileDialog fileDialog;
    fileDialog.setWindowTitle( "保存文件" );
    fileDialog.setNameFilter( "MarkDown文件 (*.md);;文本文件 (*.txt)" );
    fileDialog.setAcceptMode( QFileDialog::AcceptSave );
    if ( !fileDialog.exec() )
        return(false);
    filename = fileDialog.selectedFiles().at( 0 );
    return(true);
}


void MainWindow::write2file()
{
    try {
        auto	*inputArea = ui->inputArea;
        QFile	file( filename );
        if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            auto		content = inputArea->toPlainText();
            QTextStream	fos( &file );
            fos << content;
            file.close();
            saved = true;
        }else {
            qDebug() << "无法保存到文件：" << filename << '\t' << file.errorString();
        }
    } catch ( const QException &e ) {
        qDebug() << e.what() << '\n';
    }
}


bool MainWindow::save()
{
    if ( filename.isEmpty() )
    {
        if ( !selectNewFile2Save() )
            return(false);
    }
    write2file();
    return(true);
}


/*返回true表示保存检查完成，进行下一步；false代表取消当前操作。 */
bool MainWindow::checkFileSave()
{
    if ( !saved )
    {
        QMessageBox qbox( this );
        qbox.setWindowTitle( QString( "未保存的文件" ) );
        qbox.setText( QString( "当前工作区有未保存的修改，是否保存到文件%1？" ).arg( filename ) );
        qbox.setStandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
        qbox.button( QMessageBox::Yes )->setText( "是" );
        qbox.button( QMessageBox::No )->setText( "否" );
        qbox.button( QMessageBox::Cancel )->setText( "取消" );

        switch ( qbox.exec() )
        {
        case QMessageBox::Yes:
            return(save() );
        case QMessageBox::No:
            return(true);
        case QMessageBox::Cancel:
        case QMessageBox::Close:
        default:
            return(false);
        }
    }
    return(true);
}


void MainWindow::sync2zone()
{
    auto		*inputScroll	= ui->inputArea->verticalScrollBar();
    auto		*displayScroll	= ui->display->verticalScrollBar();
    const auto &	pos		= inputScroll->value(),
    &inMin				= inputScroll->minimum(),
    &inMax				= inputScroll->maximum();
    const auto & dispMin		= displayScroll->minimum(),
             dispMax		= displayScroll->maximum();
    auto dispPos			=
        (inMax - inMin) ?
        pos * (dispMax - dispMin) / (inMax - inMin) : 0;

    displayScroll->setValue( dispPos );
}


void MainWindow::textEdit2display()
{
    try{
        auto		*inputArea	= ui->inputArea;
        auto		*display	= ui->display;
        const auto &	content		= inputArea->toPlainText();
        /*display->setPlainText( content.sliced( content.length() / 2 ) ); */
        display->setHtml( md2html( content ) );
        sync2zone();
    } catch ( const QException &e ) {
        qDebug() << e.what() << '\n';
    }
}


void MainWindow::on_new_action_triggered()
{
    if ( !checkFileSave() )
        return;
    filename.clear();
    ui->inputArea->clear();
    saved = true;
}


void MainWindow::on_save_as_action_triggered()
{
    if ( selectNewFile2Save() )
        write2file();
}

