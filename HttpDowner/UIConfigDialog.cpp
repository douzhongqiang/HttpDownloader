#include "UIConfigDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include "UICustomCore/CustomStyleConfig.h"

UIConfigDialog::UIConfigDialog(QWidget *parent)
    : CustomDialog(parent)
{
    QWidget* pW = createContentWidget();
    this->setContentWidget(pW);

    this->resize(650, 180);
    this->setWindowTitle(tr("Download Config"));
}

UIConfigDialog::~UIConfigDialog()
{

}

QWidget* UIConfigDialog::createContentWidget(void)
{
    QWidget* widget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(widget);

    // 文件名
    m_pFileNameLineEdit = new UICustomLineEditControl;
    g_StyleConfig->setCurrentStyle(m_pFileNameLineEdit, "DefaultWidget", true);
    m_pFileNameLineEdit->setColumnWidth(0, 220);
    m_pFileNameLineEdit->setTagText(tr("Please InPut FileName: "));
    m_pFileNameLineEdit->setToolButtonVisible(false);
    m_pFileNameLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(m_pFileNameLineEdit);

    // 文件路径
    m_pFilePathLineEdit = new UICustomLineEditControl;
    g_StyleConfig->setCurrentStyle(m_pFilePathLineEdit, "DefaultWidget", true);
    m_pFilePathLineEdit->setColumnWidth(0, 220);
    m_pFilePathLineEdit->setTagText(tr("Please Select FilePath: "));
    m_pFilePathLineEdit->setToolButtonText(tr("Browse"));
    m_pFilePathLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QObject::connect(m_pFilePathLineEdit, &UICustomLineEditControl::clickedToolButton, \
                     this, &UIConfigDialog::onBrowerFilePath);
    mainLayout->addWidget(m_pFilePathLineEdit);

    mainLayout->addStretch();

    // 添加按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();

    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setFixedSize(70, 35);
    bottomLayout->addWidget(okButton);
    QObject::connect(okButton, &QPushButton::clicked, this, &UIConfigDialog::accept);

    QPushButton* cannelButton = new QPushButton(tr("Cannel"));
    cannelButton->setFixedSize(70, 35);
    bottomLayout->addWidget(cannelButton);
    QObject::connect(cannelButton, &QPushButton::clicked, this, &UIConfigDialog::reject);
    mainLayout->addLayout(bottomLayout);

    return widget;
}

void UIConfigDialog::setFilePath(const QString& filename)
{
    QFileInfo info(filename);
    m_pFilePathLineEdit->setText(info.absolutePath());
    m_pFileNameLineEdit->setText(info.fileName());
}

QString UIConfigDialog::getFilePath(void)
{
    QString filePath = m_pFilePathLineEdit->getText();
    QString fileName = m_pFileNameLineEdit->getText();

    QString destFilePath = filePath + "/" + fileName;
    return destFilePath;
}

void UIConfigDialog::onBrowerFilePath(void)
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Dir:"), m_pFilePathLineEdit->getText());
    if (dirPath.isEmpty())
        return;

    m_pFilePathLineEdit->setText(dirPath);
}
