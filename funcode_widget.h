#ifndef FUNCODE_WIDGET_H
#define FUNCODE_WIDGET_H

#include <QDebug>
#include <QGridLayout>
#include <QMainWindow>

#include <QThread>
#include <QTimer>

#include <QCheckBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushbutton>
#include <QStandardItemModel>
#include <QTableView>
#include <Qwidget>


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
  QLabel *label_interval_ms;
  QLineEdit *le_reg_edit;
  QLineEdit *le_data_or_len;
  QLineEdit *le_interval_ms;
  QCheckBox *is_auto_send;
  QPushButton *pb_read;
  QTableView *table_data;
  QStandardItemModel *model_data;

  FunCodeWidget_03(QWidget *parent = nullptr) : FunCodeWidgetBase(parent) {
    qDebug() << "FunCode 03 start 1";
    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(16);
    label_fncode = new QLabel("FunCode: 03", this);

    label_reg_address = new QLabel("Reg Address:", this);
    label_reg_value_or_len = new QLabel("Reg Len:", this);
    label_interval_ms = new QLabel("Interval(ms):", this);

    le_reg_edit = new QLineEdit(this);
    le_data_or_len = new QLineEdit(this);
    le_interval_ms = new QLineEdit(this);

    is_auto_send = new QCheckBox("Auto Send", this);
    pb_read = new QPushButton("Read", this);

    table_data = new QTableView(this);
    model_data = new QStandardItemModel;
    table_data->setModel(model_data);
    table_data->setHorizontalHeader(new QHeaderView(Qt::Horizontal));
    table_data->setVerticalHeader(new QHeaderView(Qt::Vertical));
    table_data->horizontalHeader()->setVisible(false);
    table_data->verticalHeader()->setVisible(false);
    table_data->setFixedHeight(63);

    label_fncode->setFont(font);
    label_reg_address->setFont(font);
    label_reg_value_or_len->setFont(font);
    label_interval_ms->setFont(font);

    label_reg_address->setAlignment(Qt::AlignRight);
    label_reg_value_or_len->setAlignment(Qt::AlignRight);
    label_interval_ms->setAlignment(Qt::AlignRight);

    le_reg_edit->setMaximumWidth(30);
    le_data_or_len->setMaximumWidth(30);
    le_interval_ms->setMaximumWidth(30);
    // this->setMaximumHeight(100);

    this->gridLayout->addWidget(label_fncode, 0, 0, 1, 1);
    this->gridLayout->addWidget(label_reg_address, 0, 1, 1, 1);
    this->gridLayout->addWidget(le_reg_edit, 0, 2, 1, 1);
    this->gridLayout->addWidget(label_reg_value_or_len, 0, 3, 1, 1);
    this->gridLayout->addWidget(le_data_or_len, 0, 4, 1, 1);
    this->gridLayout->addWidget(label_interval_ms, 0, 5, 1, 1);
    this->gridLayout->addWidget(le_interval_ms, 0, 6, 1, 1);
    this->gridLayout->addWidget(is_auto_send, 0, 7, 1, 1);
    this->gridLayout->addWidget(pb_read, 1, 0, 1, 1);
    this->gridLayout->addWidget(table_data, 1, 1, 1, 7);

    this->setContentsMargins(5, 5, 5, 5);


    QByteArray data = QByteArray::fromHex("01 02 03 04");
    add_data_to_table(data);
  }
  ~FunCodeWidget_03() {
    delete label_fncode;
    delete label_reg_address;
    delete label_reg_value_or_len;
    delete le_reg_edit;
    delete le_data_or_len;
    delete le_interval_ms;
    delete is_auto_send;
    delete pb_read;
    delete table_data;
  }
  bool parse_funcode(QByteArray data) override {
    qDebug() << "FunCode 03";
    return true;
  }

  void add_data_to_table(QByteArray data) {
    data = data.trimmed();
    for (qint8 i = 0; i < data.size(); i++) {
      QStandardItem *item1 = new QStandardItem(QString::number(data.at(i)));
      QStandardItem *item2 = new QStandardItem(QString::number(i * 2));
      model_data->setItem(0, i, item1);
      model_data->setItem(1, i, item2);
    };
  }
};

#endif // FUNCODE_WIDGET_H