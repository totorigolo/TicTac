#include "Camera.h"
#include <TFT_ST7735.h>



extern TFT_ST7735 tft;

TSL1401::TSL1401(uint8_t clk, uint8_t si, uint8_t aout)
        : Object(CAMERA),
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

Message::Answer TSL1401::parseInput(char c)
{
    if (c == 's')
    {
        m_time = uint8_t(Input::getInt());
        return Message::Processed;
    }
    return Message::Unprocessed;
}

void TSL1401::help() const
{
    Serial << F(" s# : Change sample speed") << endl;
}

#if 0
void TSL1401::view()
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

void TSL1401::view()
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

void TSL1401::message(Message& msg)
{
    switch (msg.type)
    {
    case Message::ParseInput:
        msg.answer = parseInput(msg.c);
        break;

    case Message::View:
        view();
        msg.answer = Message::Processed;
        break;

    case Message::Help:
        help();
        msg.answer = Message::Processed;
        break;

    default:
        msg.answer = Message::Unprocessed;
        break;
    }
}

const uint8_t* TSL1401::readData()
{
    SI_HIGH
    delayMicroseconds(m_time / 2);
    CLK_HIGH
    delayMicroseconds(m_time / 2);
    SI_LOW
    delayMicroseconds(m_time / 2);
    CLK_LOW
    delayMicroseconds(m_time);

    static uint8_t m_data[128];
    for (uint8_t& pixel : m_data)
    {
        CLK_HIGH
        pixel = analogRead(m_aout);
        delayMicroseconds(m_time);
        CLK_LOW
        delayMicroseconds(m_time);
    }
    // delay(20);
    return &m_data[0];
}
