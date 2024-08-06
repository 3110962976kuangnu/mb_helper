#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QQueue>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>


#include "funcode_widget.h"
#include "modbus_crc.h"

struct one_task {
  QWidget *sender;
  QByteArray data;
  bool iswaiting;
};

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

  void onListWidgetMousePressEvent(QListWidgetItem *item);

  void data_send_to_serial(QByteArray data);
  void package_timer_out();

private:
  Ui::MainWindow *ui;
  QSerialPort *serialPort;
  QTimer *timer;
  QTimer *package_no_response_timer;
  QByteArray rx_buffer;
  QList<FunCodeWidgetBase *> fun_code_widgets;
  // QList<QWidget *> widget_buf;
  QQueue<one_task> *task_queue;
};
#endif // MAINWINDOW_H
