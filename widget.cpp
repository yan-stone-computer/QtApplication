#include "widget.h"
#include "./ui_widget.h"
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QPixmap>
#include<QIcon>
#include<QLineEdit>
#include<QInputDialog>
#include<QMessageBox>
#include <QFile>
#include <QTextStream>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("个人待办清单");
    // 设置pushButton_3（辅助按钮，如"打开"）样式
    ui->pushButton_3->setStyleSheet(R"(
        #pushButton_3 {
            background-color: #FF8C00; /* 橙色背景 */
            color: #FFFFFF; /* 白色文字 */
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
        }
        #pushButton_3:hover {
            background-color: #FF7043; /* 稍深的橙色 */
            box-shadow: 0 2px 6px rgba(0, 0, 0, 0.15);
        }
        #pushButton_3:pressed {
            background-color: #F57C00; /* 按压时更深的橙色 */
            color: #FAFAFA;
            box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.2);
        }
        #pushButton_3:disabled {
            background-color: #FFCC80;
            color: #FFFFFF;
        }
    )");

    // 设置pushButton_4（主要按钮，如"保存"）样式
    ui->pushButton_4->setStyleSheet(R"(
        #pushButton_4 {
            background-color: #FF8C00; /* 橙色背景 */
            color: #FFFFFF; /* 白色文字 */
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
        }
        #pushButton_4:hover {
            background-color: #FF7043; /* 稍深的橙色 */
            box-shadow: 0 2px 6px rgba(0, 0, 0, 0.15);
        }
        #pushButton_4:pressed {
            background-color: #F57C00; /* 按压时更深的橙色 */
            color: #FAFAFA;
            box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.2);
        }
        #pushButton_4:disabled {
            background-color: #FFCC80;
            color: #FFFFFF;
        }
    )");
    // 创建树控件并设置为成员变量，方便后续访问
    tree = new QTreeWidget(this);
    tree->setEditTriggers(QAbstractItemView::DoubleClicked);
    tree->resize(400, 400);
    tree->setHeaderLabel("待办清单分组");

    // 创建根节点
    root = new QTreeWidgetItem();
    root->setText(0, "个人待办清单子项");
    root->setFlags(root->flags() | Qt::ItemIsEditable);

    // 关键步骤：将根节点添加到树控件中
    tree->addTopLevelItem(root);

    // 创建子项
    QTreeWidgetItem *root1 = new QTreeWidgetItem(root);
    root1->setText(0, "可以填写待办清单了");
    root1->setCheckState(0, Qt::Unchecked);
    root1->setFlags(root1->flags() | Qt::ItemIsEditable);
    // 自动展开根节点
    tree->expandItem(root);
    // 设置按钮图标
    QPixmap pix;
    pix.load(":/plus.png");
    QIcon icon(pix);
    ui->pushButton->setIcon(icon);
    ui->pushButton->setIconSize(ui->pushButton->size());
    //设置label图标
    QPixmap pix2;
    pix2.load(":/help2.webp");
    QIcon icon2(pix2);
    ui->pushButton_2->setIcon(icon2);
    ui->pushButton_2->setIconSize(ui->pushButton_2->size());


    connect(tree, &QTreeWidget::itemChanged, this, &Widget::on_checkStateChanged);
    // 在构造函数中
    // 在Widget构造函数中添加以下代码（初始化tree之后）
    // 设置右键菜单策略
    tree->setContextMenuPolicy(Qt::CustomContextMenu);


    connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
        // 直接在代码中定义帮助文档内容（多行字符串）
        QString helpContent = R"(个人待办清单使用帮助
