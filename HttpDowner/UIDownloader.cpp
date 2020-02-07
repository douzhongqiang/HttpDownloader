#include "UIDownloader.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QDesktopServices>
#include "CustomCombineControl/UICustomLineEditControl.h"
#include "UICustomCore/CustomStyleConfig.h"
#include "UIConfigDialog.h"

UIDownloadWidget::UIDownloadWidget(QWidget* parent)
    :CustomWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    m_pUrlLineEdit = new UICustomLineEditControl;
    m_pUrlLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(m_pUrlLineEdit);
    g_StyleConfig->setCurrentStyle(m_pUrlLineEdit, "DefaultWidget", true);
    m_pUrlLineEdit->setToolButtonText(tr("DownLoad"));
    m_pUrlLineEdit->setTagText(tr("Please Input Url:"));
    QObject::connect(m_pUrlLineEdit, &UICustomLineEditControl::clickedToolButton, \
                     this, &UIDownloadWidget::onAddDownUrl);

    m_pListWidget = new QListWidget;
    m_pListWidget->setSpacing(4);
    mainLayout->addWidget(m_pListWidget);
}

UIDownloadWidget::~UIDownloadWidget()
{

}

void UIDownloadWidget::addDownloadItem(const QString& urlStr, const QString& destFilePathName)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(100, 80));
    item->setBackground(QBrush(QColor(100, 100, 100)));
    m_pListWidget->addItem(item);
    m_itemList << item;

    // 创建子元素界面
    UIDownloadItemWidget* itemWidget = new UIDownloadItemWidget;
    itemWidget->setUrl(urlStr);
    itemWidget->setDestPathString(destFilePathName);
    m_itemWidgetList << itemWidget;
    QObject::connect(itemWidget, &UIDownloadItemWidget::clickedDeleteThis, \
                     this, &UIDownloadWidget::onDeleteItem);

    m_pListWidget->setItemWidget(item, itemWidget);
}

void UIDownloadWidget::onAddDownUrl(void)
{
    QString urlString = m_pUrlLineEdit->getText();
    if (urlString.isEmpty())
        return;

    // 解析这个URL
    QString fileName = "";
    int index = urlString.lastIndexOf("/");
    if (index >= 0)
        fileName = urlString.right(urlString.length() - index);

    // 设置路径
    QString filePath = "";
    if (!fileName.isEmpty())
        filePath = "./" + fileName;

    // 创建选择文件名和路径对话框
    UIConfigDialog configDialog;
    if (!filePath.isEmpty())
        configDialog.setFilePath(filePath);
    if (configDialog.exec() != UIConfigDialog::Accepted)
        return;

    filePath = configDialog.getFilePath();
    addDownloadItem(urlString, filePath);
}

void UIDownloadWidget::onDeleteItem(void)
{
    UIDownloadItemWidget* itemWidget = qobject_cast<UIDownloadItemWidget*>(sender());
    if (itemWidget == nullptr)
        return;

    int index = -1;
    int count = 0;
    // 查找位置
    for (auto iter = m_itemWidgetList.begin(); iter != m_itemWidgetList.end(); ++iter)
    {
        if (*iter == itemWidget)
        {
            index = count;
            break;
        }

        count++;
    }

    if (index < 0)
        return;

    QListWidgetItem* item = m_pListWidget->takeItem(index);
    m_itemList.removeAt(index);
    m_itemWidgetList.removeAt(index);

    delete item;
    itemWidget->deleteLater();
}

