#ifndef COM_NODE_H
#define COM_NODE_H

#define NODE_NOME "msgcom"
#define PARAMS_TOPIC_ACKERMANN "ackermann_topic"  
#define SERIAL_FILE "uart_serialterminal"
#define FILENOTFOUND 1

#include <stdio.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <rclcpp/node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors.hpp>
#include <rclcpp/logging.hpp>
#include <ackermann_msgs/msg/ackermann_drive.hpp>

class Comnode : public rclcpp::Node{
    public:
        Comnode();
        
        
        std::string g_NodeName();
        std::string g_AckermannTopic();
		//void update_FileDescriptor(); 
		virtual ~Comnode();

    

    protected:
        std::string m_ackermann_topic;
		std::string m_serial_path;
		std::ofstream file_descriptor;
        void waypoint_callback(const ackermann_msgs::msg::AckermannDrive::SharedPtr msg);

    private:
		
        rclcpp::Subscription<ackermann_msgs::msg::AckermannDrive>::SharedPtr m_ackermann_subscriber;

};
#endif