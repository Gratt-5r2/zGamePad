// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {


	void zCDSDevice::InitDevice()
	{
		DeviceCount = JslConnectDevices();
	}

	bool zCDSDevice::CheckConnection()
	{
		bool result = true;
		int *devices = new int[ DeviceCount ];

		JslGetConnectedDeviceHandles( devices, DeviceCount );

		if ( devices == 0 )
			result = false;

		delete[] devices;
		return result;
	}
	
	void zCDSDevice::UpdateState()
	{
		GamepadState = JslGetSimpleState(0);
		if ( DeviceConnected )
		{
			KeyStates = GamepadState.buttons;

			LeftStick.X = GamepadState.stickLX * 32767;
			LeftStick.Y = GamepadState.stickLY * 32767;
			RightStick.X = GamepadState.stickRX * 32767;
			RightStick.Y = GamepadState.stickRY * 32767;
			LeftTrigger = GamepadState.lTrigger * 255;
			RightTrigger = GamepadState.rTrigger * 255;
		}
	}
}