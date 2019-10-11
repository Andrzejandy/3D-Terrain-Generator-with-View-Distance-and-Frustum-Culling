#pragma once
#include "Global.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Camera
{
public:
	Camera(void);
	~Camera(void);
private:
	
#ifdef ENVIRONMENT32
	DirectX::XMFLOAT4X4 f_CameraMatrix; 
#endif
	//	    XMStoreFloat4x4(&f_CameraMatrix, CameraMatrix);
	DirectX::XMMATRIX CameraMatrix;	
	DirectX::XMVECTOR XMRight;		// right x
	DirectX::XMVECTOR XMForward;	// lookat forwards z
	DirectX::XMVECTOR XMUp;			// up y

	float yaw;
	float pitch;

	D3DXVECTOR3 CameraPos;
	D3DXVECTOR3 CameraAt;
	DirectX::XMVECTOR XMVecAt;
	DirectX::XMVECTOR XMCamPos;
	DirectX::XMFLOAT4 FrustumPlane[6];

	float speed_m;
	float speed_s;
public:
#ifdef ENVIRONMENT32
	DirectX::XMFLOAT4X4 Camera::GetCamMatrix(void);
#else
	DirectX::XMMATRIX Camera::GetCamMatrix(void);
#endif
	void Camera::SetCameraPos(D3DXVECTOR3 camerapos);
	void Camera::SetCameraPos(float x, float y, float z);
	D3DXVECTOR3 Camera::GetCameraPos(void);
	void Camera::SetCameraAt(D3DXVECTOR3 cameraat);
	void Camera::SetCameraAt(float x, float y, float z);
	D3DXVECTOR3 Camera::GetCameraAt(void);
	D3DXVECTOR3 Camera::GetCameraForward(void);

	void Camera::Update(double frametime, double time);
	void Camera::SetCameraLook(D3DXVECTOR3 MoveVec);
	void Camera::Rot(D3DXVECTOR3 YawVec);

	void Camera::Move(D3DXVECTOR3 movevec, double frametime);
	void Camera::Move(float vel, double frametime);
	void Camera::Strafe(float vel, double frametime);

	void Camera::RotByMouseCords(float x, float y);

	bool Camera::CheckRect(float px, float py, float pz, float radius);

#ifdef ENVIRONMENT64
	void Camera::CreateFrustum(float FarZ ,DirectX::XMMATRIX projection);
#else
	void Camera::CreateFrustum(float FarZ , DirectX::XMFLOAT4X4 projection);
#endif
};

