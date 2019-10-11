#include "Camera.h"
#include <DirectXMath.h>
#include <math.h>
#include <iostream>
#define PI 3.14159265358979323846
// ISROT - identift -> scale -> rot -> orbit -> translation
using namespace DirectX;


Camera::Camera(void)
{
	CameraPos.x = 1.0f;
	CameraPos.y = 2.0f;
	CameraPos.z = 1.0f;

	CameraAt.x = 4.0f;
	CameraAt.y = 1.0f;
	CameraAt.z = 4.0f;

	yaw = 0.0f;
	pitch = 0.0f;

	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
	XMVecAt = DirectX::XMVectorSet(CameraAt.x,CameraAt.y,CameraAt.z,0.0f);

	speed_s = 0.0f;
	speed_m = 0.0f;

	XMRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); // right x
	XMForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // lookat forwards z
	XMUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // up y
}

Camera::~Camera(void)
{
}
void Camera::SetCameraPos(D3DXVECTOR3 camerapos)
{
	CameraPos.x = camerapos.x;
	CameraPos.y = camerapos.y;
	CameraPos.z = camerapos.z;
	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
}


void Camera::SetCameraPos(float x, float y, float z)
{
	CameraPos.x = x;
	CameraPos.y = y;
	CameraPos.z = z;
	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
}



D3DXVECTOR3 Camera::GetCameraPos(void)
{
	return CameraPos;
}
D3DXVECTOR3 Camera::GetCameraForward(void)
{
	D3DXVECTOR3 d3dx_At;
	d3dx_At.x = DirectX::XMVectorGetX(XMForward);
	d3dx_At.y = DirectX::XMVectorGetY(XMForward);
	d3dx_At.z = DirectX::XMVectorGetZ(XMForward);
	return d3dx_At;
}
void Camera::SetCameraAt(D3DXVECTOR3 cameraat)
{
	CameraAt.x = cameraat.x;
	CameraAt.y = cameraat.y;
	CameraAt.z = cameraat.z;
	XMVecAt = DirectX::XMVectorSet(CameraAt.x,CameraAt.y,CameraAt.z,0.0f);
}

void Camera::SetCameraAt(float x, float y, float z)
{
	CameraAt.x = x;
	CameraAt.y = y;
	CameraAt.z = z;
	XMVecAt = DirectX::XMVectorSet(CameraAt.x,CameraAt.y,CameraAt.z,0.0f);
}

D3DXVECTOR3 Camera::GetCameraAt(void)
{
	return CameraAt;
}
void Camera::SetCameraLook(D3DXVECTOR3 MoveVec)
{
	CameraAt.x = MoveVec.x;
	CameraAt.y = MoveVec.y;
	CameraAt.z = MoveVec.z;
	XMForward = DirectX::XMVectorSet(CameraAt.x, CameraAt.y, CameraAt.z, 0.0f);
}

