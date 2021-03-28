// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	
    class zCDSDevice {

        JOY_SHOCK_STATE GamepadState;
        WORD KeyStates;
        zTStickState LeftStick;
        zTStickState RightStick;
        float LeftTrigger;
        float RightTrigger;
        bool DeviceConnected;

        int DeviceCount;

    public:

        void InitDevice();
        void UpdateState();
        
        WORD GetKeyState() { return KeyStates; }
        zTStickState GetLeftStick() { return LeftStick; }
        zTStickState GetRightStick() { return RightStick; }
        float GetLeftTrigger() { return LeftTrigger; }
        float GetRightTrigger() { return RightTrigger; }
        bool CheckConnection();

        bool IsConnected() { return DeviceConnected; }
        void SetConnected( bool arg ) { DeviceConnected = arg; }

    };

    zCDSDevice DS4Device;

}