#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qglobal.h>
#include <qlistwidget.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

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
  connect(ui->btn_add_03, &QPushButton::clicked, this,
          &MainWindow::create_fun_code_03_widget);
  connect(ui->lw_widget_list, &QListWidget::itemPressed, this,
          &MainWindow::onListWidgetMousePressEvent, Qt::QueuedConnection);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::get_serial_and_update_ui() {
  QList<QSerialPortInfo> serialPort_info_list =
      QSerialPortInfo::availablePorts();
  ui->cbx_serial_number->clear();
  qDebug() << "serialPort_info_list size:" << serialPort_info_list.size();
  if (serialPort_info_list.size() == 0) {
    ui->cbx_serial_number->addItem("No serial port available");
  }
  for (int i = 0; i < serialPort_info_list.size(); i++) {
    qDebug() << "serialPort_info_list[" << i
             << "].portName():" << serialPort_info_list[i].portName();
    ui->cbx_serial_number->addItem(serialPort_info_list[i].portName());
  }
}

void MainWindow::switch_serial_port() {
  qDebug() << "switch_serial_port";
  serialPort->setPortName(ui->cbx_serial_number->currentText());
  serialPort->setBaudRate(ui->cbx_baud->currentText().toUInt(),
                          QSerialPort::AllDirections);
  serialPort->setDataBits(QSerialPort::Data8);
  serialPort->setParity(QSerialPort::NoParity);
  serialPort->setStopBits(QSerialPort::OneStop);
  serialPort->setFlowControl(QSerialPort::NoFlowControl);
  if (serialPort->isOpen()) {
    qDebug() << "port close";
    serialPort->close();
    ui->btn_serial_switch->setText("Connect");
  } else {
    bool ok = serialPort->open(QIODevice::ReadWrite);
    if (ok) {
      qDebug() << "port open";
      ui->btn_serial_switch->setText("Disconnect");
      serialPort->clear();
    } else {
      qDebug() << "port open failed";
      QMessageBox::warning(this, "Warning", "Open serial port failed!");
    }
  }
}

void MainWindow::read_serial_data() {
  // timer->stop();
  QByteArray data = serialPort->readAll();
  rx_buffer.append(data);
  timer->start();
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

void MainWindow::onListWidgetMousePressEvent(QListWidgetItem *item) {
  qDebug() << "onListWidgetMousePressEvent";

  if (!item) {
    qDebug() << "item is false  ";
    return;
  }
  delete ui->lw_widget_list->takeItem(ui->lw_widget_list->row(item));
}

void MainWindow::create_fun_code_03_widget() {
  try {
    qDebug() << "create_fun_code_03_widget";
    FunCodeWidget_03 *fun_code_widget = new FunCodeWidget_03(this);
    fun_code_widgets.append(fun_code_widget);
    QListWidgetItem *item = new QListWidgetItem(ui->lw_widget_list);
    item->setSizeHint(QSize(ui->lw_widget_list->width(), 110));
    ui->lw_widget_list->setItemWidget(item, fun_code_widget);
    ui->lw_widget_list->setStyleSheet(
        "QListWidget::item:hover { background-color: transparent; }"
        "QListWidget::item:selected { border: 2px solid #007bff; }");
  } catch (...) {
    // 捕获所有类型的异常
    QMessageBox::warning(this, "Warning", "Create widget failed!");
    throw; // 重新抛出异常
  }
}
void MainWindow::create_fun_code_06_widget() {}
void MainWindow::create_fun_code_16_widget() {}