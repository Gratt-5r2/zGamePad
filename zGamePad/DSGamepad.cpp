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
		int* devices = new int[DeviceCount];

		JslGetConnectedDeviceHandles(devices, DeviceCount);

		if (devices == 0)
			result = false;

		delete[] devices;
		return result;
	}

	void zCDSDevice::UpdateState()
	{
		GamepadState = JslGetSimpleState(0);
		if (DeviceConnected)
		{
			KeyStates = GamepadState.buttons;

      LeftStick.X  = (int)(GamepadState.stickLX  * 32767.0f);
      LeftStick.Y  = (int)(GamepadState.stickLY  * 32767.0f);
      RightStick.X = (int)(GamepadState.stickRX  * 32767.0f);
      RightStick.Y = (int)(GamepadState.stickRY  * 32767.0f);
      LeftTrigger  = GamepadState.lTrigger * 255.0f;
      RightTrigger = GamepadState.rTrigger * 255.0f;
		}
	}
}