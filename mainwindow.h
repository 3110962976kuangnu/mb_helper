#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>
#include <cstdint>
#include <qglobal.h>
#include <qlist.h>
#include <qtimer.h>

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
  void get_serial_and_update_ui();
  void switch_serial_port();
  void read_serial_data();
  void send_test_data();
  void parse_serial_data();

private:
  Ui::MainWindow *ui;
  QSerialPort *serialPort;
  QTimer *timer;
  QByteArray rx_buffer;
};
#endif // MAINWINDOW_H
