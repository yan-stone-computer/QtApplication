#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_checkStateChanged(QTreeWidgetItem *item, int column);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    QTreeWidget *tree;    // 树控件成员变量
    QTreeWidgetItem *root; // 根节点成员变量

};
#endif // WIDGET_H
