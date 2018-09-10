#pragma once
#include "stable.h"

class PerformanceModel : public QObject
{
    Q_OBJECT
private:
    unsigned long lastCpuUseTime;
    unsigned long curCpuUseTime;
    unsigned long lastCpuTime;
    unsigned long curCpuTime;

    QVariantList propertyList;
    void refreshCpuTemperatures();
    void refreshCpuTime();
    void refreshMemoryInfo();
    void refreshCpuInfo();
public:
    enum Type
    {
        CpuUtilization, CpuSpeed, Processes, CpuUpTime, CpuTemperature,
        MemoryAvailable, MemoryTotal, MemoryCached,
        TypeCount
    };

    PerformanceModel(QObject * parent = nullptr);
    void refresh();
signals:
    void sendSharedData(unsigned int cpuUtilization, unsigned int memoryUtilization, unsigned long totalCpuTimeDiff);
    void updateWidget(const QVariantList & property);
};