#ifdef ENVIRONMENT64
void Camera::CreateFrustum(float FarZ ,DirectX::XMMATRIX projection)
{
	DirectX::XMMATRIX ViewProjection;
	float zFrustumMin;
	float r;
	//Ax + By + Cz + D = 0
	ViewProjection = DirectX::XMMatrixMultiply(CameraMatrix, projection);

	XMFLOAT4X4 pfm;
	XMStoreFloat4x4(&pfm, projection);

	XMFLOAT4X4 pvfm;
	XMStoreFloat4x4(&pvfm, ViewProjection);

	/*
	// Z minimum distance we frustumie.
	zFrustumMin = -pfm._43 / pfm._33;
	r = FarZ / (FarZ - zFrustumMin);
	pfm._33 = r;
	pfm._43 = -r * zFrustumMin;
	*/
	// Top
	FrustumPlane[0].x = pvfm._14 - pvfm._12; 
	FrustumPlane[0].y = pvfm._24 - pvfm._22;
	FrustumPlane[0].z = pvfm._34 - pvfm._32;
	FrustumPlane[0].w = pvfm._44 - pvfm._42;
	XMStoreFloat4(&FrustumPlane[0], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[0]))));
	// Bottom
	FrustumPlane[1].x = pvfm._14 + pvfm._12;
	FrustumPlane[1].y = pvfm._24 + pvfm._22;
	FrustumPlane[1].z = pvfm._34 + pvfm._32;
	FrustumPlane[1].w = pvfm._44 + pvfm._42;
	XMStoreFloat4(&FrustumPlane[1], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[1]))));

	// Left
	FrustumPlane[2].x = pvfm._14 + pvfm._11; 
	FrustumPlane[2].y = pvfm._24 + pvfm._21;
	FrustumPlane[2].z = pvfm._34 + pvfm._31;
	FrustumPlane[2].w = pvfm._44 + pvfm._41;
	XMStoreFloat4(&FrustumPlane[2], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[2]))));

	// XMRight
	FrustumPlane[3].x = pvfm._14 - pvfm._11; 
	FrustumPlane[3].y = pvfm._24 - pvfm._21;
	FrustumPlane[3].z = pvfm._34 - pvfm._31;
	FrustumPlane[3].w = pvfm._44 - pvfm._41;
	XMStoreFloat4(&FrustumPlane[3], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[3]))));

	// Near
	FrustumPlane[4].x = pvfm._14 + pvfm._13;
	FrustumPlane[4].y = pvfm._24 + pvfm._23;
	FrustumPlane[4].z = pvfm._34 + pvfm._33;
	FrustumPlane[4].w = pvfm._44 + pvfm._43;
	XMStoreFloat4(&FrustumPlane[4], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[4]))));

	// Far
	FrustumPlane[5].x = pvfm._14 - pvfm._13; 
	FrustumPlane[5].y = pvfm._24 - pvfm._23;
	FrustumPlane[5].z = pvfm._34 - pvfm._33;
	FrustumPlane[5].w = pvfm._44 - pvfm._43;
	XMStoreFloat4(&FrustumPlane[5], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[5]))));
}
#else

void Camera::CreateFrustum(float FarZ , DirectX::XMFLOAT4X4 fprojection)
{
	DirectX::XMMATRIX ViewProjection;
	DirectX::XMMATRIX projection;
	projection = XMLoadFloat4x4(&fprojection);
//	XMStoreFloat4x4(&fprojection, projection);


	//Ax + By + Cz + D = 0
	ViewProjection = DirectX::XMMatrixMultiply(CameraMatrix, projection);

	XMFLOAT4X4 pfm;
	XMStoreFloat4x4(&pfm, projection);

	XMFLOAT4X4 pvfm;
	XMStoreFloat4x4(&pvfm, ViewProjection);


	// Z minimum distance we frustumie.
	float zFrustumMin = -pfm._43 / pfm._33;
	float r = FarZ / (FarZ - zFrustumMin);
	pfm._33 = r;
	pfm._43 = -r * zFrustumMin;

	// Top
	FrustumPlane[0].x = pvfm._14 - pvfm._12; 
	FrustumPlane[0].y = pvfm._24 - pvfm._22;
	FrustumPlane[0].z = pvfm._34 - pvfm._32;
	FrustumPlane[0].w = pvfm._44 - pvfm._42;
	//FrustumPlane[0] = (XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[0]))), XMVECTOR4);
	XMStoreFloat4(&FrustumPlane[0], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[0]))));
	// Bottom
	FrustumPlane[1].x = pvfm._14 + pvfm._12;
	FrustumPlane[1].y = pvfm._24 + pvfm._22;
	FrustumPlane[1].z = pvfm._34 + pvfm._32;
	FrustumPlane[1].w = pvfm._44 + pvfm._42;
	XMStoreFloat4(&FrustumPlane[1], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[1]))));

	// Left
	FrustumPlane[2].x = pvfm._14 + pvfm._11; 
	FrustumPlane[2].y = pvfm._24 + pvfm._21;
	FrustumPlane[2].z = pvfm._34 + pvfm._31;
	FrustumPlane[2].w = pvfm._44 + pvfm._41;
	XMStoreFloat4(&FrustumPlane[2], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[2]))));

	// XMRight
	FrustumPlane[3].x = pvfm._14 - pvfm._11; 
	FrustumPlane[3].y = pvfm._24 - pvfm._21;
	FrustumPlane[3].z = pvfm._34 - pvfm._31;
	FrustumPlane[3].w = pvfm._44 - pvfm._41;
	XMStoreFloat4(&FrustumPlane[3], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[3]))));

	// Near
	FrustumPlane[4].x = pvfm._14 + pvfm._13;
	FrustumPlane[4].y = pvfm._24 + pvfm._23;
	FrustumPlane[4].z = pvfm._34 + pvfm._33;
	FrustumPlane[4].w = pvfm._44 + pvfm._43;
	XMStoreFloat4(&FrustumPlane[4], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[4]))));

	// Far
	FrustumPlane[5].x = pvfm._14 - pvfm._13; 
	FrustumPlane[5].y = pvfm._24 - pvfm._23;
	FrustumPlane[5].z = pvfm._34 - pvfm._33;
	FrustumPlane[5].w = pvfm._44 - pvfm._43;
	XMStoreFloat4(&FrustumPlane[5], XMPlaneNormalize(XMVECTOR(XMLoadFloat4(&FrustumPlane[5]))));
}
#endif


