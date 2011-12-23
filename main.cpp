#include "SteelBattalion.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
using namespace std;
using namespace SteelBattalion;

int test = 4;

int main()
{
	SteelBattalionController device;
	bool running = true;
	if(!device.getError())
	{
		while(running)
		{
			switch(test)
			{
				case 1:
				{
					//Test stuff
					ButtonEnum group1[] = {COM1, COM4, F1, MANIP, NIGHT_SCOPE, WASHING, WEAPON_CON_MAG, MULTIMON_OPEN_CLOSE, MAINMON_ZOOM_OUT, EJECT, START};
					ButtonEnum group2[] = {COM2, COM5, TANK_DETACH, F2, LINE_COLOR_CHANGE, EXTINGUISHER, WEAPON_CON_MAIN, MAINMON_ZOOM_IN, MULTIMON_SUB_MONITOR, IGNITION};
					ButtonEnum group3[] = {COM3, FSS, OVERRIDE, F3, CHAFF, WEAPON_CON_SUB, MULTIMON_MODE_SELECT, MULTIMON_MAP_ZOOM_IN_OUT, COCKPIT_HATCH};
					int groupCount[] = {11, 10, 9};
					while(!device.getButtonState(EJECT))
					{
						for(int i = 0; (i < 45)&&(!device.getButtonState(EJECT)); ++i)
						{
							int light = 0;
							if(i < 16)
								light = i;
							else if(i < 31)
								light = 31 - i;
							for(int j = 0; j < groupCount[0]; ++j)
								device.setLEDState(ButtonToLED(group1[j]), light, false);
							device.setLEDState(GearN, light, false);
							device.setLEDState(Gear3, light, false);
							if((i > 15)&&(i < 31))
								light = i - 15;
							else if(i > 30)
								light = 45 - i;
							else
								light = 0;
							for(int j = 0; j < groupCount[1]; ++j)
								device.setLEDState(ButtonToLED(group2[j]), light, false);
							device.setLEDState(Gear1, light, false);
							device.setLEDState(Gear4, light, false);
							if(i < 16)
							light = 31 - i;
							else if(i > 30)
								light = i - 31;
							else
								light = 0;
							for(int j = 0; j < groupCount[2]; ++j)
								device.setLEDState(ButtonToLED(group3[j]), light, false);
							device.setLEDState(Gear5, light, false);
							device.setLEDState(Gear2, light, true);
							device.setLEDState(GearR, light, true);
							usleep(10000);
						}
					}
					device.resetLEDs();
					test++;
					break;
				}
				case 2:
				{
					ButtonEnum buttons[] = {COM1, COM2, COM3, COM4, COM5, F1, TANK_DETACH, FSS, F2, OVERRIDE, MANIP, F3, NIGHT_SCOPE, LINE_COLOR_CHANGE,
								WASHING, EXTINGUISHER, CHAFF, WEAPON_CON_MAIN, WEAPON_CON_SUB, WEAPON_CON_MAG, MULTIMON_OPEN_CLOSE, MULTIMON_MAP_ZOOM_IN_OUT, MULTIMON_MODE_SELECT,
								MULTIMON_SUB_MONITOR, MAINMON_ZOOM_IN, MAINMON_ZOOM_OUT, EJECT, COCKPIT_HATCH, IGNITION, START};
					for(int i = 0; i < 30; ++i)
					{
						int intensity = 0;
						bool up = true;
						while(!device.getButtonState(buttons[i]))
						{
							if(up)
								++intensity;
							else
								--intensity;
							if(intensity == 15)
								up = false;
							if(intensity == 0)
								up = true;
							device.setLEDState(ButtonToLED(buttons[i]), intensity, true);
							usleep(10000);
						}
						device.resetLEDs();
					}
					test++;
					break;
				}
				case 3:
				{
//					unsigned char* buffer = NULL;
					ButtonEnum group1[] = {COM1};
					ButtonEnum group2[] = {COM2};
					ButtonEnum group3[] = {COM3};
					ButtonEnum group4[] = {COM4};
					ButtonEnum group5[] = {COM5};
					ButtonEnum group6[] = {WASHING};
					ButtonEnum group7[] = {WEAPON_CON_MAIN};
					ButtonEnum group8[] = {EXTINGUISHER};
					ButtonEnum group9[] = {WEAPON_CON_SUB};
					ButtonEnum group10[] = {CHAFF};
					ButtonEnum group11[] = {WEAPON_CON_MAG};
					ButtonEnum group12[] = {F1, TANK_DETACH, FSS};
					ButtonEnum group13[] = {F2, OVERRIDE, MANIP};
					ButtonEnum group14[] = {F3, NIGHT_SCOPE, LINE_COLOR_CHANGE};
					ButtonEnum group15[] = {MAINMON_ZOOM_IN, MAINMON_ZOOM_OUT};
					ButtonEnum group16[] = {MULTIMON_MODE_SELECT, MULTIMON_SUB_MONITOR};
					ButtonEnum group17[] = {MULTIMON_OPEN_CLOSE, MULTIMON_MAP_ZOOM_IN_OUT};
					ButtonEnum group18[] = {START};
					ButtonEnum group19[] = {IGNITION};
					ButtonEnum group20[] = {COCKPIT_HATCH};
					ButtonEnum group21[] = {EJECT};
					size_t groupSize[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1};
					for(size_t i = 0; i < 360; ++i)
					{
						int light[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
						if(i < 16)
						{
							light[0] = i;
						}
						else if((1 > 15)&&(i < 31))
						{
							light[1]= i - 15;
							light[0] = 30 - i;
						}
						else if((i > 30)&&(i < 46))
						{
							light[1] = 45 - i;
							light[2] = i - 30;
						}
						else if((i > 45)&&(i < 61))
						{
							light[2] = 60 - i;
							light[3] = i - 45;
						}
						else if((i > 60)&&(i < 76))
						{
							light[3] = 75 - i;
							light[4] = i - 60;
						}
						else if((i > 75)&&(i < 91))
						{
							light[4] = 90 - i;
							light[5] = i - 75;
						}
						else if((i > 90)&&(i < 106))
						{
							light[5] = 105 - i;
							light[6] = i - 90;
						}
						else if((i > 105)&&(i < 121))
						{
							light[6] = 120 - i;
							light[7] = i - 105;
						}
						else if((i > 120)&&(i < 136))
						{
							light[7] = 135 - i;
							light[8] = i - 120;
						}
						else if((i > 135)&&(i < 151))
						{
							light[8] = 150 - i;
							light[9] = i - 135;
						}
						else if((i > 150)&&(i < 166))
						{
							light[9] = 165 - i;
							light[10] = i - 150;
						}
						else if((i > 165)&&(i < 181))
						{
							light[10] = 180 - i;
							light[11] = i - 165;
						}
						else if((i > 180)&&(i < 196))
						{
							light[11] = 195 - i;
							light[12] = i - 180;
						}
						else if((i > 195)&&(i < 211))
						{
							light[12] = 210 - i;
							light[13] = i - 195;
						}
						else if((i > 210)&&(i < 226))
						{
							light[13] = 225 - i;
							light[14] = i - 210;
						}
						else if((i > 225)&&(i < 241))
						{
							light[14] = 240 - i;
							light[15] = i - 225;
						}
						else if((i > 240)&&(i < 256))
						{
							light[15] = 255 - i;
							light[16] = i - 240;
						}
						else if((i > 255)&&(i < 271))
						{
							light[16] = 270 - i;
							light[17] = i - 255;
						}
						else if((i > 270)&&(i < 286))
						{
							light[17] = 285 - i;
							light[18] = i - 270;
						}
						else if((i > 285)&&(i < 301))
						{
							light[18] = 300 - i;
							light[19] = i - 285;
						}
						else if((i > 300)&&(i < 316))
						{
							light[19] = 315 - i;
							light[20] = i - 300;
						}
						else if((i > 315)&&(i < 331))
						{
							light[20] = 330 - i;
						}
						else if((i > 330)&&(i < 346))
						{
							for(size_t j = 0; j < 21; ++j)
								light[j] = i - 330;
						}
						else if((i > 345)&&(i < 361))
						{
							for(size_t j = 0; j < 21; ++j)
								light[j] = 360 - i;
						}

						for(size_t j = 0; j < groupSize[0]; ++j)
							device.setLEDState(ButtonToLED(group1[j]), light[0], false);
						for(size_t j = 0; j < groupSize[1]; ++j)
							device.setLEDState(ButtonToLED(group2[j]), light[1], false);
						for(size_t j = 0; j < groupSize[2]; ++j)
							device.setLEDState(ButtonToLED(group3[j]), light[2], false);
						for(size_t j = 0; j < groupSize[3]; ++j)
							device.setLEDState(ButtonToLED(group4[j]), light[3], false);
						for(size_t j = 0; j < groupSize[4]; ++j)
							device.setLEDState(ButtonToLED(group5[j]), light[4], false);
						for(size_t j = 0; j < groupSize[5]; ++j)
							device.setLEDState(ButtonToLED(group6[j]), light[5], false);
						for(size_t j = 0; j < groupSize[6]; ++j)
							device.setLEDState(ButtonToLED(group7[j]), light[6], false);
						for(size_t j = 0; j < groupSize[7]; ++j)
							device.setLEDState(ButtonToLED(group8[j]), light[7], false);
						for(size_t j = 0; j < groupSize[8]; ++j)
							device.setLEDState(ButtonToLED(group9[j]), light[8], false);
						for(size_t j = 0; j < groupSize[9]; ++j)
							device.setLEDState(ButtonToLED(group10[j]), light[9], false);
						for(size_t j = 0; j < groupSize[10]; ++j)
							device.setLEDState(ButtonToLED(group11[j]), light[10], false);
						for(size_t j = 0; j < groupSize[11]; ++j)
							device.setLEDState(ButtonToLED(group12[j]), light[11], false);
						for(size_t j = 0; j < groupSize[12]; ++j)
							device.setLEDState(ButtonToLED(group13[j]), light[12], false);
						for(size_t j = 0; j < groupSize[13]; ++j)
							device.setLEDState(ButtonToLED(group14[j]), light[13], false);
						for(size_t j = 0; j < groupSize[14]; ++j)
							device.setLEDState(ButtonToLED(group15[j]), light[14], false);
						for(size_t j = 0; j < groupSize[15]; ++j)
							device.setLEDState(ButtonToLED(group16[j]), light[15], false);
						for(size_t j = 0; j < groupSize[16]; ++j)
							device.setLEDState(ButtonToLED(group17[j]), light[16], false);
						for(size_t j = 0; j < groupSize[17]; ++j)
							device.setLEDState(ButtonToLED(group18[j]), light[17], false);
						for(size_t j = 0; j < groupSize[18]; ++j)
							device.setLEDState(ButtonToLED(group19[j]), light[18], false);
						for(size_t j = 0; j < groupSize[19]; ++j)
							device.setLEDState(ButtonToLED(group20[j]), light[19], false);
						for(size_t j = 0; j < groupSize[20]; ++j)
							device.setLEDState(ButtonToLED(group21[j]), light[20], false);
						device.refreshLEDs();
//						buffer = device.getRawLEDData();
//						for(int c = 0; c < 34; ++c)
//							printf("%c", buffer[c]);
						if(i < 331)
							usleep(10000);
						else
						{
//							for(int c = 0; c < 34; ++c)
//								printf("%c", buffer[c]);
							usleep(20000);
						}
					}
					device.resetLEDs();
					test++;
					break;
				}
				case 4:
				{
					unsigned char **animation1 = new unsigned char*[45];
					ifstream file1;
					file1.open("animations/startScreen");
					for(size_t i = 0; i < 45; ++i)
					{
						animation1[i] = new unsigned char[34];
						for(size_t j = 0; j < 34; ++j)
							animation1[i][j] = file1.get();
					}
					file1.close();
					size_t frame = 0;
					while(!device.getButtonState(EJECT))
					{
						device.setRawLEDData(animation1[frame]);
						device.refreshLEDs();
						usleep(10000);
						++frame;
						if(frame > 44)
							frame = 0;
					}
					device.resetLEDs();
					for(size_t i = 0; i < 45; ++i)
						delete [] animation1[i];
					delete [] animation1;
					test++;
					break;
				}
				case 5:
				{
					ifstream file2;
					file2.open("animations/diagtest");
					unsigned char **animation2 = new unsigned char *[389];
					for(size_t i = 0; i < 389; ++i)
					{
						animation2[i] = new unsigned char[34];
						for(size_t j = 0; j < 34; ++j)
							animation2[i][j] = file2.get();
					}
					file2.close();
					for(size_t i = 0; i < 389; ++i)
					{
						device.setRawLEDData(animation2[i]);
						device.refreshLEDs();
						usleep(10000);
					}
					device.resetLEDs();
					for(size_t i = 0; i < 389; ++i)
						delete [] animation2[i];
					delete [] animation2;
					test++;
					break;
				}
/*				case 6:
				{
					device.resetLEDs();
					bool pressed = false;
					while(!pressed)
					{
/*						switch(device.getButtonState(COM1))
						{
							case true:
								device.setLEDState(ButtonToLED(COM1), 15, false);
							default:
								break;
						}
						switch(device.getButtonState(COM2))
						{
							case true:
								device.setLEDState(ButtonToLED(COM1), 15, false);
							default:
								break;
						}
						switch(device.getButtonState(COM3))
						{
							case true:
								device.setLEDState(ButtonToLED(COM1), 15, false);
							default:
								break;
						}
						switch(device.getButtonState(COM4))
						{
							case true:
								device.setLEDState(ButtonToLED(COM1), 15, false);
							default:
								break;
						}
						switch(device.getButtonState(COM5))
						{
							case true:
								device.setLEDState(ButtonToLED(COM1), 15, false);
							default:
								break;
						}
					device.resetLEDs();
					usleep(10000);
					}
					break;
				}
*/				default:
				{
					running = false;
					break;
				}
			}
		}
	}
	return 0;
}
