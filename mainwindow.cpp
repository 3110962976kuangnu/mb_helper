#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "funcode_widget.h"
#include <qdebug.h>
#include <qglobal.h>
#include <qpushbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->le_slave_address->setText("1");
  task_queue = new QQueue<one_task>();
  serialPort = new QSerialPort(this);
  timer = new QTimer(this);
  timer->setSingleShot(true);
  timer->setInterval(10);
  package_no_response_timer = new QTimer(this);
  package_no_response_timer->setSingleShot(true);
  package_no_response_timer->setInterval(1000);
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
  connect(package_no_response_timer, &QTimer::timeout, this,
          &MainWindow::package_timer_out);
  connect(ui->btn_add_03, &QPushButton::clicked, this,
          &MainWindow::create_fun_code_03_widget);
  connect(ui->btn_add_06, &QPushButton::clicked, this,
          &MainWindow::create_fun_code_06_widget);
  connect(ui->btn_add_10, &QPushButton::clicked, this,
          &MainWindow::create_fun_code_10_widget);
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
  QString str = QString::fromUtf8(rx_buffer.toHex());

  for (int i = str.size() - 2; i > 0; i -= 2) {
    str.insert(i, ' ');
  }
  ui->te_receive->append(str);

  if (!task_queue->isEmpty()) {
    qDebug() << "task_queue is not empty";
    one_task &task = task_queue->head();
    qDebug() << "task.data:" << task.data.toHex();
    qDebug() << "rx_buffer:" << rx_buffer.toHex();
    // if (rx_buffer.contains(task.data)) {
    //   qDebug() << "rx_buffer match task.data";
    //   package_no_response_timer->start();
    //   rx_buffer.clear();
    //   return;
    // }
    package_no_response_timer->stop();
    qobject_cast<FunCodeWidgetBase *>(task.sender)->parse_funcode(rx_buffer);
    task_queue->dequeue();
    if (!task_queue->isEmpty()) {
      one_task &task1 = task_queue->head();
      QByteArray send_data;
      send_data.append(ui->le_slave_address->text().toUShort());
      send_data.append(task1.data);
      quint16 crc = ModbusCRC16(send_data);
      send_data.append(crc & 0xFF);
      send_data.append((crc >> 8) & 0xFF);
      qDebug() << "send_data:" << send_data.toHex();
      if (serialPort->isOpen()) {
        task1.iswaiting = true;
        serialPort->write(send_data);
        package_no_response_timer->setInterval(ui->le_interval->text().toInt());
        package_no_response_timer->start();
      } else {
        qDebug() << "serialPort is not open";
      }
    }
  } else {
    qDebug() << "task_queue is  empty";
  }
  rx_buffer.clear();
}

void MainWindow::onListWidgetMousePressEvent(QListWidgetItem *item) {
  qDebug() << "onListWidgetMousePressEvent";

  if (!item) {
    qDebug() << "item is false  ";
    return;
  }
  FunCodeWidgetBase *fun_code_widget =
      fun_code_widgets.at(ui->lw_widget_list->row(item));
  disconnect(fun_code_widget);
  delete ui->lw_widget_list->takeItem(ui->lw_widget_list->row(item));
  fun_code_widgets.removeOne(fun_code_widget);
}

void MainWindow::create_fun_code_03_widget() {
  try {
    qDebug() << "create_fun_code_03_widget";
    FunCodeWidget_03 *fun_code_widget = new FunCodeWidget_03(this);
    fun_code_widgets.append(fun_code_widget);
    QListWidgetItem *item = new QListWidgetItem(ui->lw_widget_list);
    item->setSizeHint(QSize(ui->lw_widget_list->width(), 120));
    ui->lw_widget_list->setItemWidget(item, fun_code_widget);

    connect(fun_code_widget, &FunCodeWidget_03::send_require, this,
            &MainWindow::data_send_to_serial);

  } catch (...) {
    // 捕获所有类型的异常
    QMessageBox::warning(this, "Warning", "Create widget failed!");
    throw; // 重新抛出异常
  }
}
void MainWindow::create_fun_code_06_widget() {
  qDebug() << "create_fun_code_06_widget";
  FunCodeWidget_06 *funcode_widget = new FunCodeWidget_06(this);
  fun_code_widgets.append(funcode_widget);
  QListWidgetItem *item = new QListWidgetItem(ui->lw_widget_list);
  item->setSizeHint(QSize(ui->lw_widget_list->width(), 120));
  ui->lw_widget_list->setItemWidget(item, funcode_widget);

  connect(funcode_widget, &FunCodeWidget_06::send_require, this,
          &MainWindow::data_send_to_serial);
}
void MainWindow::create_fun_code_10_widget() {
  qDebug() << "create_fun_code_10_widget";

  FunCodeWidget_10 *funcode_widget = new FunCodeWidget_10(this);
  fun_code_widgets.append(funcode_widget);
  QListWidgetItem *item = new QListWidgetItem(ui->lw_widget_list);
  item->setSizeHint(QSize(ui->lw_widget_list->width(), 120));
  ui->lw_widget_list->setItemWidget(item, funcode_widget);

  connect(funcode_widget, &FunCodeWidget_10::send_require, this,
          &MainWindow::data_send_to_serial);
}

void MainWindow::data_send_to_serial(QByteArray data) {
  QWidget *sender_widget = qobject_cast<QWidget *>(sender());
  if (sender_widget == nullptr) {
    qDebug() << "sender_widget is nullptr";
    return;
  }
  one_task new_task = {sender_widget, data, false};
  task_queue->enqueue(new_task);
  const one_task &task = task_queue->head();
  if (task.iswaiting) {
    qDebug() << "task is waiting";
    return;
  }
  QByteArray send_data;
  send_data.append(ui->le_slave_address->text().toUShort());
  send_data.append(data);
  quint16 crc = ModbusCRC16(send_data);
  send_data.append(crc & 0xFF);
  send_data.append((crc >> 8) & 0xFF);
  qDebug() << "send_data:" << send_data.toHex();
  if (serialPort->isOpen()) {
    one_task &task1 = task_queue->head();
    task1.iswaiting = true;
    serialPort->write(send_data);
    package_no_response_timer->setInterval(ui->le_interval->text().toInt());
    package_no_response_timer->start();
    qDebug() << "package_no_response_timer start";
  } else {
    qDebug() << "serialPort is not open";
  }
}

void MainWindow::package_timer_out() { qDebug() << "package_timer_out"; }