#include "printmgr.h"
#include "exglobal.h"
#include <QCoreApplication>
#include <QPrinter>
#include <QPainter>
#include <QPdfWriter>
#include <QPrinterInfo>
#include <QVector>
#include <QLine>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>
#include "PrinterLibs.h"
#include "log.h"
#include "datahandler.h"

static bool loadlogo = false;
printmgr::printmgr(QObject *parent) : QThread(parent)
{
    printDir = QCoreApplication::applicationDirPath()+"/report";
    QDir dir(printDir);
    if (!dir.exists())
        dir.mkpath(printDir);
    printDir += "/";
    loadlogo = flashdxlogo.load(QCoreApplication::applicationDirPath()+"/Dxlogo.png");
    qDebug()<<"load logo"<<loadlogo;

}

void printmgr::run(){

    QByteArray res;
    res.resize(10);
    res[0] = '\xaa';
    res[1] = '\x04';
    res[2] = '\x01';
    res[7] = '\xB1';
    res[8] = '\x00';

    QString filename = CreatePDF();
    qDebug()<<"printmgr type = "<<ExGlobal::PrintType<<filename;
    if (filename.isEmpty())
    {
        res[8] = '\x01';
        msleep(5000);
    }
    else {
        if (ExGlobal::PrintType == 0){
            printPTP();
            msleep(3000);
        }
        else{
        int result = 0;
        result = ExGlobal::SysCommand(QString("lp -d dxprinter %1").arg(filename));
        qDebug()<<"print result = "<<result;
        msleep(3000);
        }
    }



    //Log::Logdb(LOGTYPE_PRINT,printok?1:0,ExGlobal::User);


    emit finishPrint(res);
}

bool printmgr::printPDF(){
    QPrinter printer;
    QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
    qDebug()<<"print size="<<list.size();
    for (int i = 0; i < list.size(); i++)
        qDebug()<<list.at(i).printerName();    

    //printer.setOrientation(QPrinter::Landscape);
    //printer.setPageSize(QPrinter::Custom);
    //printer.setPageSizeMM(QSize(220,800));
    //printer.setPageSize(QPrinter::A4);
    printer.setPageSize(QPrinter::A5);
    printer.setOrientation(QPrinter::Landscape);
#if 1
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(QCoreApplication::applicationDirPath()+"/../report/"+sampCode.remove(QRegExp("\\s"))+QDateTime::currentDateTime().toString("-yyyyMMdd-hhmmss")+".pdf");
#else
    if (list.size()==0)
    {
        return false;
    }

    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPrinterName(list.at(0).printerName());
#endif
    QPainter *painter = new QPainter();
    painter->begin(&printer);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(QColor(255,255,255),2));
    painter->setBrush(QColor(240,240,240));
    QRect rect(3,0,800,600);
    painter->fillRect(rect,QColor(255,255,255));
    painter->drawRect(rect);

    painter->setPen(QPen(QColor(0,0,0),1));
    QVector<QLine> lines;
    lines.append(QLine(QPoint(50,50),QPoint(750,50)));
    lines.append(QLine(QPoint(750,50),QPoint(750,550)));
    lines.append(QLine(QPoint(50,550),QPoint(750,550)));
    lines.append(QLine(QPoint(50,50),QPoint(50,550)));
    lines.append(QLine(QPoint(50,120),QPoint(750,120)));
    lines.append(QLine(QPoint(50,180),QPoint(750,180)));
    lines.append(QLine(QPoint(50,200),QPoint(750,200)));
    lines.append(QLine(QPoint(50,490),QPoint(750,490)));
    lines.append(QLine(QPoint(120,180),QPoint(120,200)));
    lines.append(QLine(QPoint(220,180),QPoint(220,200)));
    lines.append(QLine(QPoint(300,180),QPoint(300,200)));
    lines.append(QLine(QPoint(400,180),QPoint(400,490)));
    painter->drawLines(lines);

    QFont font;
    font.setPointSize(13);
    font.setFamily("??????");
    //font.setItalic(true);
    painter->setFont(font);

    //first
    painter->drawText(50,50,700,40,Qt::AlignCenter,"??????????????????????????????");
    painter->drawText(QPoint(650,110),"No:");
    font.setPointSize(20);
    painter->setFont(font);
    painter->drawText(50,80,700,40,Qt::AlignCenter,ExGlobal::panelName());
    font.setPointSize(10);
    painter->setFont(font);
    //second
    painter->drawText(60,120,200,60,Qt::AlignVCenter,QString("??????ID??? ")+sampCode);
    painter->drawText(250,120,200,60,Qt::AlignVCenter,QString("??????????????? ")+sampInfo);
    painter->drawText(450,120,200,60,Qt::AlignCenter,QString("????????? ")+ExGlobal::SysName);
    //three
    painter->drawText(50,180,70,20,Qt::AlignCenter,"??????");
    painter->drawText(120,180,100,20,Qt::AlignCenter,"????????????");
    painter->drawText(220,180,80,20,Qt::AlignCenter,"Ct");
    painter->drawText(300,180,100,20,Qt::AlignCenter,"??????");
    //four
