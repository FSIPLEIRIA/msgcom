#ifndef COM_NODE_H
#define COM_NODE_H

#define NODE_NOME "msgcom"
#define PARAMS_TOPIC_ACKERMANN "ackermann_topic"  
#define SERIAL_FILE "uart_serialterminal"
#define SERIAL_BAUDRATE "serial_baudrate"
#define FILENOTFOUND 1
#define _POSIX_SOURCE 1

#include <stdio.h>
#include <iostream>
#include <termios.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <sys/signal.h> 

#include <termios.h> 
#include <string>
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
		int g_SerialBaudRate(); 
		void s_SerialBaudRate(int baudrate);
		//void update_FileDescriptor(); 
		virtual ~Comnode();
        double velocity_rpm(double velocity);
    

    protected:
        std::string m_ackermann_topic;
		std::string m_serial_path;
		int m_serial_baudrate;
		int file_descriptor;
        void waypoint_callback(const ackermann_msgs::msg::AckermannDrive::SharedPtr msg);

    private:
        rclcpp::Subscription<ackermann_msgs::msg::AckermannDrive>::SharedPtr m_ackermann_subscriber;

};
#endif