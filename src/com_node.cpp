#include "com_node.h"


Comnode::Comnode() : Node(NODE_NOME){
    this->declare_parameter(PARAMS_TOPIC_ACKERMANN,"/");
	
    this->get_parameter(PARAMS_TOPIC_ACKERMANN,m_ackermann_topic);
	this->declare_parameter(SERIAL_FILE,"/tmp/msgcom_serial"); 
	this->get_parameter(SERIAL_FILE,m_serial_path);
    m_ackermann_subscriber = this->create_subscription<ackermann_msgs::msg::AckermannDrive>(
        m_ackermann_topic, 10, std::bind(&Comnode::waypoint_callback, this, std::placeholders::_1));
	
    file_descriptor.open(m_serial_path);
	if (!file_descriptor.is_open()){
		//throw error on ros2
		RCLCPP_ERROR(this->get_logger(), "Error opening file_descriptor for serial communication"); 
		return; 
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

void Comnode::waypoint_callback(ackermann_msgs::msg::AckermannDrive::SharedPtr msg){
    float angle;
    float velocity;
    

    ackermann_msgs::msg::AckermannDrive local_msg = *msg;
    angle = local_msg.steering_angle;
    velocity = local_msg.speed;
    
    //TODO Falar com o guilherme sobre quem trata dos "0's" 
    file_descriptor << "S-" << angle << "R-" << velocity << std::endl;
    
}
Comnode::~Comnode(){
	if (!file_descriptor.is_open()){
		RCLCPP_INFO_ONCE(this->get_logger(), "File already closed? If you are purposely closing the serial con");
	}else{
		file_descriptor.close();
	}
	
}