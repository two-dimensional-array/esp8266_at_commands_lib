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
void ESP8266_UART::Begin(){
    char *buf = new char(UART_BUF_SIZE);
    Transmit_UART(buf,Get_UART_Config(buf));
    delete[] buf;
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

// ESP8266_WIFI
ESP8266_WIFI::ESP8266_WIFI()
{
    Init_var();
    *this->ssid = '\0';
    *this->password = '\0';
    *this->ip = '\0';
    *this->ip_gateway = '\0';
    *this->ip_netmask = '\0';
    *this->mac = '\0';
}
void ESP8266_WIFI::Set_IP_Adress(const char ip[])
{
    strcpy(this->ip, ip);
}
void ESP8266_WIFI::Set_IP_Adress(const char ip[], const char ip_gateway[], const char ip_netmask[])
{
    this->Set_IP_Adress(ip);
    strcpy(this->ip_gateway, ip_gateway);
    strcpy(this->ip_netmask, ip_netmask);
}
void ESP8266_WIFI::Set_MAC_Adress(const char mac[])
{
    strcpy(this->mac, mac);
}
void ESP8266_WIFI::Set_WIFI(const char ssid[])
{
    strcpy(this->ssid, ssid);
}
void ESP8266_WIFI::Set_WIFI(const char ssid[], const char password[])
{
    strcpy(this->ssid, ssid);
    strcpy(this->password, password);
}
uint8_t ESP8266_WIFI::Get_WIFI_Mode(char *buf, Wifi_mode wifimode)
{
    return (uint8_t)sprintf(buf, COMMAND_CONFIG_WIFI_MODE, wifimode);
}
void ESP8266_WIFI::Init_var()
{
    this->ssid = new char(MAX_SIZE_SSID);
    this->password = new char(MAX_SIZE_PASSWORD);
    this->ip = new char(MAX_SIZE_IP);
    this->ip_gateway = new char(MAX_SIZE_IP);
    this->ip_netmask = new char(MAX_SIZE_IP);
    this->mac = new char(MAX_SIZE_MAC);
}
ESP8266_WIFI::~ESP8266_WIFI()
{
    delete[] ssid;
    delete[] password;
    delete[] ip;
    delete[] ip_gateway;
    delete[] ip_netmask;
    delete[] mac;
}
// ESP8266_WIFI

//Soft_AP
Soft_AP::Soft_AP()
{
    Init_var();
    *this->wifisecurity = OPEN;
    *this->channel = 1;
}
void Soft_AP::Set_WIFI(const char ssid[])
{
    ESP8266_WIFI::Set_WIFI(ssid);
    *this->wifisecurity = OPEN;
}
void Soft_AP::Set_WIFI(const char ssid[], uint8_t channel)
{
    Soft_AP::Set_WIFI(ssid);
    *this->channel = channel;
}
void Soft_AP::Set_WIFI(const char ssid[], const char password[])
{
    ESP8266_WIFI::Set_WIFI(ssid, password);
    *this->wifisecurity = (*wifisecurity != OPEN) ? *wifisecurity : WPA_PSK;
}
void Soft_AP::Set_WIFI(const char ssid[], const char password[], uint8_t channel, Wifi_security wifisecurity)
{
    *this->wifisecurity = wifisecurity;
    Soft_AP::Set_WIFI(ssid, password);
    *this->channel = channel;
}
void Soft_AP::Begin()
{
    char *buf = new char(UART_BUF_SIZE);
    Transmit_UART(buf,Get_WIFI_Mode(buf,Soft_AP_mode));
    if(*ip != '\0')
    {
        Transmit_UART(buf,Get_IP_Adress(buf));
    }
    if(*mac != '\0')
    {
        Transmit_UART(buf,Get_MAC_Adress(buf));
    }
    Transmit_UART(buf,Get_WIFI_Start(buf));
    delete[] buf;
}
uint8_t Soft_AP::Get_IP_Adress(char *buf)
{
    return (uint8_t)(*ip_gateway != '\0' && *ip_netmask != '\0') ? sprintf(buf, COMMAND_CONFIG_WIFI_IP_SOFT_AP, ip, ip_gateway, ip_netmask) : sprintf(buf, COMMAND_CONFIG_WIFI_IP_SOFT_AP_SHORT, ip);
}
uint8_t Soft_AP::Get_MAC_Adress(char *buf)
{
    return (uint8_t)sprintf(buf,COMMAND_CONFIG_WIFI_MAC_SOFT_AP, mac);
}
uint8_t Soft_AP::Get_WIFI_Start(char *buf)
{
    return (uint8_t)(*password != '\0' || *wifisecurity != OPEN) ? sprintf(buf, COMMAND_CONFIG_WIFI_SOFT_AP_START_WITH_PASSWORD, ssid, password, *channel, *wifisecurity) : sprintf(buf, COMMAND_CONFIG_WIFI_SOFT_AP_START_WITHOUT_PASSWORD, ssid, *channel);
}
void Soft_AP::Init_var()
{
    wifisecurity = new Wifi_security;
    channel = new uint8_t;
}
Soft_AP::~Soft_AP()
{
    delete wifisecurity;
    delete channel;
}
//Soft_AP

//Station
void Station::Begin()
{
    char *buf = new char(UART_BUF_SIZE);
    Transmit_UART(buf,Get_WIFI_Mode(buf,Station_mode));
    if(*ip != '\0')
    {
        Transmit_UART(buf,Get_IP_Adress(buf));
    }
    if(*mac != '\0')
    {
        Transmit_UART(buf,Get_MAC_Adress(buf));
    }
    Transmit_UART(buf,Get_WIFI_Start(buf));
    delete[] buf;
}
uint8_t Station::Get_IP_Adress(char *buf)
{
    return (uint8_t)(*ip_gateway != '\0' && *ip_netmask != '\0') ? sprintf(buf, COMMAND_CONFIG_WIFI_IP_STATION, ip, ip_gateway, ip_netmask) : sprintf(buf, COMMAND_CONFIG_WIFI_IP_STATION_SHORT, ip);
}
uint8_t Station::Get_MAC_Adress(char *buf)
{
    return (uint8_t)sprintf(buf,COMMAND_CONFIG_WIFI_MAC_STATION, mac);
}
uint8_t Station::Get_WIFI_Start(char *buf)
{
    return (uint8_t)(*password != '\0') ? sprintf(buf, COMMAND_CONFIG_WIFI_STATION_START_WITH_PASSWORD, ssid, password) : sprintf(buf, COMMAND_CONFIG_WIFI_STATION_START_WITHOUT_PASSWORD, ssid);
}
//Station
