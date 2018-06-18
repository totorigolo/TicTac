#include "Camera.h"
#include <TFT_ST7735.h>

using namespace ObjectID;

extern TFT_ST7735 tft;

TSL1401::TSL1401(uint8_t clk, uint8_t si, uint8_t aout)
        : Object(ObjectID_t::CAMERA),
          m_si(si), m_clk(clk), m_aout(aout)
{
    pinMode(m_si, OUTPUT);
    pinMode(m_clk, OUTPUT);
    pinMode(m_aout, INPUT);    // analog input
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
    m_time = 170;
}

bool TSL1401::parseInput(char c)
{
    if (c == 's')
    {
        m_time = Input::getInt();
        return true;
    }
    return false;
}

void TSL1401::help() const
{
    Serial << " s# : Change sample speed" << endl;
}

#if 0
void Camera::view()
{
    static int16_t y=0;
    int16_t x=0;
    y = (y+1) & 127;
    if (tft.boundaryCheck(x,y)) return;
    int16_t w = tft.sizeCheck(x,w,tft._width);
    tft.startTransaction();
    tft.setAddrWindow_cont(x, y, x + w - 1, y);

    uint8_t* data = readData();
    for(uint8_t i=0; i<128; i++)
    {
        int16_t rgb = data[i];
        int16_t color = tft.Color565(rgb, rgb, rgb);
        tft.writedata16_cont(color);

    }
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
        tft.writecommand_last(CMD_NOP);
#else
        tft.disableCS();
#endif
    tft.endTransaction();
}
#else

void Camera::view()
{
    static int16_t y = 0U;
    y = (y + 1U) & 127U;
    auto data = readData();
    for (uint8_t x = 0; x < 128; x++)
    {
        uint8_t rgb = data[x];
        uint16_t color = tft.Color565(rgb, rgb, rgb);

        tft.drawPixel(x, y, color);
    }
}

#endif

uint16_t TSL1401::message(Object::Message msg, uint8_t& c)
{
    switch (msg)
    {
    case Message::PARSE_INPUT:
        return uint16_t(parseInput(c));

    case Message::VIEW:
        view();
        break;

    case Message::HELP:
        help();
        break;

    default:
        return uint16_t(false);
    }
    return uint16_t(true);
}

const uint8_t* TSL1401::readData()
{
    static uint8_t m_data[128];

    SI_HIGH
    delayMicroseconds(m_time / 2);
    CLK_HIGH
    delayMicroseconds(m_time / 2);
    SI_LOW
    delayMicroseconds(m_time / 2);
    CLK_LOW
    delayMicroseconds(m_time);

    for (int pixel = 0; pixel < 128; pixel++)
    {
        CLK_HIGH
        m_data[pixel] = analogRead(m_aout);
        delayMicroseconds(m_time);
        CLK_LOW
        delayMicroseconds(m_time);
    }
    // delay(20);
    return &m_data[0];
}
