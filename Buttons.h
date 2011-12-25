#ifndef SB_BUTTONS
#define SB_BUTTONS

/*********************************************************************************
 *   SteelBattalion.X - A library to access the Steel Battalion XBox             *
 *   controller.  Written by Joseph Coutcher. Ported to C++ by Fred Hallock      *
 *                                                                               *
 *   This file is part of SteelBattalion.X                                       *
 *                                                                               *
 *   SteelBattalion.X is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by        *
 *   the Free Software Foundation, either version 3 of the License, or           *
 *   (at your option) any later version.                                         *
 *                                                                               *
 *   SteelBattalion.X is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 *   GNU General Public License for more details.                                *
 *                                                                               *
 *   You should have received a copy of the GNU General Public License           *
 *   along with SteelBattalion.X.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                               *
 *   While this code is licensed under the LGPL, please let me know whether      *
 *   you're using it.  I'm always interested in hearing about new projects,      *
 *   especially ones that I was able to make a contribution to...in the form of  *
 *   this library.                                                               *
 *                                                                               *
 *   2010-11-26: JC - Initial commit                                             *
 *   2011-11-12: FH - Initial commit						 *
 *                                                                               *
 *********************************************************************************/

namespace SteelBattalion
{
	enum ControllerLEDEnum
	{	EmergencyEject = 4,			//	 4
		CockpitHatch,				//	 5
		Ignition,				//	 6
		Start,					//	 7
		OpenClose,				//	 8
		MapZoomInOut,				//	 9
		ModeSelect,				//	10
		SubMonitorModeSelect,			//	11
		MainMonitorZoomIn,			//	12
		MainMonitorZoomOut,			//	13
		ForecastShootingSystem,			//	14
		Manipulator,				//	15
		LineColorChange,			//	16
		Washing,				//	17
		Extinguisher,				//	18
		Chaff,					//	19
		TankDetach,				//	20
		Override,				//	21
		NightScope,				//	22
		F1LED,					//	23
		F2LED,					//	24
		F3LED,					//	25
		MainWeaponControl,			//	26
		SubWeaponControl,			//	27
		MagazineChange,				//	28
		Comm1,					//	29
		Comm2,					//	30
		Comm3,					//	31
		Comm4,					//	32
		Comm5,					//	33

		GearR = 35,				//	35
		GearN,					//	36
		Gear1,					//	37
		Gear2,					//	38
		Gear3,					//	39
		Gear4,					//	40
		Gear5					//	41
	};

	enum ButtonEnum
	{
		RIGHT_JOY_MAIN_WEAPON,			//	 0
		RIGHT_JOY_FIRE,				//	 1
		RIGHT_JOY_LOCK_ON,			//	 2
		COCKPIT_HATCH,				//	 3
		IGNITION,				//	 4
		START,					//	 5
		EJECT,					//	 6
		MULTIMON_OPEN_CLOSE,			//	 7
		MULTIMON_MAP_ZOOM_IN_OUT,		//	 8
		MULTIMON_MODE_SELECT,			//	 9
		MULTIMON_SUB_MONITOR,			//	10
		MAINMON_ZOOM_IN,			//	11
		MAINMON_ZOOM_OUT,			//	12
		WASHING,				//	13
		EXTINGUISHER,				//	14
		CHAFF,					//	15
		WEAPON_CON_MAIN,			//	16
		WEAPON_CON_SUB,				//	17
		WEAPON_CON_MAG,				//	18
		FSS,					//	19
		MANIP,					//	20
		LINE_COLOR_CHANGE,			//	21
		TANK_DETACH,				//	22
		OVERRIDE,				//	23
		NIGHT_SCOPE,				//	24
		F1,					//	25
		F2,					//	26
		F3,					//	27
		COM1,					//	28
		COM2,					//	29
		COM3,					//	30
		COM4,					//	31
		COM5,					//	32
		LEFT_JOY_SIGHT_CHANGE,			//	33
		TOGGLE_FILTER_CONTROL,			//	34
		TOGGLE_OXYGEN_SUPPLY,			//	35
		TOGGLE_FUEL_FLOW_RATE,			//	36
		TOGGLE_BUFFER_MAT,			//	37
		TOGGLE_VT_LOCATION,			//	38
		TUNER_DIAL_STATE_CHANGE,		//	39
		GEAR_LEVER_STATE_CHANGE			//	40
	};

	struct ButtonMask
	{
		int bytePos;
		int maskValue;

		ButtonMask(int bytePos, int maskValue);
	};

	struct ButtonMasks
	{
		ButtonMask *MaskList[41];
		void Init();
		void deInit();
	};

	struct ButtonState
	{
		ButtonEnum button;
		bool currentState;
		bool changed;
	};

	ControllerLEDEnum ButtonToLED(ButtonEnum b);
}
#endif
