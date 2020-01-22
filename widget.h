#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QDir>
#include <QTextCodec>
#include <QByteArray>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void EncryPassWord();
    void DecryPassWord();
    QString byteToQString(const QByteArray &byte);
    QByteArray qstringToByte(const QString &strInfo);
    QString XorEncryptDecrypt(const QString &, const char &);
    bool IfAlarm();
private slots:
    void timeupdate();
    void connectgwdatabase();
    void query_shkhs();
    void query_szkhs();
    void query_shwtzbs();
    void query_szwtzbs();
    void query_cxwtzbs();
    void query_shwtzje();
    void query_szwtzje();
    void query_cxwtzje();
    void query_shwtfz();
    void query_szwtfz();
    void query_cxwtfz();
    void query_shwtfzsjd();
    void query_szwtfzsjd();
    void query_cxwtfzsjd();
    void query_shcykhs();
    void query_szcykhs();
    void query_shcjzbs();
    void query_szcjzbs();
    void query_cxcjzbs();
    void query_shcjzje();
    void query_szcjzje();
    void query_cxcjzje();
    void query_shcjfz();
    void query_szcjfz();
    void query_cxcjfz();
    void query_shcjfzsjd();
    void query_szcjfzsjd();
    void query_cxcjfzsjd();
    void query_shwtwb();
    void query_szwtwb();
    void query_shwtzb();
    void query_szwtzb();
    void query_shwtdb();
    void query_szwtdb();
    void query_shbpqr();
    void query_szbpqr();
    void query_cdfd3();


private:
    Ui::Widget *ui;
    QTimer *m_pTimer_shkhs;
    QTimer *m_pTimer_szkhs;
    QTimer *m_pTimer_shwtzbs;
    QTimer *m_pTimer_szwtzbs;
    QTimer *m_pTimer_cxwtzbs;
    QTimer *m_pTimer_shwtzje;
    QTimer *m_pTimer_szwtzje;
    QTimer *m_pTimer_cxwtzje;
    QTimer *m_pTimer_shwtfz;
    QTimer *m_pTimer_szwtfz;
    QTimer *m_pTimer_cxwtfz;
    QTimer *m_pTimer_shwtfzsjd;
    QTimer *m_pTimer_szwtfzsjd;
    QTimer *m_pTimer_cxwtfzsjd;
    QTimer *m_pTimer_shcykhs;
    QTimer *m_pTimer_szcykhs;
    QTimer *m_pTimer_shcjzbs;
    QTimer *m_pTimer_szcjzbs;
    QTimer *m_pTimer_cxcjzbs;
    QTimer *m_pTimer_shcjzje;
    QTimer *m_pTimer_szcjzje;
    QTimer *m_pTimer_cxcjzje;
    QTimer *m_pTimer_shcjfz;
    QTimer *m_pTimer_szcjfz;
    QTimer *m_pTimer_cxcjfz;
    QTimer *m_pTimer_shcjfzsjd;
    QTimer *m_pTimer_szcjfzsjd;
    QTimer *m_pTimer_cxcjfzsjd;

    QTimer *m_pTimer_shwtwb;
    QTimer *m_pTimer_szwtwb;
    QTimer *m_pTimer_shwtzb;
    QTimer *m_pTimer_szwtzb;
    QTimer *m_pTimer_shwtdb;
    QTimer *m_pTimer_szwtdb;
    QTimer *m_pTimer_shbpqr;
    QTimer *m_pTimer_szbpqr;
    QTimer *m_pTimer_cdfd3;

public:
    QSqlDatabase gwdb;
    static int refresh[9];  //刷新频率
    static int alertcount;  //警报响的次数
    static QString iniPath;
    static QString VidioPath;
    static QString PicPath;
    static QString ip;
    static QString port;
    static QString instance;
    static QString user;
    static QString password;
    static QString Currentpath;
    static int passwordflag;
    static QString morningbegin;
    static QString morningend;
    static QString afterbegin;
    static QString afterend;
};

#endif // WIDGET_H
