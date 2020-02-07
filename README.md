# HttpDownloader
HttpDownloader 是一个简单的Http下载工具，方便所有的Qt爱好者学习和使用。
预览效果如下：

![预览效果](https://www.douzhq.cn/wp-content/uploads/2020/02/httpDoawnLoader_pic.png)

作者：douzhq </br>
个人博客主页：[不会飞的纸飞机](http://www.douzhq.cn) </br>
CSDN博客主页：https://blog.csdn.net/douzhq </br>
微信公众号: 不会飞的纸飞机 ，不时有技术文章分享 </br>
![不会飞的纸飞机](https://www.douzhq.cn/wp-content/uploads/2020/02/微信公众号.jpg)


# 目录说明

1. CustomWidgets 一些通用的窗口，比如自定义Windows窗口等。
2. CustomControls 一些基础空间和组合控件。
3. HttpDowner 是App项目
- HttpDownloader Http下载核心类实现
- UIDownloader 主界面类
- UIConfigDialog 配置界面类

</br>
# 测试链接

音频文件</br>
https://www.douzhq.cn/wp-content/uploads/2020/02/丫蛋蛋-大田後生仔.mp3</br>
https://www.douzhq.cn/wp-content/uploads/2020/02/宝石Gem-野狼disco.mp3</br>
</br>
图片</br>
https://www.douzhq.cn/wp-content/uploads/2020/01/memory_pool_headImage.jpg</br>
https://www.douzhq.cn/wp-content/uploads/2020/02/thread_create.jpg</br>
https://www.douzhq.cn/wp-content/uploads/2020/02/downLoadTestPix-scaled.jpg</br>

</br>
# 说明</br>

我是用的开发环境是 Qt5.13，QtCreator，编译器VS2015 64位</br>
如果您要下载的链接时Https的，则需要在HttpDowner 目录下bin添加如下文件</br>
</br>
如果编译器时VS则添加文件</br>
libcrypto-1_1-x64.dll</br>
libssl-1_1-x64.dll</br>
</br>
如果编译器时mingw则需要</br>
libeay32.dll</br>
ssleay32.dll