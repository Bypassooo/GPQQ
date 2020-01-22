#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSound>
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
//#define CLOSEALERT  5

QString Widget::Currentpath = QDir::currentPath();
QString tempint = Widget::Currentpath;
QString tempvio = Widget::Currentpath;
QString temppic = Widget::Currentpath;
QString Widget::iniPath =tempint.append("/config.ini");
QString Widget::VidioPath=tempvio.append("/media/alerm.wav");
QString Widget::PicPath=temppic.append("/pic/logo.png");
QSettings settings_init(Widget::iniPath,QSettings::IniFormat);

//获取刷新时间
int init_a = settings_init.value("/refresh/shwtwb",5).toInt();
int init_b = settings_init.value("/refresh/szwtwb",5).toInt();
int init_c = settings_init.value("/refresh/shwtzb",5).toInt();
int init_d = settings_init.value("/refresh/szwtzb",5).toInt();
int init_e= settings_init.value("/refresh/shwtdb",5).toInt();
int init_f = settings_init.value("/refresh/szwtdb",5).toInt();
int init_g = settings_init.value("/refresh/shbpqr",5).toInt();
int init_h = settings_init.value("/refresh/szbpqr",5).toInt();
int init_i = settings_init.value("/refresh/cdfd3",5).toInt();

QString Widget::morningbegin = settings_init.value("/monitortime/morningbegin","09:30:00").toString();
QString Widget::morningend   = settings_init.value("/monitortime/morningend","11:30:00").toString();
QString Widget::afterbegin   = settings_init.value("/monitortime/afterbegin","13:00:00").toString();
QString Widget::afterend     = settings_init.value("/monitortime/afterend","15:00:00").toString();
int Widget::alertcount   = settings_init.value("/alermtime/count",10).toInt();

int Widget::refresh[9]={init_a,init_b,init_c,init_d,init_e,init_f,init_g,init_h,init_i};

