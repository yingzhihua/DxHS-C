#include <QDir>
#include <QCoreApplication>
#include <QDebug>

#include "usbmodel.h"

bool Worker::CopyDir(const QString &fromDir, const QString &toDir, bool cover){
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if (!targetDir.exists()){
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    sourceDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = sourceDir.entryInfoList();
    foreach(QFileInfo file, fileList){
        if (stop) return false;
        if (file.isFile()){
            qDebug()<<"copy file"<<file.absoluteFilePath();
            if (cover && targetDir.exists(file.fileName()))
                targetDir.remove(file.fileName());
            if (!QFile::copy(file.filePath(),targetDir.filePath(file.fileName())))
                return false;
        }
        else{
            if (!CopyDir(file.filePath(),targetDir.filePath(file.fileName()),cover))
                return false;
        }
    }
    return true;
}

Worker::Worker(QObject *parent) : QObject(parent)
{
    qDebug()<<"new Worker";
    stop = false;    
}

void Worker::start(){
    qDebug()<<"do work thread id="<<QThread::currentThreadId();
    if (QFile::exists("/dev/sda1")){
        if (system("mount /dev/sda1 /mnt") != 0)
            emit Finish(2);
        else {
            QDir dir("/mnt/CopyData");
            if (!dir.exists())
                dir.mkpath("/mnt/CopyData");
            foreach(UsbNode node, copylist){
                qDebug()<<node.Name<<node.bDir<<node.bSelect;
                if (stop) break;
                if (node.bSelect){
                    if (node.bDir)
                    {
                        if (!CopyDir(QCoreApplication::applicationDirPath()+"/"+node.Name,"/mnt/CopyData/"+node.Name,true))
                            break;
                    }
                    else if (!QFile::copy(QCoreApplication::applicationDirPath()+"/"+node.Name,"/mnt/CopyData/"+node.Name))
                        break;
                }

            }
            system("sync");
            system("umount -l /mnt");
            emit Finish(0);
        }
    }else {
        emit Finish(1);
    }


}

UsbModel::UsbModel(QObject *parent):QAbstractListModel (parent)
{
    roles[RolesIsDir] = "IsDir";
    roles[RolesName] = "Name";
    roles[RolesSelect] = "Select";
    refresh();
    worker = nullptr;
}

int UsbModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_display_list.count();
}

QVariant UsbModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_display_list.count())
        return QVariant();

    const UsbNode &item = m_display_list[index.row()];

    if (role == RolesIsDir)
        return item.bDir;
    else if(role == RolesName)
        return item.Name;
    else if(role == RolesSelect)
        return item.bSelect;

    return QVariant();
}

QHash<int, QByteArray> UsbModel::roleNames() const
{
    return roles;
}

void UsbModel::refresh(){
    m_display_list.clear();
    QDir dir(QCoreApplication::applicationDirPath());
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList fileList = dir.entryInfoList();
    beginResetModel();
    foreach(QFileInfo node, fileList){
        //qDebug()<<"name:"<<node.fileName()<<node.isDir();
        if (node.fileName() == "DxHS" || node.fileName() == "data.db" ||
                node.fileName() == "FLASHDXcn.xml" || node.fileName() == "RawData.csv" ||
                node.fileName() == "en_US.qm" || node.fileName() == "UIParam.xml" ||
                node.fileName() == "Dxlogo.png")
            continue;
        UsbNode n;
        n.Name = node.fileName();
        n.bDir = node.isDir();
        n.bSelect = false;

        m_display_list<<n;
    }
    endResetModel();
}

void UsbModel::switchSelect(int index){
    if (index >= 0 && index < m_display_list.count()){
        beginResetModel();
        m_display_list[index].bSelect = !m_display_list[index].bSelect;
        endResetModel();
    }
}

