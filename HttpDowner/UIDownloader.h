#ifndef UIDOWNLOADER_H
#define UIDOWNLOADER_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QToolButton>
#include <QProgressBar>
#include <QTimer>
#include "UICustomWidgtes/CustomWidget.h"
#include "HttpDownloader.h"
class UICustomLineEditControl;
class UIDownloadItemWidget;
class QPushButton;
class QToolButton;
class UIDownloadWidget : public CustomWidget
{
    Q_OBJECT

public:
    UIDownloadWidget(QWidget* parent = nullptr);
    ~UIDownloadWidget();

private:
    UICustomLineEditControl* m_pUrlLineEdit = nullptr;
    QListWidget* m_pListWidget = nullptr;

    QList<QListWidgetItem*> m_itemList;
    QList<UIDownloadItemWidget*> m_itemWidgetList;

    // 添加下载
    void addDownloadItem(const QString& urlStr, const QString& destFilePathName);

private slots:
    void onAddDownUrl(void);
    void onDeleteItem(void);
};

// -----------------------------------------------------------
class UIDownloadItemWidget : public CustomWidget
{
    Q_OBJECT

public:
    UIDownloadItemWidget(QWidget* parent = nullptr);
    ~UIDownloadItemWidget();

    void setUrl(const QString& url);
    void setDestPathString(const QString& filePath);

private:
    HttpDownLoader* m_pDownLoader = nullptr;

    // 显示下载路径
    QLabel* m_pDestFileNameTag = nullptr;
    // 显示进度文本
    QLabel* m_pProgressTag = nullptr;
    // 显示进度
    QProgressBar* m_pProgressBar = nullptr;
    // 播放按钮
    QToolButton* m_pPlayButton = nullptr;

    QTimer m_timer;

private slots:
    void onError(const QString& errorString);
    void onReadyWrite(void);
    void onDownFinished(void);

    void onClickedStartPauseButton(void);
    void onClickedCannelButton(void);
    void onTimeout(void);

signals:
    void clickedDeleteThis(void);
};

// -----------------------------------------------------------

#endif
