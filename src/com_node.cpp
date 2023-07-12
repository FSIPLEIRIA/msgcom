#include "com_node.h"

Comnode::Comnode() : Node(NODE_NOME){
    this->declare_parameter(PARAMS_TOPIC_ACKERMANN,"/");
    this->get_parameter(PARAMS_TOPIC_ACKERMANN,m_ackermann_topic);
    auto m_ackermann_topic2 = this->create_subscription<ackermann_msgs::msg::AckermannDrive>(
        m_ackermann_topic, 10, std::bind(&Comnode::waypoint_callback, this, std::placeholders::_1));
}
std::string Comnode::g_AckermannTopic(){
    std::string topic;
    this -> get_parameter(PARAMS_TOPIC_ACKERMANN, topic);
    return topic;
}
void Comnode::waypoint_callback(ackermann_msgs::msg::AckermannDrive::SharedPtr msg){
    float angle;
    float velocity;
    std::ofstream file;
    file.open(SERIAL_FILE);
    ackermann_msgs::msg::AckermannDrive local_msg = *msg;
    angle = local_msg.steering_angle;
    velocity = local_msg.speed;
    angle = 45.54;
    velocity = 90.54;
    //TODO Falar com o guilherme sobre quem trata dos "0's" 
    file << printf("S-%3.4fR-%3.4f", velocity, angle);
    file.close();
}