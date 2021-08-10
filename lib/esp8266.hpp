#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define UART_BUF_SIZE 64
#define MAX_SIZE_IP 15
#define MAX_SIZE_MAC 17
#define MAX_SIZE_SSID 64
#define MAX_SIZE_PASSWORD 64

#define COMMAND_CONFIG_UART "AT+UART_CUR=%i,%i,%i,%i,%i\n\r"                    // baud_rate, data_bits, stop_bits, parity, flow_control
#define COMMAND_CONFIG_WIFI_MODE "AT+CWMODE_CUR=%d\n\r"                         // wifi_mode
#define COMMAND_CONFIG_WIFI_MAC_SOFT_AP "AT+CIPAPMAC_CUR=\"%s\"\n\r"            // mac_adress
#define COMMAND_CONFIG_WIFI_IP_SOFT_AP "AT+CIPAP_CUR=\"%s\",\"%s\",\"%s\"\n\r"  // ip_adress, ip_gateway, ip_netmask
#define COMMAND_CONFIG_WIFI_IP_SOFT_AP_SHORT "AT+CIPAP_CUR=\"%s\"\n\r"          // ip_adress

//ESP8266_UART enums
enum Baud_rate
{
    SPEED_4400 = 4400,
    SPEED_9600 = 9600,
    SPEED_115200 = 115200,
    SPEED_4608000 = 4608000
};
enum Data_bits
{
    BIT_5 = 5,
    BIT_6 = 6,
    BIT_7 = 7,
    BIT_8 = 8
};
enum Stop_bits
{
    STOP_BIT_1 = 1,
    STOP_BIT_2 = 3
};
enum Parity
{
    NO_PARITY = 0,
    ODD_PARITY = 1,
    EVEN_PARITY = 2
};
enum Flow_control
{
    NO_FLOW_CONTROL = 0,
    RTS_FLOW_CONTROL = 1,
    CTS_FLOW_CONTROL = 2,
    RTS_CTS_FLOW_CONTROL = 3
};
//ESP8266_UART enums

//ESP8266_WIFI enums
enum Wifi_mode
{
    Station_mode = 1,
    Soft_AP_mode = 2,
    Station_Soft_AP_mode = 3
};

//Soft_AP enums
enum Wifi_security
{ 
    OPEN = 0,
    WPA_PSK = 1,
    WPA2_PSK = 2,
    WPA_WPA2_PSK = 3
};
//Soft_AP enums

//ESP8266_WIFI enums

//base class for ESP8266 modules
class ESP8266
{
public:
    ESP8266() {}
    virtual void Begin() = 0;
protected:
    void Read_UART(char *buf)
    {
        //enter function, which read data from UART interface
    }
    void Transmit_UART(char *buf, uint8_t size)
    {
        //enter function, which transmit data in UART interface
    }
    virtual void Init_var() = 0;
    virtual ~ESP8266(){};
};

class ESP8266_UART: public ESP8266 
{
public:
    ESP8266_UART();
    ESP8266_UART(Baud_rate baudrate, Data_bits databits, Stop_bits stopbits, Parity parity, Flow_control flowcontrol);
    void Set_UART_Config(Baud_rate baudrate, Data_bits databits, Stop_bits stopbits, Parity parity, Flow_control flowcontrol);
    void Begin() override;
    ~ESP8266_UART() override;
private:
    Baud_rate *baudrate;
    Data_bits *databits;
    Stop_bits *stopbits;
    Parity *parity;
    Flow_control *flowcontrol;
    uint8_t Get_UART_Config(char *buf);
    void Init_var() override;
};

class ESP8266_WIFI: virtual public ESP8266
{
public:
    ESP8266_WIFI();
    void Set_IP_Adress(const char ip[]);
    void Set_IP_Adress(const char ip[], const char ip_gateway[], const char ip_netmask[]);
    void Set_MAC_Adress(const char mac[]);
    virtual void Set_WIFI(const char ssid[], const char password[]);
    ~ESP8266_WIFI() override;
protected:
    char *ssid;
    char *password;
    char *ip;
    char *ip_gateway;
    char *ip_netmask;
    char *mac;
    uint8_t Get_WIFI_Mode(char *buf, Wifi_mode wifimode);
    virtual uint8_t Get_IP_Adress(char *buf) = 0;
    virtual uint8_t Get_MAC_Adress(char *buf) = 0;
    void Init_var() override;
};

class Soft_AP: public ESP8266_WIFI
{
public:
    Soft_AP();
    void Begin() override;
    ~Soft_AP();
protected:
    Wifi_security *wifisecurity;
    uint8_t *channel;
    uint8_t Get_IP_Adress(char *buf) override;
    uint8_t Get_MAC_Adress(char *buf) override;
    void Init_var() override;
};
