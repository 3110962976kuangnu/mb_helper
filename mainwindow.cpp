#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qchar.h>
#include <qcombobox.h>
#include <qglobal.h>
#include <qlist.h>
#include <qlocale.h>
#include <qpushbutton.h>
#include <qserialportinfo.h>
#include <qthread.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  serialPort = new QSerialPort(this);
  timer = new QTimer(this);
  timer->setSingleShot(true);
  timer->setInterval(10);
  rx_buffer.clear();
  ui->cbx_baud->addItems({"9600", "19200", "38400", "57600", "115200"});
  get_serial_and_update_ui();
  connect(ui->btn_scan_port, &QPushButton::clicked, this,
          &MainWindow::get_serial_and_update_ui);
  connect(ui->btn_serial_switch, &QPushButton::clicked, this,
          &MainWindow::switch_serial_port);
  connect(serialPort, &QSerialPort::readyRead, this,
          &MainWindow::read_serial_data);
  connect(ui->btn_send_test, &QPushButton::clicked, this,
          &MainWindow::send_test_data);
  connect(ui->btn_clear, &QPushButton::clicked, ui->te_receive,
          &QTextEdit::clear);
  connect(timer, &QTimer::timeout, this, &MainWindow::parse_serial_data);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::get_serial_and_update_ui() {
  QList<QSerialPortInfo> serialPort_info_list =
      QSerialPortInfo::availablePorts();
  ui->cbx_serial_number->clear();
  qDebug() << "serialPort_info_list size:" << serialPort_info_list.size();
  for (int i = 0; i < serialPort_info_list.size(); i++) {
    qDebug() << "serialPort_info_list[" << i
             << "].portName():" << serialPort_info_list[i].portName();
    ui->cbx_serial_number->addItem(serialPort_info_list[i].portName());
  }
}

void MainWindow::switch_serial_port() {
  qDebug() << "switch_serial_port";
  serialPort->setPortName(ui->cbx_serial_number->currentText());
  qDebug() << "switch_serial_port1";
  serialPort->setBaudRate(ui->cbx_baud->currentText().toUInt(),
                          QSerialPort::AllDirections);
  qDebug() << "switch_serial_port2";
  serialPort->setDataBits(QSerialPort::Data8);
  qDebug() << "switch_serial_port3";
  serialPort->setParity(QSerialPort::NoParity);
  qDebug() << "switch_serial_port4";
  serialPort->setStopBits(QSerialPort::OneStop);
  qDebug() << "switch_serial_port5";
  serialPort->setFlowControl(QSerialPort::NoFlowControl);
  qDebug() << "switch_serial_port6";
  if (serialPort->isOpen()) {
    qDebug() << "port close";
    serialPort->close();
    ui->btn_serial_switch->setText("Connect");
  } else {
    qDebug() << "port open";
    serialPort->open(QIODevice::ReadWrite);
    ui->btn_serial_switch->setText("Disconnect");
    serialPort->clear();
  }
}

void MainWindow::read_serial_data() {
  // timer->stop();
  QByteArray data = serialPort->readAll();
  rx_buffer.append(data);
  timer->start();
  // QString str = QString::fromUtf8(data.data());
  // ui->te_receive->append(str);
  // ui->te_receive->setText(str);
}
void MainWindow::send_test_data() {
  QByteArray data = "hello world";
  serialPort->write(data);
}
void MainWindow::parse_serial_data() {
  QString str = QString::fromUtf8(rx_buffer.data());
  ui->te_receive->append(str);
  rx_buffer.clear();
}