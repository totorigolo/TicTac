// This file is part of TicTac.
//
// TicTac is free software: you can redistribute it and/or modify
//         it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// TicTac is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TicTac.  If not, see <http://www.gnu.org/licenses/>.

#include <Arduino.h>
#include "GY_85.h"
#include <Wire.h>
#include <SPI.h>
#include <TFT_ST7735.h>

#include "Streaming.h"
#include "Setup.h"
#include "Console.h"
#include "Camera.h"

#include "Motor.h"
#include "Pid.h"

GY_85 GY85;

#define __CS1  8
#define __RST  9
#define __DC   10


TFT_ST7735 tft = TFT_ST7735(__CS1, __DC);

#define _BARS     9
#define _BARWIDTH   10
#define _BARSPACE   2

Camera camera(2,3,A0);

/*
   if color = 0 it will create a value sensitive spectrum color
 */
void drawVerticalVU(uint8_t x, uint8_t y, uint8_t w, uint8_t val, uint16_t color) {
	uint8_t h = map(val, 0, 255, 127 - y, 0);
	if (color < 1) color = tft.gradient(map(val, 0, 255, 0, 127));
	tft.fillRect(x, 0, w, h, DARK_GREY);
	if (val > 4) tft.fillRect(x, h + 1, w, 127 - (h + y + 2), color);
}


int16_t min_gy[9];
int16_t max_gy[9];

unsigned long interval = 100;
float fps = interval;
unsigned long tm = millis();
uint8_t pid_input_index=2;
int16_t pid_target_value=-4;

Motor* leftMotor;
Motor* rightMotor;
Pid pid;

void setup()
{
	leftMotor = new Motor('L', 5, 7, 6);
	rightMotor = new Motor('R', 2, 4, 3);
	Serial.begin(57600);

	Input::begin();

	for(int i=0; i<9; i++)
	{
		min_gy[i] =  9999;
		max_gy[i] = -9999;
	}
	// TFT RST
	Wire.begin();
	pinMode(__RST, OUTPUT);
	digitalWrite(__RST,HIGH);
	delay(10);
	tft.begin();

	GY85.init();

	// tft.begin();

  Object::listAll();
	Serial << F("[TicTac] ready!") << endl;
}

void setMotorsPower(int power)
{
	leftMotor->setPower(power);
	rightMotor->setPower(power);
}

void loop()
{
	Console::loop();
	static int16_t gy[9];

	if (millis() >= tm)
	{
		tm = tm + interval;

		static const float avg_rate = 0.99;
		fps = avg_rate*fps + (1-avg_rate)*(float)((long)tm-(long)millis());
		int16_t idx=0;

		// ax ay az
		gy[0] = GY85.accelerometer_x( GY85.readFromAccelerometer() );
		gy[1] = GY85.accelerometer_y( GY85.readFromAccelerometer() );
		gy[2] = GY85.accelerometer_z( GY85.readFromAccelerometer() );

		gy[3] = GY85.compass_x( GY85.readFromCompass() );
		gy[4] = GY85.compass_y( GY85.readFromCompass() );
		gy[5] = GY85.compass_z( GY85.readFromCompass() );

		// Note: ce sont des float
		gy[6] = GY85.gyro_x( GY85.readGyro() );
		gy[7] = GY85.gyro_y( GY85.readGyro() );
		gy[8] = GY85.gyro_z( GY85.readGyro() );
		float gt = GY85.temp  ( GY85.readGyro() );

		/*Serial << "ACC (";
		  for(int i=0; i<9; i++)
		  {
		  if (gy[i] < min_gy[i]) min_gy[i] = gy[i];
		  if (gy[i] > max_gy[i]) max_gy[i] = gy[i];

		  Serial << gy[i] << ' ' << min_gy[i] << '/' << max_gy[i];
		  if (i==2 || i==5)
		  Serial << ") (";
		  else if (i<8)
		  Serial << ", ";
		  }

		  Serial << ") gyro: " << gy[6] << ',' << gy[7] << ',' << gy[8] << " t°:" << gt << endl;*/

		int bar = _BARSPACE;
		for (uint8_t i=0;i<_BARS;i++){
			if (i==3 || i==6)
				bar += _BARSPACE;
			//drawVerticalVU(bar,_BARS,_BARWIDTH,gy[i]+128,0);
			bar += _BARWIDTH + _BARSPACE;
		}


		float pid_input = gy[pid_input_index] - (pid_target_value);
        if (abs(pid_input) > 30) pid.resetIntegral();
		float pid_result = pid.update(pid_input);
		if (settings.pidControlled()) setMotorsPower(pid_result);

		/*
		if (settings.viewPid())
		{
			Serial << F("PID(") << pid_input << ") = " << pid_result << " => ";
			pid.view();
		}
		*/

		Object::viewAll();
		camera.view();

		if (settings.viewFps())
		{
			Serial << F("Avg fps : ") << settings.fps << endl;
		}

	}
}