bool Camera::CheckRect(float px, float py, float pz, float radius)
{
	for (INT i = 0; i < 6; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px - radius), (py - radius), (pz - radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px + radius), (py - radius), (pz - radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px - radius), (py + radius), (pz - radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px + radius), (py + radius), (pz - radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px - radius), (py - radius), (pz + radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px + radius), (py - radius), (pz + radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px - radius), (py + radius), (pz + radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		if (XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&FrustumPlane[i]), XMLoadFloat4(&XMFLOAT4((px + radius), (py + radius), (pz + radius), 1.0f)))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

// eye at up = right forward up
void Camera::RotByMouseCords(float x, float y)
{
	yaw+=x;
	pitch+=y;

	if(pitch>785)
	{
		pitch = 785;
	}
	else if(pitch<-785)
	{
		pitch = -785;
	}
	if(yaw>1570)
	{
		yaw = -1570;
	}
	else if(yaw < -1570)
	{
		yaw = 1570;
	}

	XMRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); // right x
	XMForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // lookat forwards z
	XMUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // up y

	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
	XMVecAt = DirectX::XMVectorSet(CameraAt.x,CameraAt.y,CameraAt.z,0.0f);

	CameraMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch*0.002f, yaw*0.002f, 0.0f);

	XMVecAt = DirectX::XMVector3TransformCoord(XMForward,
		CameraMatrix);
	XMRight = DirectX::XMVector3TransformCoord(XMRight,
		CameraMatrix);

	XMVecAt = DirectX::XMVectorAdd(XMCamPos,XMVecAt);
	XMForward = DirectX::XMVectorSubtract(XMVecAt,XMCamPos);
	XMUp = DirectX::XMVector3Cross(XMForward, XMRight);
	
	CameraAt.x = DirectX::XMVectorGetX(XMVecAt);
	CameraAt.y = DirectX::XMVectorGetY(XMVecAt);
	CameraAt.z = DirectX::XMVectorGetZ(XMVecAt);
}

