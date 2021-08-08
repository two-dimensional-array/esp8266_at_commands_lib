#include <stdint.h>

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
