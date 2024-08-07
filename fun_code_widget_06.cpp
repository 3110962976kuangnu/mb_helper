#include "funcode_widget.h"
#include <cstdint>
#include <qdebug.h>
#include <qfont.h>
#include <qglobal.h>
#include <qmessagebox.h>

FunCodeWidget_06::FunCodeWidget_06(QWidget *parent)
    : FunCodeWidgetBase(parent) {
  qDebug() << "FunCodeWidget_06 constructor called";
  QFont font;
  font.setPointSize(16);
  label_funcode = new QLabel("FunCode 06", this);
  label_reg_address = new QLabel("Reg Address", this);
  label_reg_value = new QLabel("Reg Value", this);

  le_reg_address = new QLineEdit(this);
  le_reg_value = new QLineEdit(this);
  pb_write = new QPushButton("Write Reg", this);

  label_funcode->setFont(font);
  label_reg_address->setFont(font);
  label_reg_value->setFont(font);

  this->gridLayout->addWidget(label_funcode, 0, 0, 1, 1);
  this->gridLayout->addWidget(label_reg_address, 0, 1, 1, 1);
  this->gridLayout->addWidget(le_reg_address, 0, 2, 1, 1);
  this->gridLayout->addWidget(label_reg_value, 0, 3, 1, 1);
  this->gridLayout->addWidget(le_reg_value, 0, 4, 1, 1);
  this->gridLayout->addWidget(pb_write, 1, 0, 1, 1);

  /*  */
  /*  */
  connect(pb_write, &QPushButton::clicked, this,
          &FunCodeWidget_06::send_data_to_main);
}
FunCodeWidget_06::~FunCodeWidget_06() {
  delete label_funcode;
  delete label_reg_address;
  delete label_reg_value;
  delete le_reg_address;
  delete le_reg_value;
  delete pb_write;
}
bool FunCodeWidget_06::parse_funcode(QByteArray data) {
  qDebug() << "FunCodeWidget_06::parse_funcode() called";
  qDebug() << " 06 parse data:" << data.toHex();
  if (data.size() == 0) {
    QMessageBox::warning(this, "fun 06", "write error");
    return false;
  }
  QMessageBox::information(this, "fun 06", "write reg ok");
  return true;
}
void FunCodeWidget_06::send_data_to_main() {
  qDebug() << "FunCodeWidget_06::send_data_to_main() called";
  QByteArray data;
  uint16_t reg_address = le_reg_address->text().toUInt();
  uint16_t reg_value = le_reg_value->text().toUInt();
  data.append(0x06);
  data.append((reg_address >> 8) & 0xFF);
  data.append(reg_address & 0xFF);
  data.append((reg_value >> 8) & 0xFF);
  data.append(reg_value & 0xFF);
  emit send_require(data);
}