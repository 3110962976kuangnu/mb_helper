#include "funcode_widget.h"
#include <cstdint>
#include <qobjectdefs.h>
#include <qstandarditemmodel.h>

FunCodeWidget_10::FunCodeWidget_10(QWidget *parent)
    : FunCodeWidgetBase(parent) {
  qDebug() << "FunCodeWidget_10 constructor called";
  QFont font;
  font.setPointSize(16);
  label_funcode = new QLabel("Fun Code 10");
  label_reg_address = new QLabel("Reg Address:");
  label_reg_number = new QLabel("Reg Number:");
  le_reg_address = new QLineEdit();
  le_reg_number = new QLineEdit();
  pb_make_request = new QPushButton("Make Request");
  pb_write = new QPushButton("write");
  table_data = new QTableView();
  model_data = new QStandardItemModel(0, 2);
  table_data->setModel(model_data);
  table_data->setHorizontalHeader(new QHeaderView(Qt::Horizontal));
  table_data->setVerticalHeader(new QHeaderView(Qt::Vertical));
  table_data->horizontalHeader()->setVisible(false);
  table_data->verticalHeader()->setVisible(false);
  table_data->setFixedHeight(80);

  label_funcode->setFont(font);
  label_reg_address->setFont(font);
  label_reg_number->setFont(font);

  this->gridLayout->addWidget(label_funcode, 0, 0, 1, 1);
  this->gridLayout->addWidget(label_reg_address, 0, 1, 1, 1);
  this->gridLayout->addWidget(le_reg_address, 0, 2, 1, 1);
  this->gridLayout->addWidget(label_reg_number, 0, 3, 1, 1);
  this->gridLayout->addWidget(le_reg_number, 0, 4, 1, 1);
  this->gridLayout->addWidget(pb_make_request, 0, 5, 1, 1);
  this->gridLayout->addWidget(pb_write, 1, 0, 1, 1);
  this->gridLayout->addWidget(table_data, 1, 1, 1, 5);

  connect(pb_make_request, &QPushButton::clicked, this,
          &FunCodeWidget_10::make_request_data_table);
  connect(pb_write, &QPushButton::clicked, this,
          &FunCodeWidget_10::send_data_to_main);
}
FunCodeWidget_10::~FunCodeWidget_10() {
  delete label_funcode;
  delete label_reg_address;
  delete label_reg_number;
  delete le_reg_address;
  delete pb_make_request;
  delete pb_write;
  delete table_data;
  delete model_data;
}
bool FunCodeWidget_10::parse_funcode(QByteArray data) {}
void FunCodeWidget_10::send_data_to_main() {
  int colums_number = this->le_reg_number->text().toUInt();
  uint16_t reg_address_start = this->le_reg_address->text().toUInt();
  QByteArray data;
  data.append(0x10);
  data.append(reg_address_start >> 8);
  data.append(reg_address_start & 0xff);
  data.append(colums_number >> 8);
  data.append(colums_number & 0xff);
  data.append(colums_number * 2);

  for (uint8_t i = 0; i < colums_number; i++) {
    QStandardItem *item1 = this->model_data->item(1, i + 1);
    uint16_t value = item1->text().toUInt();
    data.append(value >> 8);
    data.append(value & 0xff);
  }
  emit send_require(data);
}
void FunCodeWidget_10::make_request_data_table() {
  int colums_number = this->le_reg_number->text().toUInt();
  uint16_t reg_address_start = this->le_reg_address->text().toUInt();
  if (colums_number == 0) {
    colums_number += 3;
  }
  model_data->setColumnCount(colums_number + 1);
  QStandardItem *item1 = new QStandardItem();
  QStandardItem *item2 = new QStandardItem();
  item1->setText("地址");
  item2->setText("值");
  model_data->setItem(0, 0, item1);
  model_data->setItem(1, 0, item2);

  for (int i = 0; i < colums_number; i++) {
    QStandardItem *item1 = new QStandardItem();
    QStandardItem *item2 = new QStandardItem();
    item1->setText(QString::number(reg_address_start + i));
    item2->setText("100");
    model_data->setItem(0, i + 1, item1);
    model_data->setItem(1, i + 1, item2);
  }
}