一、功能概述
“个人待办清单” 是一款轻量级任务管理工具，帮你便捷记录、管理待办事项，支持新增任务、标记完成、编辑任务、删除任务、保存 / 加载清单，让日常任务安排更清晰高效。
二、基础操作
（一）界面布局
树状清单区：展示任务分组与子任务，默认含 “个人待办清单子项” 分组，可展开 / 折叠，直观管理任务层级。
功能按钮区（底部）：
橙色按钮（+）：快速新增待办任务；
“打开” 按钮：加载本地保存的待办清单文件；
“保存” 按钮：将当前清单内容导出为文本文件；
“？” 按钮：查看本帮助文档。
（二）核心功能
1. 新增任务
点击底部橙色 “+” 按钮 → 弹出输入框 → 填写任务内容 → 确认后，任务自动添加到 “个人待办清单子项” 分组下，默认未勾选状态。
2. 标记任务完成 / 未完成
标记完成：点击任务前的复选框（□），勾选后任务文字自动添加删除线、变灰色，标记为已完成；
取消完成：再次点击复选框，取消勾选后文字恢复黑色、删除线消失，标记为未完成。
3. 编辑任务内容
双击任务文字 → 进入编辑模式（文字变为可修改状态） → 输入新内容 → 按回车键 / 点击空白处确认修改。
4. 删除任务
右键点击需删除的任务 → 弹出 “删除确认” 对话框 → 点击 “是”，任务从清单中移除；点击 “否”，取消删除。
5. 保存清单
点击 **“保存” 按钮 ** → 选择保存路径、设置文件名（默认.txt格式） → 确认后，当前清单内容（仅任务文字）保存为文本文件，方便下次加载。
6. 加载清单
点击 **“打开” 按钮 ** → 选择本地保存的待办清单文件（.txt格式） → 确认后，文件内容自动加载为新任务，覆盖当前清单（若有内容需提前备份）。
三、注意事项
文件格式：保存 / 加载仅支持.txt格式，若手动修改文件内容，需确保格式简单（每行对应一个任务），否则可能加载异常；
数据覆盖：加载新文件会清空当前清单内容，如需保留可先保存当前清单；
交互限制：根分组（“个人待办清单子项”）无法删除 / 重命名，仅支持新增子任务，聚焦核心任务管理。
若使用中遇到问题，可再次点击 “？” 按钮查看帮助，或联系开发者反馈～)";

        // 显示帮助内容
        QMessageBox::information(this, "使用帮助", helpContent,  // 直接使用内置的字符串作为帮助内容
        QMessageBox::Ok);
    });

    // 用匿名函数实现右键点击逻辑
    connect(tree, &QTreeWidget::customContextMenuRequested, this, [this](const QPoint &pos) {
        // 获取右键点击的项
        QTreeWidgetItem *item = tree->itemAt(pos);

        // 只处理非根节点的有效项
        if (item && item != root) {
            // 弹出确认对话框
            QMessageBox::StandardButton reply = QMessageBox::question(this,"删除确认",
                QString("确定要删除 \"%1\" 吗？").arg(item->text(0)), // 显示项的文本
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No // 默认选中"否"，防止误操作
                );
            if (reply == QMessageBox::Yes) {
                // 子节点一定有父节点，直接让父节点移除它
                item->parent()->removeChild(item);
                // 释放内存
                delete item;
            }
        }
    });

}

Widget::~Widget()
{
    delete ui;
}
// 处理复选框状态变化，设置文字样式
void Widget::on_checkStateChanged(QTreeWidgetItem *item, int column)
{
    if (column == 0) { // 只处理第一列
        QFont font = item->font(0);
        // 选中状态时添加删除线，未选中时取消
        font.setStrikeOut(item->checkState(0) == Qt::Checked);
        // 选中状态时文字变灰，未选中时恢复黑色
        QColor color = (item->checkState(0) == Qt::Checked) ? Qt::gray : Qt::black;
        item->setForeground(0, color);
        item->setFont(0, font);
    }
}

void Widget::on_pushButton_clicked()
{
    bool ok;
    QString str = QInputDialog::getText(this, tr("新增待办事项"),tr("输入文字:"), QLineEdit::Normal, "", &ok);
    if (ok && !str.isEmpty()) {
        // 新增项作为root的子项
        QTreeWidgetItem *newItem = new QTreeWidgetItem(root);
        newItem->setText(0, str);
        newItem->setCheckState(0, Qt::Unchecked);
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

        // 自动展开节点，方便查看新增项
        tree->expandItem(root);
    }
}

//打开
void Widget::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "打开待办清单", "", "待办清单文件 (*.txt)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + file.errorString());
        return;
    }

    // 先清除所有现有子项
    tree->clear();

    // 创建根项
    root = new QTreeWidgetItem(tree);
    root->setText(0, "待办事项");
    tree->addTopLevelItem(root);

    QTextStream in(&file);
    //in.setCodec("UTF-8");

    int lineCount = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineCount++;

        // 跳过空行
        if (line.isEmpty()) continue;

        // 修改：直接使用文本内容创建待办项，默认状态为未选中
        QTreeWidgetItem *newItem = new QTreeWidgetItem(root);
        newItem->setText(0, line);
        newItem->setCheckState(0, Qt::Unchecked); // 修改：默认设置为未选中状态
        newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    }

    file.close();
    tree->expandItem(root);

    QMessageBox::information(this, "成功", QString("待办清单已加载，共 %1 个项目").arg(root->childCount()));
}

//保存
void Widget::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "保存待办清单", "", "待办清单文件 (*.txt)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    //out.setCodec("UTF-8");

    // 保存每个子项的内容（只保存文本，不保存状态）
    for (int i = 0; i < root->childCount(); ++i) {
        QTreeWidgetItem *item = root->child(i);
        out << item->text(0) << "\n"; // 修改：只保存文本内容，不保存状态
    }

    file.close();
    QMessageBox::information(this, "成功", "待办清单已保存");
}