QString Widget::ip = settings_init.value("/database/ip").toString();
QString Widget::port = settings_init.value("/database/port").toString();
QString Widget::instance = settings_init.value("/database/instance").toString();
QString Widget::user = settings_init.value("/database/user").toString();
QString Widget::password = settings_init.value("/database/password").toString();
int Widget::passwordflag = settings_init.value("/database/changepassword").toInt();

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString logpath = Currentpath;
    logpath.append("/log");
    QDir dir(logpath);


    if(!dir.exists())
    {
        dir.mkdir(logpath);//只创建一级子目录，即必须保证上级目录存在
    }

    if(passwordflag  == 1)
        EncryPassWord();
    else
        DecryPassWord();


    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeupdate()));
    timer->start(1000);

    //设置窗口图标
    QIcon icon(PicPath);
    setWindowIcon(icon);

    //给每个窗口设置时钟
    m_pTimer_shkhs      = new QTimer(this);
    m_pTimer_szkhs      = new QTimer(this);
    m_pTimer_shwtzbs    = new QTimer(this);
    m_pTimer_szwtzbs    = new QTimer(this);
    m_pTimer_cxwtzbs    = new QTimer(this);
    m_pTimer_shwtzje    = new QTimer(this);
    m_pTimer_szwtzje    = new QTimer(this);
    m_pTimer_cxwtzje    = new QTimer(this);
    m_pTimer_shwtfz     = new QTimer(this);
    m_pTimer_szwtfz     = new QTimer(this);
    m_pTimer_cxwtfz     = new QTimer(this);
    m_pTimer_shwtfzsjd  = new QTimer(this);
    m_pTimer_szwtfzsjd  = new QTimer(this);
    m_pTimer_cxwtfzsjd  = new QTimer(this);
    m_pTimer_shcykhs    = new QTimer(this);
    m_pTimer_szcykhs    = new QTimer(this);
    m_pTimer_shcjzbs    = new QTimer(this);
    m_pTimer_szcjzbs    = new QTimer(this);
    m_pTimer_cxcjzbs    = new QTimer(this);
    m_pTimer_shcjzje    = new QTimer(this);
    m_pTimer_szcjzje    = new QTimer(this);
    m_pTimer_cxcjzje    = new QTimer(this);
    m_pTimer_shcjfz     = new QTimer(this);
    m_pTimer_szcjfz     = new QTimer(this);
    m_pTimer_cxcjfz     = new QTimer(this);
    m_pTimer_shcjfzsjd  = new QTimer(this);
    m_pTimer_szcjfzsjd  = new QTimer(this);
    m_pTimer_cxcjfzsjd  = new QTimer(this);
    m_pTimer_shwtwb     = new QTimer(this);
    m_pTimer_szwtwb     = new QTimer(this);
    m_pTimer_shwtzb     = new QTimer(this);
    m_pTimer_szwtzb     = new QTimer(this);
    m_pTimer_shwtdb     = new QTimer(this);
    m_pTimer_szwtdb     = new QTimer(this);
    m_pTimer_shbpqr     = new QTimer(this);
    m_pTimer_szbpqr     = new QTimer(this);
    m_pTimer_cdfd3      = new QTimer(this);

    connectgwdatabase();

    connect(m_pTimer_shkhs,     SIGNAL(timeout()), this, SLOT(query_shkhs()));
    connect(m_pTimer_szkhs,     SIGNAL(timeout()), this, SLOT(query_szkhs()));
    connect(m_pTimer_shwtzbs,   SIGNAL(timeout()), this, SLOT(query_shwtzbs()));
    connect(m_pTimer_szwtzbs,   SIGNAL(timeout()), this, SLOT(query_szwtzbs()));
    connect(m_pTimer_cxwtzbs,   SIGNAL(timeout()), this, SLOT(query_cxwtzbs()));
    connect(m_pTimer_shwtzje,   SIGNAL(timeout()), this, SLOT(query_shwtzje()));
    connect(m_pTimer_szwtzje,   SIGNAL(timeout()), this, SLOT(query_szwtzje()));
    connect(m_pTimer_cxwtzje,   SIGNAL(timeout()), this, SLOT(query_cxwtzje()));
    connect(m_pTimer_shwtfz,    SIGNAL(timeout()), this, SLOT(query_shwtfz()));
    connect(m_pTimer_szwtfz,    SIGNAL(timeout()), this, SLOT(query_szwtfz()));
    connect(m_pTimer_cxwtfz,    SIGNAL(timeout()), this, SLOT(query_cxwtfz()));
    connect(m_pTimer_shwtfzsjd, SIGNAL(timeout()), this, SLOT(query_shwtfzsjd()));
    connect(m_pTimer_szwtfzsjd, SIGNAL(timeout()), this, SLOT(query_szwtfzsjd()));
    connect(m_pTimer_cxwtfzsjd, SIGNAL(timeout()), this, SLOT(query_cxwtfzsjd()));
    connect(m_pTimer_shcykhs,   SIGNAL(timeout()), this, SLOT(query_shcykhs()));
    connect(m_pTimer_szcykhs,   SIGNAL(timeout()), this, SLOT(query_szcykhs()));
    connect(m_pTimer_shcjzbs,   SIGNAL(timeout()), this, SLOT(query_shcjzbs()));
    connect(m_pTimer_szcjzbs,   SIGNAL(timeout()), this, SLOT(query_szcjzbs()));
    connect(m_pTimer_cxcjzbs,   SIGNAL(timeout()), this, SLOT(query_cxcjzbs()));
    connect(m_pTimer_shcjzje,   SIGNAL(timeout()), this, SLOT(query_shcjzje()));
    connect(m_pTimer_szcjzje,   SIGNAL(timeout()), this, SLOT(query_szcjzje()));
    connect(m_pTimer_cxcjzje,   SIGNAL(timeout()), this, SLOT(query_cxcjzje()));
    connect(m_pTimer_shcjfz,    SIGNAL(timeout()), this, SLOT(query_shcjfz()));
    connect(m_pTimer_szcjfz,    SIGNAL(timeout()), this, SLOT(query_szcjfz()));
    connect(m_pTimer_cxcjfz,    SIGNAL(timeout()), this, SLOT(query_cxcjfz()));
    connect(m_pTimer_shcjfzsjd, SIGNAL(timeout()), this, SLOT(query_shcjfzsjd()));
    connect(m_pTimer_szcjfzsjd, SIGNAL(timeout()), this, SLOT(query_szcjfzsjd()));
    connect(m_pTimer_cxcjfzsjd, SIGNAL(timeout()), this, SLOT(query_cxcjfzsjd()));
    connect(m_pTimer_shwtwb,    SIGNAL(timeout()), this, SLOT(query_shwtwb()));
    connect(m_pTimer_szwtwb,    SIGNAL(timeout()), this, SLOT(query_szwtwb()));
    connect(m_pTimer_shwtzb,    SIGNAL(timeout()), this, SLOT(query_shwtzb()));
    connect(m_pTimer_szwtzb,    SIGNAL(timeout()), this, SLOT(query_szwtzb()));
    connect(m_pTimer_shwtdb,    SIGNAL(timeout()), this, SLOT(query_shwtdb()));
    connect(m_pTimer_szwtdb,    SIGNAL(timeout()), this, SLOT(query_szwtdb()));
    connect(m_pTimer_shbpqr,    SIGNAL(timeout()), this, SLOT(query_shbpqr()));
    connect(m_pTimer_szbpqr,    SIGNAL(timeout()), this, SLOT(query_szbpqr()));
    connect(m_pTimer_cdfd3,     SIGNAL(timeout()), this, SLOT(query_cdfd3()));

    QSettings settings_refresh(iniPath,QSettings::IniFormat);

    int shkhs       = settings_refresh.value("/refresh/shkhs",5).toInt();
    int szkhs       = settings_refresh.value("/refresh/szkhs",5).toInt();
    int shwtzbs     = settings_refresh.value("/refresh/shwtzbs",5).toInt();
    int szwtzbs     = settings_refresh.value("/refresh/szwtzbs",5).toInt();
    int cxwtzbs     = settings_refresh.value("/refresh/cxwtzbs",5).toInt();
    int shwtzje     = settings_refresh.value("/refresh/shwtzje",5).toInt();
    int szwtzje     = settings_refresh.value("/refresh/szwtzje",5).toInt();
    int cxwtzje     = settings_refresh.value("/refresh/cxwtzje",5).toInt();
    int shwtfz      = settings_refresh.value("/refresh/shwtfz",5).toInt();
    int szwtfz      = settings_refresh.value("/refresh/szwtfz",5).toInt();
    int cxwtfz      = settings_refresh.value("/refresh/cxwtfz",5).toInt();
    int shwtfzsjd   = settings_refresh.value("/refresh/shwtfzsjd",5).toInt();
    int szwtfzsjd   = settings_refresh.value("/refresh/szwtfzsjd",5).toInt();
    int cxwtfzsjd   = settings_refresh.value("/refresh/cxwtfzsjd",5).toInt();
    int shcykhs     = settings_refresh.value("/refresh/shcykhs",5).toInt();
    int szcykhs     = settings_refresh.value("/refresh/szcykhs",5).toInt();
    int shcjzbs     = settings_refresh.value("/refresh/shcjzbs",5).toInt();
    int szcjzbs     = settings_refresh.value("/refresh/szcjzbs",5).toInt();
    int cxcjzbs     = settings_refresh.value("/refresh/cxcjzbs",5).toInt();
    int shcjzje     = settings_refresh.value("/refresh/shcjzje",5).toInt();
    int szcjzje     = settings_refresh.value("/refresh/szcjzje",5).toInt();
    int cxcjzje     = settings_refresh.value("/refresh/cxcjzje",5).toInt();
    int shcjfz      = settings_refresh.value("/refresh/shcjfz",5).toInt();
    int szcjfz      = settings_refresh.value("/refresh/szcjfz",5).toInt();
    int cxcjfz      = settings_refresh.value("/refresh/cxcjfz",5).toInt();
    int shcjfzsjd   = settings_refresh.value("/refresh/shcjfzsjd",5).toInt();
    int szcjfzsjd   = settings_refresh.value("/refresh/szcjfzsjd",5).toInt();
    int cxcjfzsjd   = settings_refresh.value("/refresh/cxcjfzsjd",5).toInt();


    m_pTimer_shkhs->start(shkhs*1000);
    m_pTimer_szkhs->start(szkhs*1000);
    m_pTimer_shwtzbs->start(shwtzbs*1000);
    m_pTimer_szwtzbs->start(szwtzbs*1000);
    m_pTimer_cxwtzbs->start(cxwtzbs*1000);
    m_pTimer_shwtzje->start(shwtzje*1000);
    m_pTimer_szwtzje->start(szwtzje*1000);
    m_pTimer_cxwtzje->start(cxwtzje*1000);
    m_pTimer_shwtfz->start(shwtfz*1000);
    m_pTimer_szwtfz->start(szwtfz*1000);
    m_pTimer_cxwtfz->start(cxwtfz*1000);
    m_pTimer_shwtfzsjd->start(shwtfzsjd*1000);
    m_pTimer_szwtfzsjd->start(szwtfzsjd*1000);
    m_pTimer_cxwtfzsjd->start(cxwtfzsjd*1000);
    m_pTimer_shcykhs->start(shcykhs*1000);
    m_pTimer_szcykhs->start(szcykhs*1000);
    m_pTimer_shcjzbs->start(shcjzbs*1000);
    m_pTimer_szcjzbs->start(szcjzbs*1000);
    m_pTimer_cxcjzbs->start(cxcjzbs*1000);
    m_pTimer_shcjzje->start(shcjzje*1000);
    m_pTimer_szcjzje->start(szcjzje*1000);
    m_pTimer_cxcjzje->start(cxcjzje*1000);
    m_pTimer_shcjfz->start(shcjfz*1000);
    m_pTimer_szcjfz->start(szcjfz*1000);
    m_pTimer_cxcjfz->start(cxcjfz*1000);
    m_pTimer_shcjfzsjd->start(shcjfzsjd*1000);
    m_pTimer_szcjfzsjd->start(szcjfzsjd*1000);
    m_pTimer_cxcjfzsjd->start(cxcjfzsjd*1000);

    m_pTimer_shwtwb->start(refresh[0]*1000);
    m_pTimer_szwtwb->start(refresh[1]*1000);
    m_pTimer_shwtzb->start(refresh[2]*1000);
    m_pTimer_szwtzb->start(refresh[3]*1000);
    m_pTimer_shwtdb->start(refresh[4]*1000);
    m_pTimer_szwtdb->start(refresh[5]*1000);
    m_pTimer_shbpqr->start(refresh[6]*1000);
    m_pTimer_szbpqr->start(refresh[7]*1000);
    m_pTimer_cdfd3->start(refresh[8]*1000);

}
bool Widget::IfAlarm()
{
    QString appendtime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString appendtimea = appendtime;
    QString appendtimeb = appendtime;
    QString appendtimec = appendtime;
    QString appendtimed = appendtime;
    QString Temp_morningbegin = appendtimea.append(" ").append(morningbegin);
    QString Temp_morningend   = appendtimeb.append(" ").append(morningend);
    QString Temp_afterbegin   = appendtimec.append(" ").append(afterbegin);
    QString Temp_afterend     = appendtimed.append(" ").append(afterend);


    QString Temp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QDateTime TempCurrent = QDateTime::fromString(Temp, "yyyy-MM-dd hh:mm:ss");


    QDateTime Set_morningbegin = QDateTime::fromString(Temp_morningbegin, "yyyy-MM-dd hh:mm:ss");
    QDateTime Set_morningend = QDateTime::fromString(Temp_morningend, "yyyy-MM-dd hh:mm:ss");
    QDateTime Set_afterbegin = QDateTime::fromString(Temp_afterbegin, "yyyy-MM-dd hh:mm:ss");
    QDateTime Set_afterend = QDateTime::fromString(Temp_afterend, "yyyy-MM-dd hh:mm:ss");

    uint a = TempCurrent.toTime_t();
    uint b = Set_morningbegin.toTime_t();
    uint c = Set_morningend.toTime_t();
    uint d = Set_afterbegin.toTime_t();
    uint f = Set_afterend.toTime_t();
    int q = a-b;
    int w = a-c;
    int e = a-d;
    int r = a-f;

    if(( q >= 0 && w <=0 ) || ( e >= 0  && r <= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString Widget::XorEncryptDecrypt(const QString &str, const char &key)
{
    QString result;
    QByteArray bs = this->qstringToByte(str);
    int i;
    for(i=0; i<bs.size(); i++)
    {
      bs[i] = bs[i] ^ key;
    }
    result = byteToQString(bs);
    return result;
}
QString Widget::byteToQString(const QByteArray &byte)
{
    QString result;
    if(byte.size() > 0)
    {
      QTextCodec *codec = QTextCodec::codecForName("utf-8");
      result = codec->toUnicode(byte);
    }
    return result;
}
QByteArray Widget::qstringToByte(const QString &strInfo)
{
    QByteArray result;
    if(strInfo.length() > 0)
    {
      QTextCodec *codec = QTextCodec::codecForName("utf-8");
      result = codec->fromUnicode(strInfo);
    }
    return result;
}

void Widget::EncryPassWord()
{
    QSettings settings_setpwd(iniPath,QSettings::IniFormat);
    QString pwd = XorEncryptDecrypt(password,12);
    settings_setpwd.setValue("/database/password",pwd);
    settings_setpwd.setValue("/database/changepassword",0);
}

void Widget::DecryPassWord()
{
    password = XorEncryptDecrypt(password,12);
}

Widget::~Widget()
{
    delete ui;
    gwdb.close();
}

void Widget::timeupdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = "中泰证券股票期权系统       ";
    str.append(time.toString("yyyy-MM-dd hh:mm:ss dddd"));
    int currenthour = QTime::currentTime().hour();
    int currentminute = QTime::currentTime().minute();
    QString marketstatus;
    if(currenthour < 9 ||
      (currenthour == 9 && currentminute < 30) ||
       currenthour >= 15)
    {
        marketstatus = "        闭市";
    }
    else if((currenthour == 9 && currentminute > 29) ||
            (currenthour > 9 && currenthour < 11) ||
            (currenthour == 11 && currentminute < 30) ||
            (currenthour >=13 && currenthour <15))
    {
        marketstatus = "        开市";
    }
    str.append(marketstatus);
    ui->label->setText(str);

}
void Widget::connectgwdatabase()
{
    ip = ip.append(",").append(port);
    gwdb=QSqlDatabase::addDatabase("QODBC","basename");
    gwdb.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                   "SERVER=%1;" //服务器名称
                                   "UID=%3;"           //登录名
                                   "PWD=%4;"        //密码
                                   ).arg(ip)
                           .arg(user)
                           .arg(password)
                           );
    if (!gwdb.open())
    {
        qDebug()<<gwdb.lastError().databaseText();
    }
    else
    {
        qDebug()<<"success";
    }
}
//上海开户数
void Widget::query_shkhs()
{      
    QSqlQuery query(gwdb);
    QString sql="select  count(1) from kbssoptsett..OPT_TRDACCT  WHERE STKBD='15' AND OP_FLAG IN  ('0','1')";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_5->setDigitCount(17);
       ui->lcdNumber_5->display(a0);
    }
    else
    {
        ui->lcdNumber_5->setDigitCount(17);
        ui->lcdNumber_5->display(0);
    }
}
//深圳开户数
void Widget::query_szkhs()
{
    QSqlQuery query(gwdb);
    QString sql="select   count(1) from kbssoptsett..OPT_TRDACCT  WHERE STKBD='05'  AND OP_FLAG IN  ('0','1')";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_13->setDigitCount(17);
       ui->lcdNumber_13->display(a0);
    }
    else
    {
        ui->lcdNumber_13->setDigitCount(17);
        ui->lcdNumber_13->display(0);
    }

}
//上海委托总笔数
void Widget::query_shwtzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='15'";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber->setDigitCount(17);
       ui->lcdNumber->display(a0);
    }
    else
    {
        ui->lcdNumber->setDigitCount(17);
        ui->lcdNumber->display(0);
    }
}
//深圳委托总笔数
void Widget::query_szwtzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='05'";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_3->setDigitCount(17);
       ui->lcdNumber_3->display(a0);
    }
    else
    {
        ui->lcdNumber_3->setDigitCount(17);
        ui->lcdNumber_3->display(0);
    }
}
//次席委托总笔数
void Widget::query_cxwtzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds40..OPT_ORDER";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_30->setDigitCount(17);
       ui->lcdNumber_30->display(a0);
    }
    else
    {
        ui->lcdNumber_30->setDigitCount(17);
        ui->lcdNumber_30->display(0);
    }
}
//上海委托总金额
void Widget::query_shwtzje()
{
    QSqlQuery query(gwdb);
    QString sql="select  SUM(CONVERT(decimal(18,3),ORDER_PRICE)/1000*10000*ORDER_QTY) from   opt_stds..OPT_ORDER     WHERE STKBD='15'";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_2->setDigitCount(17);
       ui->lcdNumber_2->display(a0);
    }
    else
    {
        ui->lcdNumber_2->setDigitCount(17);
        ui->lcdNumber_2->display(0);
    }
}
//深圳委托总金额
void Widget::query_szwtzje()
{
    QSqlQuery query(gwdb);
    QString sql="select  SUM(CONVERT(decimal(18,3),ORDER_PRICE)/1000*10000*ORDER_QTY) from   opt_stds..OPT_ORDER     WHERE STKBD='05'";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_4->setDigitCount(17);
       ui->lcdNumber_4->display(a0);
    }
    else
    {
        ui->lcdNumber_4->setDigitCount(17);
        ui->lcdNumber_4->display(0);
    }
}
//深圳委托总金额
void Widget::query_cxwtzje()
{
    QSqlQuery query(gwdb);
    QString sql="select SUM(CONVERT(decimal(18,3),ORDER_PRICE)/1000*10000*ORDER_QTY) from   opt_stds40..OPT_ORDER";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_31->setDigitCount(17);
       ui->lcdNumber_31->display(a0);
    }
    else
    {
        ui->lcdNumber_31->setDigitCount(17);
        ui->lcdNumber_31->display(0);
    }
}
//上海委托峰值
void Widget::query_shwtfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds..OPT_ORDER"
            " WHERE STKBD='15'"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_6->setDigitCount(17);
       ui->lcdNumber_6->display(a0);
    }
    else
    {
        ui->lcdNumber_6->setDigitCount(17);
        ui->lcdNumber_6->display(0);
    }
}
//深圳委托峰值
void Widget::query_szwtfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds..OPT_ORDER"
            " WHERE STKBD='05'"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_14->setDigitCount(17);
       ui->lcdNumber_14->display(a0);
    }
    else
    {
        ui->lcdNumber_14->setDigitCount(17);
        ui->lcdNumber_14->display(0);
    }
}
//次席委托峰值
void Widget::query_cxwtfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  COUNT(1) from opt_stds40..OPT_ORDER"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_32->setDigitCount(17);
       ui->lcdNumber_32->display(a0);
    }
    else
    {
        ui->lcdNumber_32->setDigitCount(17);
        ui->lcdNumber_32->display(0);
    }
}
//上海委托峰值时间点
void Widget::query_shwtfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19) from opt_stds..OPT_ORDER"
            " WHERE STKBD='15'"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(11,2);
       QString max_minute   =   max_time.mid(14,2);
       QString max_second   =   max_time.mid(17,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_7->setDigitCount(17);
       ui->lcdNumber_7->display(showtime);
    }
    else
    {
        ui->lcdNumber_7->setDigitCount(17);
        ui->lcdNumber_7->display("00:00:00");
    }
}
//深圳委托峰值时间点
void Widget::query_szwtfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19) from opt_stds..OPT_ORDER"
            " WHERE STKBD='05'"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(11,2);
       QString max_minute   =   max_time.mid(14,2);
       QString max_second   =   max_time.mid(17,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_9->setDigitCount(17);
       ui->lcdNumber_9->display(showtime);
    }
    else
    {
        ui->lcdNumber_9->setDigitCount(17);
        ui->lcdNumber_9->display("00:00:00");
    }
}
//次席委托峰值时间点
void Widget::query_cxwtfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19) from opt_stds40..OPT_ORDER"
            " group by  LEFT(RTRIM(convert(varchar(100),ORDER_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(11,2);
       QString max_minute   =   max_time.mid(14,2);
       QString max_second   =   max_time.mid(17,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_33->setDigitCount(17);
       ui->lcdNumber_33->display(showtime);
    }
    else
    {
        ui->lcdNumber_33->setDigitCount(17);
        ui->lcdNumber_33->display("00:00:00");
    }
}
//上海参与客户数
void Widget::query_shcykhs()
{
    QSqlQuery query(gwdb);
    QString sql="SELECT COUNT(1) FROM  ( select  CUST_CODE from  opt_stds..OPT_ORDER  where STKBD='15'  GROUP BY CUST_CODE ) a";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_8->setDigitCount(17);
       ui->lcdNumber_8->display(a0);
    }
    else
    {
        ui->lcdNumber_8->setDigitCount(17);
        ui->lcdNumber_8->display(0);
    }
}
//深圳参与客户数
void Widget::query_szcykhs()
{
    QSqlQuery query(gwdb);
    QString sql="SELECT COUNT(1) FROM  ( select  CUST_CODE from  opt_stds..OPT_ORDER  where STKBD='05'  GROUP BY CUST_CODE ) a";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_10->setDigitCount(17);
       ui->lcdNumber_10->display(a0);
    }
    else
    {
        ui->lcdNumber_10->setDigitCount(17);
        ui->lcdNumber_10->display(0);
    }
}
//上海成交总笔数
void Widget::query_shcjzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  count(1) from opt_stds..OPT_MATCHING   where STKBD='15'";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_11->setDigitCount(17);
       ui->lcdNumber_11->display(a0);
    }
    else
    {
        ui->lcdNumber_11->setDigitCount(17);
        ui->lcdNumber_11->display(0);
    }
}
//深圳成交总笔数
void Widget::query_szcjzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  count(1) from opt_stds..OPT_MATCHING   where STKBD='05'";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_12->setDigitCount(17);
       ui->lcdNumber_12->display(a0);
    }
    else
    {
        ui->lcdNumber_12->setDigitCount(17);
        ui->lcdNumber_12->display(0);
    }
}
//次席成交总笔数
void Widget::query_cxcjzbs()
{
    QSqlQuery query(gwdb);
    QString sql="select  count(1) from opt_stds40..OPT_MATCHING";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(0).toInt();
       ui->lcdNumber_34->setDigitCount(17);
       ui->lcdNumber_34->display(a0);
    }
    else
    {
        ui->lcdNumber_34->setDigitCount(17);
        ui->lcdNumber_34->display(0);
    }
}
//上海成交总金额
void Widget::query_shcjzje()
{
    QSqlQuery query(gwdb);
    QString sql="select SUM(CONVERT(decimal(18,3),MATCHED_AMT)/1000) from opt_stds..OPT_MATCHING   where STKBD='15'";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_15->setDigitCount(17);
       ui->lcdNumber_15->display(a0);
    }
    else
    {
        ui->lcdNumber_15->setDigitCount(17);
        ui->lcdNumber_15->display(0);
    }
}
//深圳成交总金额
void Widget::query_szcjzje()
{
    QSqlQuery query(gwdb);
    QString sql="select SUM(CONVERT(decimal(18,3),MATCHED_AMT)/1000) from opt_stds..OPT_MATCHING   where STKBD='05'";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_16->setDigitCount(17);
       ui->lcdNumber_16->display(a0);
    }
    else
    {
        ui->lcdNumber_16->setDigitCount(17);
        ui->lcdNumber_16->display(0);
    }
}
//次席成交总金额
void Widget::query_cxcjzje()
{
    QSqlQuery query(gwdb);
    QString sql="select SUM(MATCHED_AMT)/1000 from opt_stds40..OPT_MATCHING";
    query.exec(sql);
    if (query.next())
    {
       double a0 = query.value(0).toDouble();
       ui->lcdNumber_35->setDigitCount(17);
       ui->lcdNumber_35->display(a0);
    }
    else
    {
        ui->lcdNumber_35->setDigitCount(17);
        ui->lcdNumber_35->display(0);
    }
}
//上海成交峰值
void Widget::query_shcjfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19),COUNT(1) from opt_stds..OPT_MATCHING"
            " where STKBD='15'"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(1).toInt();
       ui->lcdNumber_17->setDigitCount(17);
       ui->lcdNumber_17->display(a0);
    }
    else
    {
        ui->lcdNumber_17->setDigitCount(17);
        ui->lcdNumber_17->display(0);
    }
}
//深圳成交峰值
void Widget::query_szcjfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19),COUNT(1) from opt_stds..OPT_MATCHING"
            " where STKBD='05'"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(1).toInt();
       ui->lcdNumber_18->setDigitCount(17);
       ui->lcdNumber_18->display(a0);
    }
    else
    {
        ui->lcdNumber_18->setDigitCount(17);
        ui->lcdNumber_18->display(0);
    }

}
//次席成交峰值
void Widget::query_cxcjfz()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19),COUNT(1) from opt_stds40..OPT_MATCHING"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       int a0 = query.value(1).toInt();
       ui->lcdNumber_36->setDigitCount(17);
       ui->lcdNumber_36->display(a0);
    }
    else
    {
        ui->lcdNumber_36->setDigitCount(17);
        ui->lcdNumber_36->display(0);
    }

}
//上海成交峰值时间点
void Widget::query_shcjfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19) from opt_stds..OPT_MATCHING"
            " where STKBD='15'"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(0,2);
       QString max_minute   =   max_time.mid(2,2);
       QString max_second   =   max_time.mid(4,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_28->setDigitCount(17);
       ui->lcdNumber_28->display(showtime);
    }
    else
    {
        ui->lcdNumber_28->setDigitCount(17);
        ui->lcdNumber_28->display("00:00:00");
    }
}
//深圳成交峰值时间点
void Widget::query_szcjfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19) from opt_stds..OPT_MATCHING"
            " where STKBD='05'"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {  
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(0,2);
       QString max_minute   =   max_time.mid(2,2);
       QString max_second   =   max_time.mid(4,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_29->setDigitCount(17);
       ui->lcdNumber_29->display(showtime);
    }
    else
    {
        ui->lcdNumber_29->setDigitCount(17);
        ui->lcdNumber_29->display("00:00:00");
    }
}
//次席成交峰值时间点
void Widget::query_cxcjfzsjd()
{
    QSqlQuery query(gwdb);
    QString sql="select  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19) from opt_stds40..OPT_MATCHING"
            " group by  LEFT(RTRIM(convert(varchar(100),MATCHED_TIME,21)),19)"
            " ORDER BY COUNT(1) DESC";
    query.exec(sql);
    if (query.next())
    {
       QString max_time     =   query.value(0).toString();
       QString max_hour     =   max_time.mid(0,2);
       QString max_minute   =   max_time.mid(2,2);
       QString max_second   =   max_time.mid(4,2);
       QString showtime     =   max_hour.append(":").append(max_minute).append(":").append(max_second);
       ui->lcdNumber_37->setDigitCount(17);
       ui->lcdNumber_37->display(showtime);
    }
    else
    {
        ui->lcdNumber_37->setDigitCount(17);
        ui->lcdNumber_37->display("00:00:00");
    }
}
//上海委托未报
void Widget::query_shwtwb()
{
    QString shwtwblog = Currentpath;
    shwtwblog.append("/log/");
    shwtwblog.append(QDate::currentDate().toString("yyyyMMdd"));
    shwtwblog.append("shwtwb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_shwtwb_Count = 0;
    static int upThreshold_shwtwb_Count = 0;
    static int closealert_shwtwb = 0;
    static int closealert_shwtwb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/shwtwb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/shwtwb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/shwtwb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '0'";
    query.exec(sql_orderrec);
    QFile file(shwtwblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
        // 如果阈值设置为1  则>=1的时候就告警
       //大于0且小于阈值时计数
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_shwtwb_Count++;
       }
       else
       {
           lowThreshold_shwtwb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_shwtwb_Count++;
       }
       else
       {
           upThreshold_shwtwb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_shwtwb_Count >0)
           {
               if(closealert_shwtwb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtwb++;
               }
               logmessage = "触发阈值立刻报警，当前上海委托未报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_19->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtwb_Count >= lowlengtime/refresh[0])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '0'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtwb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtwb_low++;
                      }
                      logmessage = "至少有一笔上海委托未报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_19->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_19->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtwb_low =0;
                  }
               }
           }
           else
           {
               closealert_shwtwb = 0;
               ui->lcdNumber_19->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_shwtwb_Count >= uplengtime/refresh[0])
           {
               if(closealert_shwtwb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtwb++;
               }
               logmessage = "上海委托未报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_19->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtwb_Count >= lowlengtime/refresh[0])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '0'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtwb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtwb_low++;
                      }
                      logmessage = "至少有一笔上海委托未报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_19->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_19->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtwb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_19->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_shwtwb = 0;
               closealert_shwtwb_low = 0;
           }
       }
       ui->lcdNumber_19->setDigitCount(17);
       ui->lcdNumber_19->display(order_count);
    }
    else
    {
        ui->lcdNumber_19->setDigitCount(17);
        ui->lcdNumber_19->display(order_count);
    }
    file.flush();
    file.close();
}
//深圳委托未报
void Widget::query_szwtwb()
{
    QString szwtwblog = Currentpath;
    szwtwblog.append("/log/");
    szwtwblog.append(QDate::currentDate().toString("yyyyMMdd"));
    szwtwblog.append("szwtwb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_szwtwb_Count = 0;
    static int upThreshold_szwtwb_Count = 0;
    static int closealert_szwtwb = 0;
    static int closealert_szwtwb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/szwtwb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/szwtwb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/szwtwb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '0'";
    query.exec(sql_orderrec);
    QFile file(szwtwblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
        // 如果阈值设置为1  则>=1的时候就告警
       //大于0且小于阈值时计数
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_szwtwb_Count++;
       }
       else
       {
           lowThreshold_szwtwb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_szwtwb_Count++;
       }
       else
       {
           upThreshold_szwtwb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_szwtwb_Count >0)
           {
               if(closealert_szwtwb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtwb++;
               }
               logmessage = "触发阈值立刻报警，深圳委托未报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_20->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtwb_Count >= lowlengtime/refresh[1])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '0'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtwb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtwb_low++;
                      }
                      logmessage = "至少有一笔深圳委托未报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_20->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_20->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtwb_low =0;
                  }
               }
           }
           else
           {
               closealert_szwtwb = 0;
               ui->lcdNumber_20->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_szwtwb_Count >= uplengtime/refresh[1])
           {
               if(closealert_szwtwb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtwb++;
               }
               logmessage = "深圳委托未报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_20->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtwb_Count >= lowlengtime/refresh[1])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '0'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtwb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtwb_low++;
                      }
                      logmessage = "至少有一笔深圳委托未报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_20->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_20->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtwb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_20->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_szwtwb = 0;
               closealert_szwtwb_low = 0;
           }
       }
       ui->lcdNumber_20->setDigitCount(17);
       ui->lcdNumber_20->display(order_count);
    }
    else
    {
        ui->lcdNumber_20->setDigitCount(17);
        ui->lcdNumber_20->display(order_count);
    }
    file.flush();
    file.close();
}
//上海委托正报
void Widget::query_shwtzb()
{
    QString shwtzblog = Currentpath;
    shwtzblog.append("/log/");
    shwtzblog.append(QDate::currentDate().toString("yyyyMMdd"));
    shwtzblog.append("shwtzb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_shwtzb_Count = 0;
    static int upThreshold_shwtzb_Count = 0;
    static int closealert_shwtzb = 0;
    static int closealert_shwtzb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/shwtzb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/shwtzb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/shwtzb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '1'";
    query.exec(sql_orderrec);
    QFile file(shwtzblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_shwtzb_Count++;
       }
       else
       {
           lowThreshold_shwtzb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_shwtzb_Count++;
       }
       else
       {
           upThreshold_shwtzb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_shwtzb_Count >0)
           {
               if(closealert_shwtzb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtzb++;
               }
               logmessage = "触发阈值立刻报警，当前上海委托正报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_21->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtzb_Count >= lowlengtime/refresh[2])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '1'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtzb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtzb_low++;
                      }
                      logmessage = "至少有一笔上海委托正报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_21->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_21->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtzb_low =0;
                  }
               }
           }
           else
           {
               closealert_shwtzb = 0;
               ui->lcdNumber_21->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_shwtzb_Count >= uplengtime/refresh[2])
           {
               if(closealert_shwtzb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtzb++;
               }
               logmessage = "上海委托正报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_21->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtzb_Count >= lowlengtime/refresh[2])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = '1'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtzb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtzb_low++;
                      }
                      logmessage = "至少有一笔上海委托正报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_21->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_21->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtzb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_21->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_shwtzb = 0;
               closealert_shwtzb_low = 0;
           }
       }
       ui->lcdNumber_21->setDigitCount(17);
       ui->lcdNumber_21->display(order_count);
    }
    else
    {
        ui->lcdNumber_21->setDigitCount(17);
        ui->lcdNumber_21->display(order_count);
    }
    file.flush();
    file.close();
}

