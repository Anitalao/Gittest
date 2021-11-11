#include <QCompleter>
#include <QDebug>
//#include <QHostAddress>
#include <QMessageBox>
#include <QMovie>
//#include <QNetworkInterface>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStringListModel>

#include <QSettings>
#include <string>
#include <ros/network.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <qinputdialog.h>

#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <iostream>

static bool ROS_LINK = false;

VizlibTest::VizlibTest(int argc, char **argv, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VizlibTest)
{
    ui->setupUi(this);
    //窗体标题
    this->setWindowTitle("WHEELTEC APP");
    //窗体ICO图片，如图不起别名，后缀直接写图片全名。
    // this->setWindowIcon(QIcon(":/image/image/SetGoal.png"));
      // setWindowIcon(QIcon("://image/robot.png"));
          // ui->label_8->setPixmap(QPixmap("://image/robot.png"));
      //  ui-> label_3->setIcon(QIcon("://image/robot.png"));
    //UI界面初始化
    ui_init();   
    //读取配置
    ReadSettings();
    QObject::connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(slot_btn_display()));
        connections();    //连接成功才能执行此函数

}

VizlibTest::~VizlibTest()
{
    delete ui;
}


//UI界面初始化
void VizlibTest::ui_init() {
  ROS_INFO("UI界面初始化");
  ui->pushmapping->setStyleSheet("background-color: rgb(0,255,0)");//设置按键绿色
  ui->pushnavigation->setStyleSheet("background-color: rgb(0,255,0)");//设置按键绿色


  // ui->pushmapping->setIcon(QIconQPixmap((":/image/image/SetGoal.png")));
  // ui->set_goal_btn_2->setIcon(QIcon(":/image/image/Pose.png"));
  // ui->set_mutil_goal_btn_2->setIcon(QIcon(QPixmap(":/image/image/PublishPoint.svg")));
  
}


void VizlibTest::connections() {
    
//设置2D 目标位置
   connect(ui->set_goal_btn_2, SIGNAL(clicked()), this, SLOT(slot_set_2D_Goal()));
  // //设置2D Pose初始位置
  connect(ui->set_pos_btn_2, SIGNAL(clicked()), this, SLOT(slot_set_2D_Pos()));
//设置2D 多点目标位置
    connect(ui->set_mutil_goal_btn_2, SIGNAL(clicked()), this, SLOT(slot_set_mutil_Goal()));
  

   QObject::connect(ui->pushmapping, &QPushButton::clicked,[=](){
       ROS_INFO("发送启动建图话题");
       pub_my_cmd("finish"); 
       pub_my_cmd("mapping");
      // if(ROS_LINK)
        //  display_rviz();     //建图的时候需要更新一下显示，否则局部地图会残留
        map_rviz->Display_Navigate(false,"/move_base/global_costmap/costmap","/move_base/NavfnROS/plan","/move_base/local_costmap/costmap","/move_base/DWAPlannerROS/local_plan");
   });
QObject::connect(ui->pushnavigation, &QPushButton::clicked,[=](){
    map_rviz->Display_Navigate(true,"/move_base/global_costmap/costmap","/move_base/NavfnROS/plan","/move_base/local_costmap/costmap","/move_base/DWAPlannerROS/local_plan");
      pub_my_cmd("finish");
       if(ui->checkBoxna->isChecked())
       {
          ROS_INFO("发送启动导航话题");
          // map_rviz->clear();        //清除RVIZ显示     
          pub_my_cmd("navigation");  
       }else{
          ROS_INFO("发送自定义地图导航话题");
          // map_rviz->clear();        //清除RVIZ显示
          bool ok;
            QString text = QInputDialog::getText(this, tr("选择导航地图"),tr("请输入地图名称"), QLineEdit::Normal,0, &ok);
            //  pub_my_cmd("savemap");    
            qDebug()<<text;   
          if (ok && !text.isEmpty())
          {
            text = "#" + text;
              ROS_INFO("输入导航地图");
              pub_my_cmd(text);  
          }
       }   
   });

QObject::connect(ui->pushfinish, &QPushButton::clicked,[=](){
       ROS_INFO("发送结束任务话题");
        // map_rviz->clear();        //清除RVIZ显示
        // display_rviz();
       pub_my_cmd("finish");  
   });

QObject::connect(ui->pushclear_point, &QPushButton::clicked,[=](){
       ROS_INFO("发送清除导航点话题");
       pub_my_cmd("clear_point");       
   });

QObject::connect(ui->pushsavemap, &QPushButton::clicked,[=](){
       ROS_INFO("保存地图话题");
       if(ui->checkBoxmap->isChecked())
       {
          ROS_INFO("自动保存");
          pub_my_cmd("savemap");  
       }else{
              ROS_INFO("自定义保存");
              bool ok;
              QString text = QInputDialog::getText(this, tr("保存地图"),tr("请输入地图名称"), QLineEdit::Normal,0, &ok);
          //  pub_my_cmd("savemap");    
              qDebug()<<text;   
              if (ok && !text.isEmpty())
              {
                ROS_INFO("地图输入正确");
                text = "*" + text;
                pub_my_cmd(text);  
              }
        }  
   });

}

