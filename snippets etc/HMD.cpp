#include <iostream>
#include "HMD.h"

hmdcontroler::hmdcontroler()
{
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
	m_pManager = *DeviceManager::Create();
	m_pHMD = *m_pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	m_pDistortionK = new float[3];

	if (m_pHMD->GetDeviceInfo(&m_hmd)) {
		m_MonitorName = m_hmd.DisplayDeviceName;
		m_EyeDistance = m_hmd.InterpupillaryDistance;
		m_pDistortionK = m_hmd.DistortionK;
	}

	m_pSensor = *m_pHMD->GetSensor();
	if (m_pSensor)
		m_SFusion.AttachToSensor(m_pSensor);

	m_LastEyeYaw = 0.0f;
	m_SFusion.SetGravityEnabled(true);
	m_SFusion.SetYawCorrectionEnabled(true);
}

hmdcontroler::~hmdcontroler()
{
	delete m_pDistortionK;

}

void hmdcontroler::Update()
{
	Quatf hmdOrient = m_SFusion.GetOrientation();
	//float yaw = 0.0f;
	hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&m_EyeYaw, &m_EyePitch, &m_EyeRoll);
	//std::cout << yaw << std::endl;
	//m_EyeYaw += (yaw-m_LastEyeYaw);
	//m_LastEyeYaw = yaw;
}

float hmdcontroler::GetXAngle()
{
	return RadtoDegree(m_EyePitch);
}

float hmdcontroler::GetYAngle()
{
	return RadtoDegree(m_EyeYaw);
}

float hmdcontroler::GetZAngle()
{
	return RadtoDegree(m_EyeRoll);
}

float* hmdcontroler::GetDistortion()
{
	return m_pDistortionK;
}

float hmdcontroler::GetEyeDistance()
{
	return m_EyeDistance;
}

const char* hmdcontroler::GetMonitorName()
{
	return m_MonitorName;
}
