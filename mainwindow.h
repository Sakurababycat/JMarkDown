#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QScrollBar"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

private slots:
    void on_exit_action_triggered();


    void on_about_action_triggered();


    void on_open_action_triggered();


    void on_save_action_triggered();


    void textEdit2display();


    void sync2zone( QScrollBar *, QScrollBar * );


    bool checkFileSave();


    void on_new_action_triggered();


    void on_save_as_action_triggered();


protected:
    void closeEvent( QCloseEvent *event ) override;


private:
    bool save();


    bool selectNewFile2Save();


    void write2file();


    Ui::MainWindow *ui;
    QString filename;
    bool saved;
};
#endif /* MAINWINDOW_H */