static bool DelDir(const QString &path){
    qDebug()<<"DelDir"<<path;
    if (path.isEmpty())
        return false;
    QDir dir(path);
    if (!dir.exists())
        return true;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach(QFileInfo file, fileList){
        if (file.isFile())
            file.dir().remove(file.fileName());
        else
            DelDir(file.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}

static QString findUDisk(){
    if (system("fdisk -l > /opt/usbdevice") == 0){
        QFile file("/opt/usbdevice");
        if (!file.open(QIODevice::ReadOnly)){
            return "";
        }
        if (file.size()>0){
            QTextStream in(&file);
            QString line;
            bool startRead = false;
            int lineno = 0;
            while(!in.atEnd()){
                line = in.readLine();
                qDebug()<<++lineno<<line;
                if (startRead){
                    if (line.startsWith("/dev/sda"))
                    {
                        file.close();
                        return line.split(' ')[0];
                    }
                }
                else if (line.startsWith("Device"))
                    startRead = true;
            }
        }
        file.close();
    }
    return "";
}

void UsbModel::deleteDir(){
    qDebug()<<"deleteDir";
    QDir dir(QCoreApplication::applicationDirPath());
    foreach(UsbNode node, m_display_list){
        if (node.bSelect){
            if (node.bDir)
                DelDir(QCoreApplication::applicationDirPath()+"/"+node.Name);
            else
                dir.remove(node.Name);
        }
    }
    refresh();
}

QString UsbModel::updateSoft(){
    QString result =tr("????????????U??????");
    if (QFile::exists("/dev/sda1")){
        if (system("mount /dev/sda1 /mnt") != 0)
            return tr("U??????????????????????????????FAT32??????");
        bool updated = false;
        result = tr("?????????????????????");
        if (QFile::exists("/mnt/DxHS"))
        {
            QFile::remove("/opt/DxHS.cpy");
            QFile::copy("/mnt/DxHS","/opt/DxHS.cpy");
            result += " DxHS";
            updated = true;
        }

        if (QFile::exists("/mnt/data.db"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/data.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/data.db",QCoreApplication::applicationDirPath()+"/data.p");
            QFile::copy("/mnt/data.db",QCoreApplication::applicationDirPath()+"/data.db");
            result += " data.db\n";
            updated = true;
        }

        if (QFile::exists("/mnt/FLASHDXcn.xml"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/FLASHDXcn.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/FLASHDXcn.xml",QCoreApplication::applicationDirPath()+"/FLASHDXcn.p");
            QFile::copy("/mnt/FLASHDXcn.xml",QCoreApplication::applicationDirPath()+"/FLASHDXcn.xml");
            result += " FLASHDXcn.xml";
            updated = true;
        }
      /*
        if (QFile::exists("/mnt/FLASHDXen.xml"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/FLASHDXen.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/FLASHDXen.xml",QCoreApplication::applicationDirPath()+"/FLASHDXen.p");
            QFile::copy("/mnt/FLASHDXen.xml",QCoreApplication::applicationDirPath()+"/FLASHDXen.xml");
            result += " FLASHDXen.xml";
            updated = true;
        }
        */
        if (QFile::exists("/mnt/RawData.csv"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/RawData.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/RawData.csv",QCoreApplication::applicationDirPath()+"/RawData.p");
            QFile::copy("/mnt/RawData.csv",QCoreApplication::applicationDirPath()+"/RawData.csv");
            result += " RawData.csv\n";
            updated = true;
        }

        if (QFile::exists("/mnt/en_US.qm"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/en_US.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/en_US.qm",QCoreApplication::applicationDirPath()+"/en_US.p");
            QFile::copy("/mnt/en_US.qm",QCoreApplication::applicationDirPath()+"/en_US.qm");
            result += " en_US.qm";
            updated = true;
        }

        if (QFile::exists("/mnt/UIParam.xml"))
        {
            QFile::remove(QCoreApplication::applicationDirPath()+"/UIParam.p");
            QFile::rename(QCoreApplication::applicationDirPath()+"/UIParam.xml",QCoreApplication::applicationDirPath()+"/UIParam.p");
            QFile::copy("/mnt/UIParam.xml",QCoreApplication::applicationDirPath()+"/UIParam.xml");
            result += " UIParam.xml";
            updated = true;
        }

        if (!updated)
            result = tr("?????????????????????");
        system("sync");
        system("umount -l /mnt");
    }
    return result;
}

void UsbModel::copyDir(){
    /*
    if (QFile::exists("/dev/sda1")){
        system("mount /dev/sda1 /mnt");
        QDir dir("/mnt/CopyData");
        if (!dir.exists())
            dir.mkpath("/mnt/CopyData");
        foreach(UsbNode node, m_display_list){
            if (node.bSelect){
                if (node.bDir)
                    CopyDir(QCoreApplication::applicationDirPath()+"/"+node.Name,"/mnt/CopyData/"+node.Name,true);
                else
                    QFile::copy(QCoreApplication::applicationDirPath()+"/"+node.Name,"/mnt/CopyData/"+node.Name);
            }
        }
        system("sync");
        system("umount -l /mnt");
    }
    */
    startCpy();
}

void UsbModel::restart(){
    system("shutdown -r now");
}

void UsbModel::Progress(int val){
    qDebug()<<"UsbModel thread id"<<QThread::currentThreadId()<<"pa="<<val;
}

void UsbModel::Finish(int val){
    worker = nullptr;
    qDebug()<<"thread Finish";
    emit copyFinish(val);
}

void UsbModel::startCpy(){
    if (worker != nullptr)
        return;
    worker = new Worker;
    connect(worker,SIGNAL(Progress(int)),this,SLOT(Progress(int)));
    connect(worker,SIGNAL(Finish(int)),this,SLOT(Finish(int)));
    QThread *thread = new QThread;
    worker->moveToThread(thread);
    connect(thread,SIGNAL(started()),worker,SLOT(start()));
    connect(worker,SIGNAL(Finish(int)),worker,SLOT(deleteLater()));
    connect(worker,SIGNAL(destroyed()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    worker->setList(m_display_list);
    thread->start();
}

void UsbModel::stopCopy(){
    if (worker != nullptr)
        worker->stopWork();
}