void VizlibTest::ReadSettings() {
    QSettings connect_info("app", "connect_info");

    ui->lineEditMasterIp->setText( connect_info.value("master_url", m_qMasterIp).toString());
    ui->lineEditRosIp->setText( connect_info.value("host_url", m_qRosIp).toString());

    qDebug()<<connect_info.value("master_url", m_qMasterIp).toString();
    qDebug()<<connect_info.value("master_url", m_qMasterIp).toString();
}


//初始化的函数*********************************
bool VizlibTest::init(const std::string& master_url, const std::string& host_url) {
  std::map<std::string, std::string> remappings;
  remappings["__master"] = master_url;
  remappings["__hostname"] = host_url;
        ROS_INFO("---------------%s",remappings["__master"].c_str());
        ROS_INFO("---------------%s",remappings["__hostname"].c_str());
  ros::init(remappings, "app_monitor", ros::init_options::AnonymousName);
  if (!ros::master::check()) {
    return false;
  }


  ros::start();  // explicitly needed since our nodehandle is going out of
                 // scope.
  SubAndPubTopic();    //创建订阅者与发布者
  // start();
  return true;
}

//创建订阅者与发布者
void VizlibTest::SubAndPubTopic() {
  ros::NodeHandle h;

 // 创建一个Publisher，发布名为launch_pub的topic，消息类型为std_msgs::Strin
 app_pub =   h.advertise<std_msgs::String>("app_pub", 1000);
 app_sub    = h.subscribe("app_sub", 1000,
                           &VizlibTest::chatterCallBack, this);

}

//发布launch命令信息
void VizlibTest::pub_my_cmd(QString date) {
   if(ROS_LINK){
        // 初始化std_msgs::String类型的消息
        std_msgs::String msg;  
        msg.data = date.toStdString();
 
        // 发布消息
        ROS_INFO("发布消息---%s", msg.data.c_str());
       
           app_pub.publish(msg);
 
        // 循环等待回调函数
        ros::spinOnce();
   }
}

//APP 监听回调函数
void VizlibTest::chatterCallBack(const std_msgs::String::ConstPtr &msg)
{
    ROS_WARN("app_sub date is: [%s]     ",msg->data.c_str());
    if(msg->data=="map_succeed"){
          QMessageBox::information(NULL, "保存地图",
                          "地图保存成功！",
                          QMessageBox::Yes);
    }
}

