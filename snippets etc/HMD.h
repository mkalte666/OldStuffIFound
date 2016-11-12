#ifndef HEADER_HMD
#define HEADER_HMD
#include "util.h"
#include "OVR.h"
using namespace OVR;

class hmdcontroler
{
public:
	hmdcontroler();
	~hmdcontroler();

	void Update();
	float GetXAngle();
	float GetYAngle();
	float GetZAngle();

	float* GetDistortion();
	float GetEyeDistance();
	const char* GetMonitorName();
	
private:
	Ptr<DeviceManager>				m_pManager;
	Ptr<HMDDevice>					m_pHMD;
	Ptr<SensorDevice>				m_pSensor;
	HMDInfo							m_hmd;
	SensorFusion					m_SFusion;

	float							*m_pDistortionK;
	float							m_EyeDistance;
	const char*						m_MonitorName;

	float							m_EyeRoll;
	float							m_EyePitch;
	float							m_EyeYaw;
	float							m_LastEyeYaw;
	
};

#endif