#if 0
    painter->drawText(50,210,70,20,Qt::AlignCenter,"1");
    painter->drawText(120,210,100,20,Qt::AlignCenter,"ADV-2");
    //painter->drawText(220,210,80,20,Qt::AlignCenter,"12");
    painter->drawText(300,210,100,20,Qt::AlignCenter,"??????");
#endif
    QMapIterator<QString,int> it(itemMap);
    int itemIndex = 1;
    int xPos = 205;
    while(it.hasNext()){
        it.next();
        if (it.value() != 0)
        {
            double ct = (double)(it.value())/10;
            painter->setPen(QPen(QColor(255,0,0),1));
            painter->drawText(220,xPos,80,20,Qt::AlignCenter,QString("%1").arg(ct));
            painter->drawText(300,xPos,100,20,Qt::AlignCenter,"??????");
        }
        else
            painter->drawText(300,xPos,100,20,Qt::AlignCenter,"??????");

        painter->drawText(50,xPos,70,20,Qt::AlignCenter,QString::number(itemIndex));
        painter->drawText(120,xPos,100,20,Qt::AlignCenter,it.key());
        painter->setPen(QPen(QColor(0,0,0),1));
        itemIndex++;
        xPos += 20;
    }
    //five
    painter->drawText(60,490,700,30,Qt::AlignVCenter,QString("???????????? "));
    painter->drawText(360,490,700,30,Qt::AlignVCenter,QString("???????????? ")+user);
    painter->drawText(590,490,700,30,Qt::AlignVCenter,QString("???????????? "));
    painter->drawText(60,520,700,30,Qt::AlignVCenter,QString("??????????????? ")+testTime);
    painter->drawText(360,520,700,30,Qt::AlignVCenter,QString("??????????????? ")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
/*
    //draw coor
    int co_left = 50;
    int co_top = 700;
    font.setPointSize(6);
    painter->setFont(font);
    painter->drawLine(QLine(QPoint(co_left+50,co_top+100),QPoint(co_left+50+50*5,co_top+100)));
    for (int i = 0; i < 11; i++){
        painter->drawText(co_left+50-5+25*i,co_top+100,10,10,Qt::AlignCenter,QString::number(i*5));
    }
    int prenumber = qrand()%100;
    int currnumber = 0;
    painter->setPen(QPen(QColor(100,100,200),1));
    for (int i = 2; i < 40; i++){
        currnumber = qrand()%100;
        painter->drawLine(QLine(QPoint(co_left+50+(i-1)*5,co_top+100-prenumber),QPoint(co_left+50+i*5,co_top+100-currnumber)));
        prenumber = currnumber;
    }
    */
    painter->end();

    return true;
}

