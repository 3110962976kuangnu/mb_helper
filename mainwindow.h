#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>
#include <qlist.h>

#include "funcode_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void get_serial_and_update_ui();
  void switch_serial_port();
  void read_serial_data();
  void send_test_data();
  void parse_serial_data();

  void create_fun_code_03_widget();
  void create_fun_code_06_widget();
  void create_fun_code_16_widget();

private:
  Ui::MainWindow *ui;
  QSerialPort *serialPort;
  QTimer *timer;
  QByteArray rx_buffer;
  QList<FunCodeWidgetBase *> fun_code_widgets;
};
#endif // MAINWINDOW_H
