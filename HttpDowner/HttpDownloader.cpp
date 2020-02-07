#include "HttpDownloader.h"
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QSslSocket>
#include <QFile>
#include <QDebug>

HttpDownLoader::HttpDownLoader(QObject* parent)
    :QObject(parent)
{
    // 创建NetworkManager
    m_pNetworkManager = new QNetworkAccessManager(this);
    connect(m_pNetworkManager, &QNetworkAccessManager::finished, \
            this, &HttpDownLoader::onReplyFinished);

    // 设置代理
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("127.0.0.1");
    proxy.setPort(8888);
    m_pNetworkManager->setProxy(proxy);
}

HttpDownLoader::~HttpDownLoader()
{
    delete m_file;
}

void HttpDownLoader::onReplyFinished(QNetworkReply* reply)
{
    if (m_status == t_Start)
    {
        // 获取文件大小
        QString lengthString = reply->rawHeader("Content-Length");
        if (lengthString.isEmpty())
        {
            emit error(tr("Failed to get file size."));
            return;
        }

        // 设置文件大小
        m_nTotalSize = lengthString.toInt();
        emit readyWrite();
        return;
    }
    else
    {
        // 接收完成
        if (m_nTotalSize <= m_nCurrentDownloadSize)
        {
            m_file->close();
            m_status = t_Finished;
            emit downFinished();
//            qDebug() << "Recv Finished: " << "Current Size is " \
//                     << m_nCurrentDownloadSize << "; Total Size is " \
//                     << m_nTotalSize;
            return;
        }

        // 继续发送
        requestFile();
    }

    reply->deleteLater();
}

void HttpDownLoader::requestFile(void)
{
    if (m_nCurrentDownloadSize >= m_nTotalSize || m_status != t_Downloading)
        return;

    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(m_cUrlString));
    networkRequest.setRawHeader("User-Agent", "QtApp-HttpDownLoader");
    networkRequest.setRawHeader("Accept", "application/octet-stream");

    // 设置获取文件的范围
    QString rangeStr = "bytes=%1-%2";
    int rangeRValue = m_nCurrentDownloadSize + m_nIntervalSize;
    if (rangeRValue > m_nTotalSize)
        rangeRValue = m_nTotalSize;
    rangeStr = rangeStr.arg(m_nCurrentDownloadSize).arg(m_nCurrentDownloadSize + m_nIntervalSize);
    networkRequest.setRawHeader("Range", rangeStr.toUtf8());

    // 发送请求
    QNetworkReply *reply = m_pNetworkManager->get(networkRequest);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), \
              this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void HttpDownLoader::slotReadyRead(void)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply == nullptr)
        return;

    // 获取接收的数据
    QByteArray byteArray = reply->readAll();
    if (byteArray.size() <= 0)
    {
        m_status = t_Error;
        emit error(tr("Fail Recv File Data"));
        return;
    }
    m_nCurrentDownloadSize += byteArray.size();

//    // for Test
//    QString recvRanges = reply->rawHeader("Content-Range");
//    QString recvLength = reply->rawHeader("Content-Length");

//    qDebug() << recvRanges << ", " << recvLength << ", " <<  byteArray.size();
//    qDebug() << "Recving: " << "Current Size is " \
//             << m_nCurrentDownloadSize << "; Total Size is " \
//             << m_nTotalSize;
//    qDebug() << "---------------------------------------------------";

    // 写入文件
    m_file->write(byteArray);
    m_file->flush();


}

void HttpDownLoader::slotError(QNetworkReply::NetworkError errorcode)
{
    QString errorString = tr("Recv Data Error, Error Code is ") + QString::number((int)errorcode);

    emit error(errorString);
}

// 设置URL
void HttpDownLoader::setCurrentURL(const QString& url)
{
    m_cUrlString = url;
    m_nTotalSize = 0;
    m_nCurrentDownloadSize = 0;

    // 创建请求
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(url));
    networkRequest.setRawHeader("User-Agent", "QtApp-HttpDownLoader");
    networkRequest.setRawHeader("Accept", "application/octet-stream");

    // head请求（获取文件大小）
    m_pNetworkManager->head(networkRequest);
}

void HttpDownLoader::setDestPath(const QString& fileName)
{
    if (m_file != nullptr)
        delete m_file;

    m_file = new QFile(fileName);
    m_file->open(QFile::WriteOnly);
}

// 获取当前下载进度
void HttpDownLoader::getCurrentDownProgress(int& currentSize, int& totalSize)
{
    currentSize = m_nCurrentDownloadSize;
    totalSize = m_nTotalSize;
}

// 开始下载
void HttpDownLoader::start(void)
{
    if (m_status != t_Start && m_status != t_Pause)
        return;

    m_status = t_Downloading;
    requestFile();
}

// 暂停下载
void HttpDownLoader::pause(void)
{
    m_status = t_Pause;
}

// 获取当前状态
HttpDownLoader::DownloadStatus HttpDownLoader::getCurrentStatus(void)
{
    return m_status;
}
