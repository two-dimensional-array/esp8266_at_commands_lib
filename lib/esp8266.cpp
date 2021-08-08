#include "esp8266.hpp"

// ESP8266_UART
ESP8266_UART::ESP8266_UART()
{
    Init_var();
    Set_UART_Config(SPEED_115200,BIT_8,STOP_BIT_1,NO_PARITY,NO_FLOW_CONTROL);
}
ESP8266_UART::ESP8266_UART(Baud_rate baudrate, Data_bits databits, Stop_bits stopbits, Parity parity, Flow_control flowcontrol)
{
    Init_var();
    Set_UART_Config(baudrate,databits,stopbits,parity,flowcontrol);
}
void ESP8266_UART::Set_UART_Config(Baud_rate baudrate, Data_bits databits, Stop_bits stopbits, Parity parity, Flow_control flowcontrol)
{
    *this->baudrate = baudrate;
    *this->databits = databits;
    *this->stopbits = stopbits;
    *this->parity = parity;
    *this->flowcontrol = flowcontrol;
}
uint8_t ESP8266_UART::Get_UART_Config(char *buf)
{
    return (uint8_t)sprintf(buf, COMMAND_CONFIG_UART, *baudrate, *databits, *stopbits, *parity, *flowcontrol);
}
void ESP8266_UART::Init_var()
{
    this->baudrate = new Baud_rate;
    this->databits = new Data_bits;
    this->stopbits = new Stop_bits;
    this->parity = new Parity;
    this->flowcontrol = new Flow_control;
}
ESP8266_UART::~ESP8266_UART()
{
    delete baudrate;
    delete databits;
    delete parity;
    delete flowcontrol;
}
// ESP8266_UART
