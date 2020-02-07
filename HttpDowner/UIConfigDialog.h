#ifndef UICONDIFDIALOG_H
#define UICONDIFDIALOG_H

#include <QWidget>
#include "UICustomWidgtes/CustomDialog.h"
#include "CustomCombineControl/UICustomLineEditControl.h"

class UIConfigDialog : public CustomDialog
{
    Q_OBJECT

public:
    UIConfigDialog(QWidget *parent = nullptr);
    ~UIConfigDialog();

    // 设置和获取文件路径
    void setFilePath(const QString& filename);
    QString getFilePath(void);

private:
    UICustomLineEditControl* m_pFileNameLineEdit = nullptr;
    UICustomLineEditControl* m_pFilePathLineEdit = nullptr;

    QWidget* createContentWidget(void);

private slots:
    void onBrowerFilePath(void);
};

#endif // WIDGET_H