bool printmgr::printPTP(){
    void *handle = Port_OpenUSBIO("/dev/usb/lp0");
    if (handle == nullptr)
    {
        qDebug()<<"Can not open printer";
        return false;
    }
    else
    {
        QString printStr;
        Port_SetPort(handle);
        Pos_Reset();
        Pos_PrintNVLogo(1);
        Pos_FeedLine();
        QString testName = panelName+"\n";
        Pos_Text(testName.toStdString().c_str(),0,-2,1,1,0,0);
        Pos_FeedLine();

        printStr = QString("????????? ")+ExGlobal::SysName+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
        printStr = QString("??????ID??? ")+sampCode+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
        printStr = QString("??????????????? ")+sampInfo+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);

        Pos_FeedLine();

        QMapIterator<QString,int> it(itemMap);
        int itemIndex = 1;
        while(it.hasNext()){
            it.next();
            printStr = QString::number(itemIndex)+" "+it.key();
            int n = printStr.length();
            n = 2*n;
            while (n<26) {
                printStr+=" ";
                n++;
            }
            if (it.value() != 0){
                printStr += "??????\n";
                Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,8);
            }
            else
            {
                printStr += "??????\n";
                Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
            }
            itemIndex++;
        }

        Pos_FeedLine();
        printStr = QString("???????????? ")+user+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
        printStr = QString("???????????? ")+checker+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
        printStr = QString("??????????????? ")+testTime+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
        printStr = QString("??????????????? ")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"\n";
        Pos_Text(printStr.toStdString().c_str(),0,-1,0,0,0,0);
/*
        Pos_FeedLine();
        Pos_Reset();
        Pos_Align(1);
        Pos_Qrcode("https://www.ubaike.cn/show_10459701.html\n",4);

        Pos_FeedLine();
        Pos_Barcode("SLX 123",0x45,0,3,96,0,2);        
*/
        Pos_Feed_N_Line(3);
        Port_ClosePort(handle);
    }
    return true;
}

#define OUTERX 5
#define OUTERY 5
#define OUTERWIDTH 790
#define OUTERHEIGHT 545
#define TITLEWIDTH 400
#define TITLEHEIGHT 25
#define SAMPLECODEX 100
#define SAMPLECODEY 50
#define INNERREFX 20
#define INNERREFY 130
#define ITEMHEADLINEX 20
#define ITEMHEADLINEY INNERREFY+20
#define ITEMHEADLINELENGTH 760
#define ITEMENDLINEY ITEMHEADLINEY+340

#define ITEMHEIGHT 20
#define LINEHEIGHT 18
#define ITEMLINEWIDTH 700
#define ITEMINDEXPOS ITEMHEADLINEX+10
#define ITEMINDEXWIDTH 80
#define ITEMCODEPOS ITEMINDEXPOS+ITEMINDEXWIDTH
#define ITEMCODEWIDTH 120
#define ITEMNAMEPOS ITEMCODEPOS+ITEMCODEWIDTH
#define ITEMNAMEWIDTH 200
#define ITEMRESULTPOS ITEMNAMEPOS+ITEMNAMEWIDTH
#define ITEMRESULTWIDTH 120
#define ITEMREFPOS ITEMRESULTPOS+ITEMRESULTWIDTH
#define ITEMREFWIDTH 120

//const static QRect OuterFrame(OUTERX,OUTERY,OUTERWIDTH,OUTERHEIGHT);
const static QRect TitleRect((OUTERX+OUTERWIDTH-TITLEWIDTH)>>1,OUTERY+10,TITLEWIDTH,TITLEHEIGHT);
const static QRect HospitalRect(OUTERX+10,OUTERY+10,200,TITLEHEIGHT);
//const static QRect dxLogoRect(SAMPLECODEX+570,OUTERY+8,117,28);
const static QRect dxLogoRect(SAMPLECODEX+570,OUTERY+8,94,22);
const static QRect SampleCodeRect(SAMPLECODEX,SAMPLECODEY,300,20);
const static QRect SampleInfoRect(SAMPLECODEX+200,SAMPLECODEY+20,300,20);
const static QRect SampleTypeRect(SAMPLECODEX+200,SAMPLECODEY,300,20);
const static QRect SampleRemarkRect(SAMPLECODEX,SAMPLECODEY+20,300,20);
const static QRect InnerRefRect(INNERREFX,INNERREFY,150,20);
const static QRect PanelNameRect(INNERREFX+150,INNERREFY,150,20);
const static QRect SerialRect(INNERREFX+300,INNERREFY,150,20);
const static QRect ValidDateRect(INNERREFX+450,INNERREFY,150,20);
const static QRect SystemNameDateRect(INNERREFX+600,INNERREFY,160,20);
const static QRect ItemRemarkRect(ITEMHEADLINEX+10,ITEMENDLINEY-20,ITEMHEADLINELENGTH,20);
const static QRect TestTimeRect(ITEMHEADLINEX+10,ITEMENDLINEY+4,200,20);
const static QRect ReportTimeRect(ITEMHEADLINEX+10,ITEMENDLINEY+22,200,20);
const static QRect UserRect(ITEMHEADLINEX+250,ITEMENDLINEY+4,100,20);
const static QRect CheckerRect(ITEMHEADLINEX+500,ITEMENDLINEY+4,100,20);
const static QRect CopyRightRect((OUTERX+OUTERWIDTH-TITLEWIDTH)>>1,OUTERY+OUTERHEIGHT-20,TITLEWIDTH,20);
const static QRect BarCodeRect(SAMPLECODEX+480,SAMPLECODEY,200,70);

