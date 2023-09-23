#include "com_node.h"
#include <cstdio>
#include <rclcpp/logging.hpp>
#include <rclcpp/parameter_value.hpp>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <math.h>

#define deg2Rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define rad2Deg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

Comnode::Comnode() : Node(NODE_NOME){
    this->declare_parameter(PARAMS_TOPIC_ACKERMANN,"/");
	
    this->get_parameter(PARAMS_TOPIC_ACKERMANN,m_ackermann_topic);
	this->declare_parameter(SERIAL_FILE,"/tmp/msgcom_serial"); 
	this->get_parameter(SERIAL_FILE,m_serial_path);
	this->declare_parameter(SERIAL_BAUDRATE,m_serial_baudrate);
	this->get_parameter(SERIAL_BAUDRATE,m_serial_baudrate);

	struct termios tty;

    m_ackermann_subscriber = this->create_subscription<ackermann_msgs::msg::AckermannDrive>(
        m_ackermann_topic, 10, std::bind(&Comnode::waypoint_callback, this, std::placeholders::_1));
	
	// );
    file_descriptor = open(m_serial_path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	int flag_a=0;
	switch(m_serial_baudrate){
		case 115200:
			flag_a = B115200;
			break;
		case 19200:
			flag_a = B19200;
			break;
		case 38400:
			flag_a = B38400;
			break;
		case 57600:
			flag_a = B57600;
			break;
		case 9600:
		default:
			flag_a = B9600;
			break;
	}

	tcgetattr(file_descriptor, &tty);
	cfsetispeed(&tty, flag_a);
    cfsetospeed(&tty, flag_a);

	tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;  // No parity
    tty.c_cflag &= ~CSTOPB;  // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

	tcsetattr(file_descriptor, TCSANOW, &tty);

	if (file_descriptor <0) { 
		perror(m_serial_path.c_str()); 
		RCLCPP_ERROR(this->get_logger(), "Error opening file_descriptor for serial communication"); 
	}


	
}
/** ofstream might be a pointer or shared pointer for now i dont need this boylerplate anyway
void Comnode::update_FileDescriptor(){
	if(!file_descriptor || !file_descriptor.is_open()){
		RCLCPP_INFO_ONCE(this->get_logger(), "File wasnt open, getter 'instantiated' it"); 

		file_descriptor.open(m_serial_path);
	}

	
}**/
std::string Comnode::g_AckermannTopic(){
    std::string topic;
    this -> get_parameter(PARAMS_TOPIC_ACKERMANN, topic);
    return topic;
}
std::string Comnode::g_NodeName(){
	return NODE_NOME;
}
void Comnode::velocity_rpm(double velocity){
	return (velocity*M_PI_2*0.26)/(4*60);
}

void Comnode::waypoint_callback(ackermann_msgs::msg::AckermannDrive::SharedPtr msg){
    float angle;
    float velocity;
    

    ackermann_msgs::msg::AckermannDrive local_msg = *msg;
    angle = local_msg.steering_angle;
    velocity = local_msg.speed;
	std::string message = std::to_string(rad2Deg((int)angle)) + "d" + std::to_string(velocity_rpm((int)velocity))+"\n";
	RCLCPP_INFO(this->get_logger(), "Sending message: %s", message.c_str()); 

	auto k = write (file_descriptor, message.c_str(), message.length());
	if(k == -1 || k < (ssize_t)message.length()){
		RCLCPP_ERROR(this->get_logger(), "Error writing to file_descriptor for serial communication");
	}

}

int  Comnode::g_SerialBaudRate(){
	this->get_parameter(SERIAL_BAUDRATE,this->m_serial_baudrate);
	return this->m_serial_baudrate;
}
void Comnode::s_SerialBaudRate(int baudrate){
	this->m_serial_baudrate = baudrate;
	//Overrides previous value
	this->declare_parameter(SERIAL_BAUDRATE, baudrate);
	

	//this->set_parameter(p);

	
}
Comnode::~Comnode(){
	auto close_status = close(file_descriptor);
	if(close_status == -1){
		RCLCPP_ERROR(this->get_logger(), "Error closing file_descriptor for serial communication"); 
	}
}