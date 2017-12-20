#ifndef TESTWINDOW_HH
#define TESTWINDOW_HH

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

private slots:

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::TestWindow *ui;
};

#endif // TESTWINDOW_HH
