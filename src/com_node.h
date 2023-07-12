#ifndef COM_NODE_H
#define COM_NODE_H

#define NODE_NOME "msgcom"
#define PARAMS_TOPIC_ACKERMANN "ackermann_topic"
#define SERIAL_FILE "/home/fsipleiria_ad/serialcom"
#define FILENOTFOUND 1

#include <stdio.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <ackermann_msgs/msg/ackermann_drive.hpp>

class Comnode : public rclcpp::Node{
    public:
        Comnode();
        virtual ~Comnode() = default;
        
        std::string g_NodeName();
        std::string g_AckermannTopic();


    protected:
        std::string m_ackermann_topic;
        void waypoint_callback(const ackermann_msgs::msg::AckermannDrive::SharedPtr msg);
};
#endif