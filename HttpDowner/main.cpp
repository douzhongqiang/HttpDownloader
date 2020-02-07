#include "UIConfigDialog.h"
#include <QApplication>
#include "UICustomWidgtes/CustomDialog.h"
#include "UIDownloader.h"
#include "UICustomCore/CustomStyleConfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    g_StyleConfig->init();

    CustomDialog* dialog = new CustomDialog;
    dialog->setWindowTitle(QObject::tr("Http下载软件  作者：douzhq www.douzhq.cn"));
    dialog->setContentWidget(new UIDownloadWidget);
    dialog->resize(800, 600);
    dialog->show();

    return a.exec();
}