// -----------------------------------------------------------
UIDownloadItemWidget::UIDownloadItemWidget(QWidget* parent)
    :CustomWidget(parent)
{
    // 设置样式
    g_StyleConfig->setCurrentStyle(this, "DownLoadItemView", true);

    m_pDownLoader = new HttpDownLoader(this);
    QObject::connect(m_pDownLoader, &HttpDownLoader::error, this, &UIDownloadItemWidget::onError);
    QObject::connect(m_pDownLoader, &HttpDownLoader::readyWrite, this, &UIDownloadItemWidget::onReadyWrite);
    QObject::connect(m_pDownLoader, &HttpDownLoader::downFinished, this, &UIDownloadItemWidget::onDownFinished);

    // 初始化UI
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(4);
    mainLayout->setSpacing(0);

    // 添加路径显示
    m_pDestFileNameTag = new QLabel("");
    mainLayout->addWidget(m_pDestFileNameTag);

    // 添加进度条显示
    QWidget* sliderWidget = new QWidget();
    mainLayout->addWidget(sliderWidget);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderWidget);
    sliderLayout->setMargin(0);
    sliderLayout->setSpacing(2);
    m_pProgressBar = new QProgressBar();
    m_pProgressBar->setMinimum(0);
    m_pProgressBar->setMaximum(100);
    sliderLayout->addWidget(m_pProgressBar);
    // 添加暂停按钮
    m_pPlayButton = new QToolButton;
    m_pPlayButton->setFixedSize(25, 25);
    m_pPlayButton->setIcon(QIcon("./Images/play.png"));
    sliderLayout->addWidget(m_pPlayButton);
    QObject::connect(m_pPlayButton, &QToolButton::clicked, \
                     this, &UIDownloadItemWidget::onClickedStartPauseButton);

    // 添加取消按钮
    QToolButton* cannelButton = new QToolButton;
    cannelButton->setFixedSize(25, 25);
    cannelButton->setIcon(QIcon("./Images/close.png"));
    sliderLayout->addWidget(cannelButton);
    QObject::connect(cannelButton, &QToolButton::clicked, \
                     this, &UIDownloadItemWidget::onClickedCannelButton);

    // 添加文本进度显示按钮
    m_pProgressTag = new QLabel("Loading Url ...");
    mainLayout->addWidget(m_pProgressTag);

    // 设置定时器
    m_timer.setInterval(10);
    QObject::connect(&m_timer, &QTimer::timeout, this, &UIDownloadItemWidget::onTimeout);
}

UIDownloadItemWidget::~UIDownloadItemWidget()
{

}

void UIDownloadItemWidget::setUrl(const QString& url)
{
    m_pDownLoader->setCurrentURL(url);
}

void UIDownloadItemWidget::setDestPathString(const QString& filePath)
{
    m_pDownLoader->setDestPath(filePath);

    // 设置下载绝对路径显示
    QFileInfo file(filePath);
    m_pDestFileNameTag->setText(file.absoluteFilePath());
}

void UIDownloadItemWidget::onError(const QString& errorString)
{
    m_pProgressTag->setText(errorString);
}

void UIDownloadItemWidget::onReadyWrite(void)
{
    int currentSize = 0, totalSize = 0;
    m_pDownLoader->getCurrentDownProgress(currentSize, totalSize);
    QString str = tr("Total Size is ") + QString::number(totalSize) + "B";

    m_pProgressTag->setText(str);
}

void UIDownloadItemWidget::onDownFinished(void)
{
    int currentSize = 0, totalSize = 0;
    m_pDownLoader->getCurrentDownProgress(currentSize, totalSize);
    QString str = tr("Download Finished, Total Size is ") + QString::number(totalSize) + "B";
    m_pProgressTag->setText(str);

    m_pProgressBar->setValue(100);

    // 更改为完成图标
    m_pPlayButton->setIcon(QIcon("./Images/openFile.png"));
}

void UIDownloadItemWidget::onClickedStartPauseButton(void)
{
    QToolButton* toolButton = qobject_cast<QToolButton*>(sender());
    if (m_pDownLoader->getCurrentStatus() == HttpDownLoader::t_Finished)
    {
        // 打开文件所在目录
        QFileInfo info(m_pDestFileNameTag->text());
        QDesktopServices::openUrl(QUrl(info.absolutePath()));
    }
    else if (m_pDownLoader->getCurrentStatus() != HttpDownLoader::t_Downloading)
    {
        // 下载
        toolButton->setIcon(QIcon("./Images/pause.png"));
        m_pDownLoader->start();
        m_timer.start();
    }
    else
    {
        // 暂停
        toolButton->setIcon(QIcon("./Images/play.png"));
        m_pDownLoader->pause();
        m_timer.stop();
    }
}

void UIDownloadItemWidget::onClickedCannelButton(void)
{
    emit clickedDeleteThis();
}

void UIDownloadItemWidget::onTimeout(void)
{
    if (m_pDownLoader->getCurrentStatus() == HttpDownLoader::t_Downloading)
    {
        // 设置进度文本
        int currentSize = 0, totalSize = 0;
        m_pDownLoader->getCurrentDownProgress(currentSize, totalSize);
        QString downloadStr = "%1 / %2";
        downloadStr = downloadStr.arg(currentSize).arg(totalSize);
        m_pProgressTag->setText(downloadStr);

        // 设置进度条
        if (totalSize > 0)
            m_pProgressBar->setValue(currentSize * 1.0 / totalSize * 100);

        m_timer.start();
    }
}