void VizlibTest::display_rviz() {
  QSettings settings("cyrobot_monitor", "Displays");
  bool Grid_enable = settings.value("Grid/enable", bool(true)).toBool();
  double Grid_count = settings.value("Grid/count", double(20)).toDouble();

  bool Map_enable = settings.value("Map/enable", bool(true)).toBool();
  QString Map_topic = settings.value("Map/topic", QString("/map")).toString();
  double Map_alpha = settings.value("Map/alpha", double(0.7)).toDouble();
  QString Map_scheme = settings.value("Map/scheme", QString("map")).toString();
  bool Laser_enable = settings.value("Laser/enable", bool(true)).toBool();
  QString Laser_topic =
      settings.value("Laser/topic", QString("/scan")).toString();
  bool Polygon_enable = settings.value("Polygon/enable", bool(true)).toBool();
  QString Polygon_topic =
      settings
          .value("Polygon/topic", QString("/move_base/local_costmap/footprint"))
          .toString();

  bool RobotModel_enable =
      settings.value("RobotModel/enable", bool(true)).toBool();
  bool Navigation_enable =
      settings.value("Navigation/enable", bool(true)).toBool();

  bool MarkerArray_enable =
      settings.value("MarkerArray/enable", bool(true)).toBool();
    QString MarkerArray_topic =
      settings.value("MarkerArray/topic", QString("/path_point")).toString();

  QString GlobalMap_topic =
      settings
          .value("Navigation/GlobalMap/topic",
                 QString("/move_base/global_costmap/costmap"))
          .toString();
  QString GlobalMap_paln = settings
                               .value("Navigation/GlobalPlan/topic",
                                      QString("/move_base/NavfnROS/plan"))
                               .toString();
  QString LocalMap_topic =
      settings
          .value("Navigation/LocalMap/topic",
                 QString("/move_base/local_costmap/costmap"))
          .toString();
  QString LocalMap_plan =
      settings
          .value("Navigation/LocalPlan/topic",
                 QString("/move_base/DWAPlannerROS/local_plan"))
          .toString();


  QTimer::singleShot(500,[=](){
      map_rviz->Display_Grid(Grid_enable, "QGrid", Grid_count,
                             QColor(160, 160, 160));
  });

  QTimer::singleShot(800,[=](){
      ROS_INFO("地图显示");
      map_rviz->Display_Map(Map_enable, Map_topic, Map_alpha, Map_scheme);
    });

  // QTimer::singleShot(1000,[=](){
  //     ROS_INFO("局部地图显示");
  //     map_rviz->Local_Costmap(true);
  //   });

      QTimer::singleShot(2500,[=](){
      ROS_INFO("模型显示");
      map_rviz->Display_RobotModel(RobotModel_enable);
    });

//显示多点导航坐标
  QTimer::singleShot(1500,[=](){
      ROS_INFO("导航显示");
      map_rviz->Display_MarkerArray(MarkerArray_enable, MarkerArray_topic);
});

  QTimer::singleShot(3000,[=](){
      map_rviz->Display_LaserScan(Laser_enable, Laser_topic);
  });
 QTimer::singleShot(2000,[=](){
     map_rviz->Display_Navigate(Navigation_enable,GlobalMap_topic,GlobalMap_paln,LocalMap_topic,LocalMap_plan);
 });
}

//设置导航当前位置按钮的槽函数
void VizlibTest::slot_set_2D_Pos() {
 if(ROS_LINK)
    map_rviz->Set_Pos();
}

//设置导航目标位置按钮的槽函数
void VizlibTest::slot_set_2D_Goal() {
  if(ROS_LINK)
    map_rviz->Set_Goal();

}


//设置多点导航目标位置按钮的槽函数
void VizlibTest::slot_set_mutil_Goal() {
 if(ROS_LINK)
    map_rviz->Set_mutil_Goal();
    ROS_INFO("进入多点导航槽函数");
}

void VizlibTest::slot_btn_display()
{
    static bool flag = false;
    flag = !flag;
    // qDebug()<<"flag"<<flag;
// map_rviz = new QRviz(ui.verticalLayout_rviz, "qrviz");
    if(flag == true)
    {
         ROS_WARN("连接");
        bool isConnect = init(ui->lineEditMasterIp->text().toStdString(),ui->lineEditRosIp->text().toStdString());
        qDebug()<<"isConnect"<<isConnect;
        if(isConnect){
              
                ui->mapViz_2->hide();
                ui->widget_rviz_2->show();
                if (map_rviz == NULL) {
                    map_rviz = new QRviz(ui->verticalLayout_rviz_2, "qrviz");
                }
                display_rviz();   //显示RVIZ

                ui->pushButton_start->setText("断开连接");

                QSettings connect_info("app", "connect_info");
                connect_info.setValue("master_url", ui->lineEditMasterIp->text());
                connect_info.setValue("host_url", ui->lineEditRosIp->text());
                ROS_LINK = true;    //ROS连接成功标志
          }
        else{
                QMessageBox::information(NULL, "连接失败",
                                      "连接失败！请检查你的连接配置或重启重试！",
                                      QMessageBox::Yes);
                  flag = false;
                  ROS_LINK = false;
          }

    }
    else 
    {
      ROS_WARN("断开连接");
        ros::shutdown();
        ui->pushButton_start->setText("连接");
    }
}

void VizlibTest::closeEvent(QCloseEvent *event)
{
  ros::shutdown();
  // this->close();
}