void Camera::Update(double frametime, double time)
{
	bool forward = true;
	bool right = true;
//	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
//	XMVecAt = DirectX::XMVectorSet(CameraAt.x,CameraAt.y,CameraAt.z,0.0f);
//	XMForward = DirectX::XMVectorSubtract(XMVecAt,XMCamPos);

	if(speed_m < 0.0f)
	{
		forward = false;
		speed_m += frametime * 0.008f;
		if(time >= 0.20)
		{
			speed_m += frametime * 0.8f;
		}
		if(time >= 0.40)
		{
			speed_m += frametime* 8;
		}
	}
	else if(speed_m > 0.0f)
	{
		forward = true;
		speed_m -= frametime * 0.008f;
		if(time >= 0.20)
		{
			speed_m -= frametime* 0.8f;
		}
		if(time >= 0.40)
		{
			speed_m -= frametime * 8;
		}
	}

	if(speed_s < 0.0f)
	{
		right = false;
		speed_s += frametime * 0.008f;
		if(time >= 0.15)
		{
			speed_s += frametime * 0.8f;
		}
		if(time >= 0.30)
		{
			speed_s += frametime * 8;
		}
	}
	else if(speed_s > 0.0f)
	{
		right = true;
		speed_s -= frametime * 0.008f;
		if(time >= 0.15)
		{
			speed_s -= frametime * 0.8f;
		}
		if(time >= 0.30)
		{
			speed_s -= frametime * 8;
		}
	}

	if(speed_m < 0.0f && forward == true)
	{
		speed_m = 0.0f;
	}
	else if (speed_m > 0.0f && forward == false)
	{
		speed_m = 0.0f;
	}

	if(speed_s < 0.0f && right == true)
	{
		speed_s = 0.0f;
	}
	else if(speed_s > 0.0f && right == false)
	{
		speed_s = 0.0f;
	}

	CameraPos.x += speed_m*DirectX::XMVectorGetX(XMForward);
	CameraPos.y += speed_m*DirectX::XMVectorGetY(XMForward);
	CameraPos.z += speed_m*DirectX::XMVectorGetZ(XMForward);
	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);

	CameraAt.x += speed_m*DirectX::XMVectorGetX(XMForward);
	CameraAt.y += speed_m*DirectX::XMVectorGetY(XMForward);
	CameraAt.z += speed_m*DirectX::XMVectorGetZ(XMForward);
	XMVecAt = DirectX::XMVectorSet(CameraAt.x, CameraAt.y, CameraAt.z, 0.0f);


	CameraPos.x += speed_s*DirectX::XMVectorGetX(XMRight);
	CameraPos.y += speed_s*DirectX::XMVectorGetY(XMRight);
	CameraPos.z += speed_s*DirectX::XMVectorGetZ(XMRight);
	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);

	CameraAt.x += speed_s*DirectX::XMVectorGetX(XMRight);
	CameraAt.y += speed_s*DirectX::XMVectorGetY(XMRight);
	CameraAt.z += speed_s*DirectX::XMVectorGetZ(XMRight);
	XMVecAt = DirectX::XMVectorSet(CameraAt.x, CameraAt.y, CameraAt.z, 0.0f);

	CameraMatrix = DirectX::XMMatrixLookAtLH(XMCamPos, XMVecAt, XMUp);
}


#ifdef ENVIRONMENT32
DirectX::XMFLOAT4X4 Camera::GetCamMatrix(void)
{
	XMStoreFloat4x4(&f_CameraMatrix, CameraMatrix);
	return f_CameraMatrix;
}
#else
DirectX::XMMATRIX Camera::GetCamMatrix(void)
{
	return CameraMatrix;
}
#endif
void Camera::Move(D3DXVECTOR3 movevec, double frametime)
{
	double speed;

	speed =	frametime * 0.001;
	if(speed < 0.01f)
	{
		speed = 0.01f;
	}
	else if(speed > 0.05f)
	{
		speed = 0.05f;
	}
	movevec.x = movevec.x * speed;
	movevec.y = movevec.y * speed;
	movevec.z = movevec.z * speed;

	std::cout << movevec.y << std::endl;
	CameraPos.x = CameraPos.x + movevec.x;
	CameraPos.y = CameraPos.y + movevec.y;
	CameraPos.z = CameraPos.z + movevec.z;
	CameraAt.x = CameraAt.x + movevec.x;
	CameraAt.y = CameraAt.y + movevec.y;
	CameraAt.z = CameraAt.z + movevec.z;
	XMCamPos = DirectX::XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 0.0f);
	XMVecAt = DirectX::XMVectorSet(CameraAt.x, CameraAt.y, CameraAt.z, 0.0f);
	XMUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	CameraMatrix = DirectX::XMMatrixLookAtLH(XMCamPos, XMVecAt, XMUp);
}

void Camera::Move(float vel, double frametime)
{
	speed_m = speed_m + vel * frametime * 0.1;
	if(frametime > 0.2f)
	{
		frametime = 0.2f;
	}
}

void Camera::Strafe(float vel, double frametime)
{
	speed_s = speed_s + vel * frametime * 0.2;
	if(frametime > 0.2f)
	{
		frametime = 0.2f;
	}
}