bool printmgr::printReport(QPainter &painter){
    QRect ItemRect(ITEMHEADLINEX,ITEMHEADLINEY,ITEMHEADLINELENGTH,LINEHEIGHT);
    QRect ItemIndexRect(ITEMINDEXPOS,ITEMHEADLINEY,ITEMINDEXWIDTH,ITEMHEIGHT);
    QRect ItemCodeRect(ITEMCODEPOS,ITEMHEADLINEY,ITEMCODEWIDTH,ITEMHEIGHT);
    QRect ItemNameRect(ITEMNAMEPOS,ITEMHEADLINEY,ITEMNAMEWIDTH,ITEMHEIGHT);
    QRect ItemResultRect(ITEMRESULTPOS,ITEMHEADLINEY,ITEMRESULTWIDTH,ITEMHEIGHT);
    QRect ItemRefRect(ITEMREFPOS,ITEMHEADLINEY,ITEMREFWIDTH,ITEMHEIGHT);

    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(QColor(0,0,0),1));

    QFont font;
    font.setPointSize(16);
    font.setFamily("??????");
    //font.setItalic(true);
    painter.setFont(font);

    painter.drawText(TitleRect,Qt::AlignCenter,panelName+tr("??????"));
    if (flashdxlogo.load(QCoreApplication::applicationDirPath()+"/Dxlogo.png"))
        painter.drawImage(dxLogoRect,flashdxlogo);
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(HospitalRect,Qt::AlignCenter,ExGlobal::HospitalName);

    font.setPointSize(10);
    painter.setFont(font);
    painter.drawText(SampleCodeRect,Qt::AlignVCenter,tr("???????????????")+sampCode);
    painter.drawText(SampleInfoRect,Qt::AlignVCenter,tr("???????????????")+sampInfo);
    painter.drawText(SampleTypeRect,Qt::AlignVCenter,tr("???????????????")+sampType);
    painter.drawText(SampleRemarkRect,Qt::AlignVCenter,tr("???????????????")+sampRemark);
    font.setPointSize(8);
    painter.setFont(font);
    if (refValid)
        painter.drawText(InnerRefRect,Qt::AlignVCenter,tr("??????????????????")+tr("?????? ???"));
    else
        painter.drawText(InnerRefRect,Qt::AlignVCenter,tr("??????????????????")+tr("?????? x"));
    painter.drawText(PanelNameRect,Qt::AlignVCenter,panelName);
    painter.drawText(SerialRect,Qt::AlignVCenter,tr("?????????")+ExGlobal::boxSerial());
    painter.drawText(ValidDateRect,Qt::AlignVCenter,tr("????????????")+validTime.split(" ")[0]);
    painter.drawText(SystemNameDateRect,Qt::AlignVCenter,tr("?????????")+ExGlobal::SysName);


    painter.drawLine(QLine(QPoint(ITEMHEADLINEX,ITEMHEADLINEY),QPoint(ITEMHEADLINEX+ITEMHEADLINELENGTH,ITEMHEADLINEY)));
    painter.drawLine(QLine(QPoint(ITEMHEADLINEX,ITEMENDLINEY),QPoint(ITEMHEADLINEX+ITEMHEADLINELENGTH,ITEMENDLINEY)));
    painter.drawText(ItemRemarkRect,Qt::AlignLeft,tr("??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????"));
    painter.drawText(TestTimeRect,Qt::AlignLeft,tr("???????????????")+testTime);
    painter.drawText(ReportTimeRect,Qt::AlignLeft,tr("???????????????")+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    painter.drawText(UserRect,Qt::AlignLeft,tr("????????????")+user);
    painter.drawText(CheckerRect,Qt::AlignLeft,tr("????????????")+checker);

    painter.drawText(ItemIndexRect,Qt::AlignCenter,tr("??????"));
    painter.drawText(ItemCodeRect,Qt::AlignVCenter,tr("??????"));
    painter.drawText(ItemNameRect,Qt::AlignVCenter,tr("??????"));
    painter.drawText(ItemResultRect,Qt::AlignCenter,tr("??????"));
    painter.drawText(ItemRefRect,Qt::AlignCenter,tr("?????????"));

    QMapIterator<QString,int> it(itemMap);
    int itemIndex = 1;

    while(it.hasNext()){
        it.next();
        ItemRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+5);
        ItemIndexRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+4);
        ItemCodeRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+4);
        ItemNameRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+4);
        ItemResultRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+4);
        ItemRefRect.moveTop(itemIndex*LINEHEIGHT+ITEMHEADLINEY+4);
        if (itemIndex%2 == 1)
        painter.fillRect(ItemRect,QColor(210,210,210));
        painter.drawText(ItemIndexRect,Qt::AlignCenter,QString::number(itemIndex));
        painter.drawText(ItemCodeRect,Qt::AlignVCenter,it.key());
        painter.drawText(ItemNameRect,Qt::AlignVCenter,ExGlobal::ItemCodeToFullName(it.key()));
        if (it.value() != 0)
            painter.drawText(ItemResultRect,Qt::AlignCenter,tr("??????(+)"));
        else
            painter.drawText(ItemResultRect,Qt::AlignCenter,tr("??????(-)"));
        painter.drawText(ItemRefRect,Qt::AlignCenter,tr("??????(-)"));
        itemIndex++;
    }

    font.setPointSize(6);
    painter.setFont(font);
    painter.drawText(CopyRightRect,Qt::AlignCenter,tr("??????????????????????????????@2020??????????????????????????????")+ExGlobal::version());

    painter.setPen(QPen(QColor(200,200,200),1));
    font.setPointSize(20);
    painter.setFont(font);
    painter.drawRect(BarCodeRect);
    painter.drawText(BarCodeRect,Qt::AlignCenter,tr("?????????????????????"));

    return true;
}

