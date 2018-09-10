#pragma once
#include "stable.h"
#include "process_table_model.h"
#include "performance_model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QPoint origin;
    bool isDragging;

    QTimer refreshTimer;
    ProcessTableModel processModel;
    PerformanceModel performanceModel;
    void setupUsagePlots();
    void setupStaticInformation();
private slots:
    void updateUsageOptionIcon();
    void refresh();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static const int REFRESH_RATE;
    void updateWidget(const QVariantList & property);
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
private:
    Ui::MainWindow *ui;
};
