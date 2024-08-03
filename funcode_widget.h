#ifndef FUNCODE_WIDGET_H
#define FUNCODE_WIDGET_H

#include <QDebug>
#include <QGridLayout>
#include <QMainWindow>

#include <QThread>
#include <QTimer>

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushbutton>
#include <QTableWidget>
#include <Qwidget>
#include <qglobal.h>
#include <qobject.h>
#include <qtablewidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class FunCodeWidgetBase;
class FunCodeWidget_03;
} // namespace Ui
QT_END_NAMESPACE

class FunCodeWidgetBase : public QWidget {
  Q_OBJECT
public:
  QGridLayout *gridLayout;

  FunCodeWidgetBase(QWidget *parent = nullptr) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(gridLayout);
  }
  virtual ~FunCodeWidgetBase() { delete gridLayout; }
  virtual bool parse_funcode(QByteArray data) = 0;
};

class FunCodeWidget_03 : public FunCodeWidgetBase {
  Q_OBJECT
public:
  QLabel *label_fncode;
  QLabel *label_reg_address;
  QLabel *label_reg_value_or_len;
  QLineEdit *le_reg_edit;
  QLineEdit *le_data_or_len;
  QLineEdit *le_interval_ms;
  QPushButton *pb_auto_send;
  QPushButton *pb_read;
  QTableWidget *table_data;

  FunCodeWidget_03(QWidget *parent = nullptr) : FunCodeWidgetBase(parent) {
    qDebug() << "FunCode 03 start 1";
    label_fncode = new QLabel("FunCode: 03", this);

    label_reg_address = new QLabel("Reg Address:", this);
    label_reg_value_or_len = new QLabel("Reg Len:", this);
    le_reg_edit = new QLineEdit(this);

    le_data_or_len = new QLineEdit(this);
    le_interval_ms = new QLineEdit(this);
    pb_auto_send = new QPushButton("Auto Send", this);
    pb_read = new QPushButton("Read", this);
    table_data = new QTableWidget(this);
    table_data->
    table_data->setRowCount(2);
    table_data->setColumnCount(1);
    le_reg_edit->setMaximumWidth(50);
    le_data_or_len->setMaximumWidth(50);
    le_interval_ms->setMaximumWidth(50);

    this->gridLayout->addWidget(label_fncode, 0, 0, 1, 1);
    this->gridLayout->addWidget(label_reg_address, 0, 1, 1, 1);
    this->gridLayout->addWidget(le_reg_edit, 0, 2, 1, 1);
    this->gridLayout->addWidget(label_reg_value_or_len, 0, 3, 1, 1);
    this->gridLayout->addWidget(le_data_or_len, 0, 4, 1, 1);
    this->gridLayout->addWidget(le_interval_ms, 0, 5, 1, 1);
    this->gridLayout->addWidget(pb_auto_send, 0, 6, 1, 1);
    this->gridLayout->addWidget(pb_read, 1, 0, 1, 1);
    this->gridLayout->addWidget(table_data, 1, 1, 1, 6);
  }
  ~FunCodeWidget_03() {
    delete label_fncode;
    // delete label_reg_address;
    // delete label_reg_value_or_len;
    // delete le_reg_edit;
    // delete le_data_or_len;
    // delete le_interval_ms;
    // delete pb_auto_send;
    // delete pb_read;
    // delete table_data;
  }
  bool parse_funcode(QByteArray data) override {
    qDebug() << "FunCode 03";
    return true;
  }
};

#endif // FUNCODE_WIDGET_H