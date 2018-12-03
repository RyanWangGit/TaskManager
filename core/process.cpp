#include "process.h"
#include "process_table_model.h"

Process::Process(unsigned int id, QObject * parent)
    :QObject(parent)
{
    lastCpuTime = -1;
    lastDiskIO = -1;

    // append info into the property list
    propertyList << QString() << id << 0.0f << 0.0f << 0.0f << 0.0f;

    // open files
    QFile stat(QString("/proc/%1/stat").arg(id));
    QFile statm(QString("/proc/%1/statm").arg(id));


    if(!stat.exists() ||
            !statm.exists() ||
            !stat.open(QIODevice::ReadOnly) ||
            !statm.open(QIODevice::ReadOnly))
        return;

    Q_ASSERT(stat.isOpen() && statm.isOpen());

    // get process name
    QStringList statContent = QString(stat.readAll()).split(" ");
    QString name = statContent.at(1);
    name.chop(1);
    name.remove(0, 1);

    propertyList[ProcessName] = name;

    stat.close();
    statm.close();
}

Process::~Process()
{

}

const QVariant & Process::property(int propertyName)
{
    return propertyList.at(propertyName);
}

bool Process::refresh(unsigned int totalCpuTimeDiff)
{
    // open files
    QFile stat(QString("/proc/%1/stat").arg(propertyList.at(ID).toUInt()));

    QFile statm(QString("/proc/%1/statm").arg(propertyList.at(ID).toUInt()));
    QFile io(QString("/proc/%1/io").arg(propertyList.at(ID).toUInt()));
    if(!stat.exists() || !statm.exists() || !io.exists())
        return false;
    if(!stat.open(QIODevice::ReadOnly) || !statm.open(QIODevice::ReadOnly) || !io.open(QIODevice::ReadOnly))
        return false;

    QStringList statContent = QString(stat.readAll()).split(" ");
    QStringList statmContent = QString(statm.readAll()).split(" ");
    QStringList ioContent = QString(io.readAll()).split("\n");

    // get data snapshot
    unsigned int curCpuTime = 0;
    for(int i = 13; i <= 14; i ++)
    {
        const QString & item = statContent.at(i);
        curCpuTime += item.toUInt();
    }
    unsigned int curDiskIO = 0;
    curDiskIO += ioContent[5].split(" ").at(1).toULong();
    curDiskIO += ioContent[6].split(" ").at(1).toULong();

    // calculate if data is ready
    if(lastCpuTime > 0)
    {
        float curCpuTimeDiff = curCpuTime - lastCpuTime;
        if(totalCpuTimeDiff == 0 || curCpuTimeDiff == 0)
            propertyList[CPUUsage] = 0;
        else
            propertyList[CPUUsage] = 100 * ((float)curCpuTimeDiff / totalCpuTimeDiff);
    }
    if(lastDiskIO > 0)
        propertyList[DiskUsage] = curDiskIO - lastDiskIO;

    propertyList[MemoryUsage] = statmContent.at(1).toUInt() * (getpagesize() / 1024);

    // refresh last statistics
    lastCpuTime = curCpuTime;
    lastDiskIO = curDiskIO;
    stat.close();
    statm.close();
    return true;
}

void Process::setProperty(int propertyName, const QVariant &property)
{
    this->propertyList[propertyName] = property;
}
