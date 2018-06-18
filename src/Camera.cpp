#include "Camera.h"
#include <TFT_ST7735.h>

extern TFT_ST7735 tft;

#if 1
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
	static int16_t y=0;
	y = (y+1) & 127;
	uint8_t* data = readData();
	for(uint8_t x=0; x<128; x++)
	{
		int8_t rgb = data[x];
		int16_t color = tft.Color565(rgb, rgb, rgb);

		tft.drawPixel(x,y,color);
	}
}
#endif
