#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qcombobox.h>
#include <qglobal.h>
#include <qlist.h>
#include <qserialportinfo.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  get_serial_and_update_ui();
    // connect(ui->btn_scan_port, SIGNAL(clicked()), this, SLOT(get_serial_and_update_ui()));
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