QString printmgr::CreatePDF(){
    QString file = printDir+sampCode.remove(QRegExp("\\s"))+QDateTime::currentDateTime().toString("-yyyyMMdd-hhmmss")+".pdf";
    QPrinter printer;
    printer.setPageSize(QPrinter::A5);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);

    QPainter painter;
    painter.begin(&printer);
    printReport(painter);
    painter.end();
    return file;
}

bool printmgr::printResult(QString dirname){
    QString filename = dirname+"/TestResult.pdf";

    if (QFile::exists(filename))
        QFile::remove(filename);

    QFile pdfFile(filename);
    pdfFile.open(QIODevice::WriteOnly);
    QPdfWriter pWriter(&pdfFile);

    pWriter.setPageSize(QPagedPaintDevice::A5);
    pWriter.setPageOrientation(QPageLayout::Orientation::Landscape);
    pWriter.setResolution(96);
    QPainter painter(&pWriter);

    printReport(painter);

    pWriter.setPageSize(QPagedPaintDevice::Custom);
    pWriter.setPageSizeMM(QSize(210,300));
    pWriter.setResolution(96);
    pWriter.setPageOrientation(QPageLayout::Orientation::Portrait);
    pWriter.newPage();

    QFont font;
    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(QPen(QColor(0,0,0),1));
    DataHandler::printCT(painter);

    pWriter.setPageSize(QPagedPaintDevice::Custom);
    pWriter.setPageSizeMM(QSize(240,DataHandler::NameSize()*190));
    pWriter.setResolution(96);
    pWriter.setPageOrientation(QPageLayout::Orientation::Portrait);
    pWriter.newPage();
    DataHandler::printImage(dirname,painter);

    painter.end();
    pdfFile.close();
    return true;
}
