#include <stdint.h>
#include <stdio.h>

#define UART_BUF_SIZE 64
#define MAX_SIZE_IP 15
#define MAX_SIZE_MAC 17
#define MAX_SIZE_SSID 64
#define MAX_SIZE_PASSWORD 64

#define COMMAND_CONFIG_UART "AT+UART_CUR=%i,%i,%i,%i,%i\n\r"                    // baud_rate, data_bits, stop_bits, parity, flow_control

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
    void Set_IP_MAC_Adress(const char ip[], const char mac[]);
    void Set_IP_MAC_Adress(const char ip[], const char ip_gateway[], const char ip_netmask[], const char mac[]);
    ~ESP8266_WIFI() override;
protected:
    char *ssid;
    char *password;
    char *ip;
    char *ip_gateway;
    char *ip_netmask;
    char *mac;
    virtual uint8_t Get_IP_Adress(char *buf) = 0;
    virtual uint8_t Get_MAC_Adress(char *buf) = 0;
    void Init_var() override;
};
