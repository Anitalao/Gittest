#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool_manager.h>

#include <QTimer>
#include <ros/ros.h>
#include <std_msgs/String.h>

#include "qrviz.hpp"
namespace Ui {
class VizlibTest;
}


class VizlibTest : public QWidget
{
    Q_OBJECT

public:
    explicit VizlibTest(int argc, char **argv, QWidget *parent = 0);
    ~VizlibTest();

    bool init(const std::string& master_url, const std::string& host_url);
    void ui_init();
    void ReadSettings();

 public slots:
    // void slot_changeMapType(int);
    void slot_set_2D_Goal();
    void slot_set_2D_Pos();
    void slot_set_mutil_Goal();

private slots:
    void slot_btn_display();
    // void slot_btn_quit();

private:
    rviz::VisualizationManager *manager_;
    rviz::RenderPanel * render_panel_;
    Ui::VizlibTest *ui;

    QRviz *map_rviz = NULL;
    void display_rviz();
    void connections();
    void SubAndPubTopic();
    void pub_my_cmd(QString date);
    void chatterCallBack(const std_msgs::String::ConstPtr &msg);


private:
        ros::Publisher app_pub;
        ros::Subscriber app_sub;

        QString m_qRosIp;
        QString m_qMasterIp;

protected:
    void closeEvent(QCloseEvent *event);
    
};


#endif // LOGINWIDGET_H
