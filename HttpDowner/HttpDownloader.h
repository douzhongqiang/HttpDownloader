#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDataStream>
#include <QFile>
class HttpDownLoader : public QObject
{
    Q_OBJECT

public:
    enum DownloadStatus
    {
        t_Start,                // 起始状态
        t_Downloading,          // 下载中
        t_Pause,                // 暂停
        t_Finished,             // 下载完成
        t_Error                 // 错误
    };

public:
    HttpDownLoader(QObject* parent = nullptr);
    ~HttpDownLoader();

    // 设置URL
    void setCurrentURL(const QString& url);
    // 开始下载
    void start(void);
    // 暂停下载
    void pause(void);
    // 设置目标文件
    void setDestPath(const QString& fileName);

    // 获取当前下载进度
    void getCurrentDownProgress(int& currentSize, int& totalSize);

    // 获取当前状态
    DownloadStatus getCurrentStatus(void);

private:
    QNetworkAccessManager* m_pNetworkManager = nullptr;

    // 文件的大小
    int m_nTotalSize = 0;
    // 已经下载了的大小
    int m_nCurrentDownloadSize = 0;
    // 限速下载
    int m_nIntervalSize = 10240;

    // URL
    QString m_cUrlString;
    // 当前状态
    DownloadStatus m_status = t_Start;

    QFile* m_file = nullptr;

    // 发送获取文件请求
    void requestFile(void);

private slots:
    void onReplyFinished(QNetworkReply* reply);
    void slotReadyRead(void);
    void slotError(QNetworkReply::NetworkError);

signals:
    void error(const QString& errorString);
    void readyWrite(void);
    void downFinished(void);
};

#endif