//深圳委托正报
void Widget::query_szwtzb()
{
    QString szwtzblog = Currentpath;
    szwtzblog.append("/log/");
    szwtzblog.append(QDate::currentDate().toString("yyyyMMdd"));
    szwtzblog.append("szwtwb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_szwtzb_Count = 0;
    static int upThreshold_szwtzb_Count = 0;
    static int closealert_szwtzb = 0;
    static int closealert_szwtzb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/szwtzb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/szwtzb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/szwtzb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '1'";
    query.exec(sql_orderrec);
    QFile file(szwtzblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_szwtzb_Count++;
       }
       else
       {
           lowThreshold_szwtzb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_szwtzb_Count++;
       }
       else
       {
           upThreshold_szwtzb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_szwtzb_Count >0)
           {
               if(closealert_szwtzb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtzb++;
               }
               logmessage = "触发阈值立刻报警，当前深圳委托正报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_22->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtzb_Count >= lowlengtime/refresh[3])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '1'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtzb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtzb_low++;
                      }
                      logmessage = "至少有一笔深圳委托正报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_22->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_22->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtzb_low =0;
                  }
               }
           }
           else
           {
               closealert_szwtzb = 0;
               ui->lcdNumber_22->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_szwtzb_Count >= uplengtime/refresh[3])
           {
               if(closealert_szwtzb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtzb++;
               }
               logmessage = "深圳委托正报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_22->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtzb_Count >= lowlengtime/refresh[3])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = '1'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtzb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtzb_low++;
                      }
                      logmessage = "至少有一笔深圳委托正报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_22->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_22->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtzb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_22->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_szwtzb = 0;
               closealert_szwtzb_low = 0;
           }
       }
       ui->lcdNumber_22->setDigitCount(17);
       ui->lcdNumber_22->display(order_count);
    }
    else
    {
        ui->lcdNumber_22->setDigitCount(17);
        ui->lcdNumber_22->display(order_count);
    }
    file.flush();
    file.close();
}
//上海委托待报
void Widget::query_shwtdb()
{
    QString shwtdblog = Currentpath;
    shwtdblog.append("/log/");
    shwtdblog.append(QDate::currentDate().toString("yyyyMMdd"));
    shwtdblog.append("shwtdb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_shwtdb_Count = 0;
    static int upThreshold_shwtdb_Count = 0;
    static int closealert_shwtdb = 0;
    static int closealert_shwtdb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/shwtdb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/shwtdb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/shwtdb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'A'";
    query.exec(sql_orderrec);
    QFile file(shwtdblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_shwtdb_Count++;
       }
       else
       {
           lowThreshold_shwtdb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_shwtdb_Count++;
       }
       else
       {
           upThreshold_shwtdb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_shwtdb_Count >0)
           {
               if(closealert_shwtdb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtdb++;
               }
               logmessage = "触发阈值立刻报警，当前上海委托待报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_23->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtdb_Count >= lowlengtime/refresh[4])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'A'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtdb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtdb_low++;
                      }
                      logmessage = "至少有一笔上海委托待报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_23->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_23->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtdb_low =0;
                  }
               }
           }
           else
           {
               closealert_shwtdb = 0;
               ui->lcdNumber_23->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_shwtdb_Count >= uplengtime/refresh[4])
           {
               if(closealert_shwtdb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shwtdb++;
               }
               logmessage = "上海委托待报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_23->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shwtdb_Count >= lowlengtime/refresh[4])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'A'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shwtdb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shwtdb_low++;
                      }
                      logmessage = "至少有一笔上海委托待报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_23->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_23->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shwtdb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_23->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_shwtdb = 0;
               closealert_shwtdb_low = 0;
           }
       }
       ui->lcdNumber_23->setDigitCount(17);
       ui->lcdNumber_23->display(order_count);
    }
    else
    {
        ui->lcdNumber_23->setDigitCount(17);
        ui->lcdNumber_23->display(0);
    }
    file.flush();
    file.close();
}
//深圳委托待报
void Widget::query_szwtdb()
{
    QString szwtdblog = Currentpath;
    szwtdblog.append("/log/");
    szwtdblog.append(QDate::currentDate().toString("yyyyMMdd"));
    szwtdblog.append("szwtdb.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_szwtdb_Count = 0;
    static int upThreshold_szwtdb_Count = 0;
    static int closealert_szwtdb = 0;
    static int closealert_szwtdb_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/szwtdb",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/szwtdb",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/szwtdb",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'A'";
    query.exec(sql_orderrec);
    QFile file(szwtdblog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_szwtdb_Count++;
       }
       else
       {
           lowThreshold_szwtdb_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_szwtdb_Count++;
       }
       else
       {
           upThreshold_szwtdb_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_szwtdb_Count >0)
           {
               if(closealert_szwtdb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtdb++;
               }
               logmessage = "触发阈值立刻报警，当前深圳委托待报笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_24->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtdb_Count >= lowlengtime/refresh[5])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'A'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtdb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtdb_low++;
                      }
                      logmessage = "至少有一笔深圳委托待报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_24->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_24->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtdb_low =0;
                  }
               }
           }
           else
           {
               closealert_szwtdb = 0;
               ui->lcdNumber_24->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_szwtdb_Count >= uplengtime/refresh[5])
           {
               if(closealert_szwtdb < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szwtdb++;
               }
               logmessage = "深圳委托待报笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_24->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szwtdb_Count >= lowlengtime/refresh[5])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'A'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szwtdb_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szwtdb_low++;
                      }
                      logmessage = "至少有一笔深圳委托待报数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_24->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_24->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szwtdb_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_24->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_szwtdb = 0;
               closealert_szwtdb_low = 0;
           }
       }
       ui->lcdNumber_24->setDigitCount(17);
       ui->lcdNumber_24->display(order_count);
    }
    else
    {
        ui->lcdNumber_24->setDigitCount(17);
        ui->lcdNumber_24->display(order_count);
    }
    file.flush();
    file.close();
}
//上海委托报盘确认
void Widget::query_shbpqr()
{
    QString shbpqrlog = Currentpath;
    shbpqrlog.append("/log/");
    shbpqrlog.append(QDate::currentDate().toString("yyyyMMdd"));
    shbpqrlog.append("shbpqr.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_shbpqr_Count = 0;
    static int upThreshold_shbpqr_Count = 0;
    static int closealert_shbpqr = 0;
    static int closealert_shbpqr_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/shbpqr",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/shbpqr",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/shbpqr",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'B'";
    query.exec(sql_orderrec);
    QFile file(shbpqrlog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_shbpqr_Count++;
       }
       else
       {
           lowThreshold_shbpqr_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_shbpqr_Count++;
       }
       else
       {
           upThreshold_shbpqr_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_shbpqr_Count >0)
           {
               if(closealert_shbpqr < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shbpqr++;
               }
               logmessage = "触发阈值立刻报警，当前上海报盘确认笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_25->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shbpqr_Count >= lowlengtime/refresh[6])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'B'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shbpqr_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shbpqr_low++;
                      }
                      logmessage = "至少有一笔上海报盘确认数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_25->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_25->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shbpqr_low =0;
                  }
               }
           }
           else
           {
               closealert_shbpqr = 0;
               ui->lcdNumber_25->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_shbpqr_Count >= uplengtime/refresh[6])
           {
               if(closealert_shbpqr < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_shbpqr++;
               }
               logmessage = "上海报盘确认笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_25->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_shbpqr_Count >= lowlengtime/refresh[6])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='15'  and ORDER_STATUS = 'B'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_shbpqr_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_shbpqr_low++;
                      }
                      logmessage = "至少有一笔上海报盘确认数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_25->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_25->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_shbpqr_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_25->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_shbpqr = 0;
               closealert_shbpqr_low = 0;
           }
       }
       ui->lcdNumber_25->setDigitCount(17);
       ui->lcdNumber_25->display(order_count);
    }
    else
    {
        ui->lcdNumber_25->setDigitCount(17);
        ui->lcdNumber_25->display(order_count);
    }
    file.flush();
    file.close();
}
//深圳委托报盘确认
void Widget::query_szbpqr()
{
    QString szbpqrlog = Currentpath;
    szbpqrlog.append("/log/");
    szbpqrlog.append(QDate::currentDate().toString("yyyyMMdd"));
    szbpqrlog.append("szbpqr.txt");
    //达不到阈值时如果长期存在大于1笔的单，则告警
    static int lowThreshold_szbpqr_Count = 0;
    static int upThreshold_szbpqr_Count = 0;
    static int closealert_szbpqr = 0;
    static int closealert_szbpqr_low = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/szbpqr",2).toInt();
    //有至少一笔存在时间超过多久就告警
    int lowlengtime = settings.value("/lowlengtime/szbpqr",60).toInt();
    //高于阈值持续多久告警
    int uplengtime = settings.value("/uplengtime/szbpqr",30).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  COUNT(1) from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'B'";
    query.exec(sql_orderrec);
    QFile file(szbpqrlog);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       if( order_count >0 && order_count < threshold )
       {
           lowThreshold_szbpqr_Count++;
       }
       else
       {
           lowThreshold_szbpqr_Count = 0;
       }
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           upThreshold_szbpqr_Count++;
       }
       else
       {
           upThreshold_szbpqr_Count = 0;
       }
       if(uplengtime == -1)//触发阈值直接告警
       {
           if(upThreshold_szbpqr_Count >0)
           {
               if(closealert_szbpqr < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szbpqr++;
               }
               logmessage = "触发阈值立刻报警，当前深圳报盘确认笔数为:";
               logmessage.append(QString::number(order_count));
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_26->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szbpqr_Count >= lowlengtime/refresh[7])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'B'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szbpqr_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szbpqr_low++;
                      }
                      logmessage = "至少有一笔深圳报盘确认数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_26->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_26->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szbpqr_low =0;
                  }
               }
           }
           else
           {
               closealert_szbpqr = 0;
               ui->lcdNumber_26->setStyleSheet("background-color:#576d88;color: #50fdff;");
           }
       }
       else //触发阈值持续一段时间后告警
       {
           if(upThreshold_szbpqr_Count >= uplengtime/refresh[7])
           {
               if(closealert_szbpqr < alertcount)
               {
                   if(IfAlarm())
                   {
                       QSound::play(VidioPath);
                   }
                   closealert_szbpqr++;
               }
               logmessage = "深圳报盘确认笔数达到";
               logmessage.append(QString::number(threshold));
               logmessage.append("笔已经持续了");
               logmessage.append(QString::number(uplengtime));
               logmessage.append("秒");
               text_stream << logmessage << "\r\n";
               ui->lcdNumber_26->setStyleSheet("background-color:red;color: #50fdff;");
           }
           else if(lowThreshold_szbpqr_Count >= lowlengtime/refresh[7])
           {
               QString sql_query=QString(" select count(*) as flag from opt_stds..OPT_ORDER  where STKBD='05'  and ORDER_STATUS = 'B'"
                                         " and (Sysdate - To_Date(ORDER_TIME, 'yyyymmddhh24miss'))* 24 * 60 * 60 > %1").arg(lowlengtime);
               query.exec(sql_query);
               if (query.next())
               {
                  int order_count = query.value(0).toInt();
                  if(order_count>0)
                  {
                      if(closealert_szbpqr_low < alertcount)
                      {
                          if(IfAlarm())
                          {
                              QSound::play(VidioPath);
                          }
                          closealert_szbpqr_low++;
                      }
                      logmessage = "至少有一笔深圳报盘确认数据已持续";
                      logmessage.append(QString::number(lowlengtime));
                      logmessage.append("秒");
                      text_stream << logmessage << "\r\n";
                      ui->lcdNumber_26->setStyleSheet("background-color:orange;color: #50fdff;");
                  }
                  else
                  {
                      ui->lcdNumber_26->setStyleSheet("background-color:#576d88;color: #50fdff;");
                      closealert_szbpqr_low =0;
                  }
               }
           }
           else
           {
               ui->lcdNumber_26->setStyleSheet("background-color:#576d88;color: #50fdff;");
               closealert_szbpqr = 0;
               closealert_szbpqr_low = 0;
           }
       }
       ui->lcdNumber_26->setDigitCount(17);
       ui->lcdNumber_26->display(order_count);
    }
    else
    {
        ui->lcdNumber_26->setDigitCount(17);
        ui->lcdNumber_26->display(order_count);
    }
    file.flush();
    file.close();
}
//撤单废单3次
void Widget::query_cdfd3()
{
    QString cdfd3log = Currentpath;
    cdfd3log.append("/log/");
    cdfd3log.append(QDate::currentDate().toString("yyyyMMdd"));
    cdfd3log.append("cdfd3.txt");
    static int closealert_cdfd3 = 0;
    int order_count = 0;
    //读取配置文件获得配置
    QSettings settings(iniPath,QSettings::IniFormat);
    //上海委托未报阈值
    int threshold = settings.value("/threshold/cdfd3",2).toInt();
    QSqlQuery query(gwdb);
    QString sql_orderrec="select  count(1) from  opt_stds..OPT_ORDER A, "
                         " (select  RAW_ORDER_ID from  opt_stds..OPT_ORDER  where IS_WITHDRAW='T'and ORDER_STATUS='9' group by RAW_ORDER_ID  having count(1)>3)B "
                         " WHERE A.ORDER_ID=B.RAW_ORDER_ID"
                         " group by A.CUST_CODE";
    query.exec(sql_orderrec);
    QFile file(cdfd3log);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    QString logmessage;
    if (query.next())
    {
       order_count = query.value(0).toInt();
       //大于等于阈值时计数
       if(order_count >=threshold)
       {
           if(closealert_cdfd3 < alertcount)
           {
               if(IfAlarm())
               {
                   QSound::play(VidioPath);
               }
               closealert_cdfd3++;
           }
           logmessage = "触发阈值立刻报警，当前撤单三次废单的客户有:";
           logmessage.append(QString::number(order_count));
           text_stream << logmessage << "\r\n";
           ui->lcdNumber_27->setStyleSheet("background-color:red;color: #50fdff;");
       }
       else
       {
           closealert_cdfd3 = 0;
           ui->lcdNumber_27->setStyleSheet("background-color:#576d88;color: #50fdff;");
       }
       ui->lcdNumber_27->setDigitCount(17);
       ui->lcdNumber_27->display(order_count);
    }
    else
    {
        ui->lcdNumber_27->setDigitCount(17);
        ui->lcdNumber_27->display(order_count);
        ui->lcdNumber_27->setStyleSheet("background-color:#576d88;color: #50fdff;");
    }
    file.flush();
    file.close();
}
