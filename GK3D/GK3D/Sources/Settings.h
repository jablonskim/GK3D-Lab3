#pragma once

#include "Settings.h"

class Settings
{
public:
	static const int OpenGlMinVersionMajor = 3;
	static const int OpenGlMinVersionMinor = 3;

	static const bool Fullscreen = true;
	static const bool WindowResizable = false;
	static constexpr char* WindowTitle = "GK3D";

	static constexpr int MessageBufferSize = 1024;
	static constexpr int NumInputKeys = 1024;


	static const int ScreenWidth = 1280;
	static const int ScreenHeight = 720;

	static constexpr float FieldOfView = 45.0f;
	static constexpr float PerspectiveNear = .01f;
	static constexpr float PerspectiveFar = 1000.f;


	static const int MoveUpKey = GLFW_KEY_UP;
	static const int MoveDownKey = GLFW_KEY_DOWN;
	static const int MoveLeftKey = GLFW_KEY_A;
	static const int MoveRightKey = GLFW_KEY_D;
	static const int MoveForwardKey = GLFW_KEY_W;
	static const int MoveBackwardKey = GLFW_KEY_S;

	static const int FlashlightSwitchKey = GLFW_KEY_L;

	static const int TextureChangeKey = GLFW_KEY_T;
	static const int MultisamplingKey = GLFW_KEY_M;

	static const int FogSwitchKey = GLFW_KEY_F;
	static const int FogIncKey = GLFW_KEY_KP_ADD;
	static const int FogDecKey = GLFW_KEY_KP_SUBTRACT;

	static const int BlurLimitDecKey = GLFW_KEY_LEFT_BRACKET;
	static const int BlurLimitIncKey = GLFW_KEY_RIGHT_BRACKET;

	static constexpr float MovementSpeed = .7f;
	static constexpr float MouseSensitivityX = .1f;
	static constexpr float MouseSensitivityY = .1f;

	static const bool InvertMouseX = false;
	static const bool InvertMouseY = false;

	static const bool MoveToLookDirection = true;

	
	static const int TerrainSize = 200;
	static constexpr float TerrainScaleFactor = 10.f;

	static constexpr float AmbientStrength = 0.2f;
	static constexpr float SpecularStrength = 0.9f;
	static const int SpecularShininess = 128;

	static const int PointLightsCount = 2;
	static const int SpotLightsCount = 1;
	static const int TexturesCount = 2;

	static constexpr float AmbientR = 1.0f;
	static constexpr float AmbientG = 1.0f;
	static constexpr float AmbientB = 1.0f;

	static constexpr float SpotLightAngle = 12.f;
	static constexpr float SpotLightSoft = 4.f;

	static constexpr float SpotLightR = 1.0f;
	static constexpr float SpotLightG = 1.0f;
	static constexpr float SpotLightB = 1.0f;

	static constexpr float PointLightR = 1.0f;
	static constexpr float PointLightG = 1.0f;
	static constexpr float PointLightB = 1.0f;

	static constexpr float SpotLightConstantFactor = 1.0f;
	static constexpr float SpotLightLinearFactor = 0.2f;
	static constexpr float SpotLightQuadraticFactor = 0.08f;

	static constexpr float PointLightConstantFactor = 1.0f;
	static constexpr float PointLightLinearFactor = 0.35f;
	static constexpr float PointLightQuadraticFactor = 0.44f;

	static const int FogIntensityMax = 2000;
	static const int FogIntensityMin = 100;
	static const int FogIntensityDefault = 200;
	static const int FogIntensityStep = 5;
	static constexpr float FogBrightness = 0.1f;

	static const int GaussianBlurBrightnessStep = 5;
	static const int GaussianBlurBrightnessDefault = 200;

	static const int MultisamplingSamplesCount = 4;


	static constexpr char* VertexShaderPath = "Shaders/VertexShader.glsl";
	static constexpr char* FragmentShaderPath = "Shaders/FragmentShader.glsl";

	static constexpr char* VertexShader2DPath = "Shaders/VertexShader2D.glsl";
	static constexpr char* FragmentShader2DPath = "Shaders/FragmentShader2D.glsl";

	static constexpr char* VertexShaderSkyboxPath = "Shaders/VertexShaderSkybox.glsl";
	static constexpr char* FragmentShaderSkyboxPath = "Shaders/FragmentShaderSkybox.glsl";

	static constexpr char* VertexShaderEnvPath = "Shaders/VertexShaderEnv.glsl";
	static constexpr char* FragmentShaderEnvPath = "Shaders/FragmentShaderEnv.glsl";

	static constexpr char* VertexShaderSSAOGeometryPath = "Shaders/SSAOGeometryVertexShader.glsl";
	static constexpr char* FragmentShaderSSAOGeometryPath = "Shaders/SSAOGeometryFragmentShader.glsl";

	static constexpr char* VertexShaderSSAOOcclusionPath = "Shaders/SSAOOcclusionVertexShader.glsl";
	static constexpr char* FragmentShaderSSAOOcclusionPath = "Shaders/SSAOOcclusionFragmentShader.glsl";

	static constexpr char* VertexShaderSSAOBlurPath = "Shaders/SSAOBlurVertexShader.glsl";
	static constexpr char* FragmentShaderSSAOBlurPath = "Shaders/SSAOBlurFragmentShader.glsl";

	static constexpr char* VertexShaderSSAOLightingPath = "Shaders/SSAOLightingVertexShader.glsl";
	static constexpr char* FragmentShaderSSAOLightingPath = "Shaders/SSAOLightingFragmentShader.glsl";

	static constexpr char* LampModelPath = "Models/lamp/Fl_l_1.3DS";
	static constexpr char* TreeModelPath = "Models/fir1_3ds/firtree1.3ds";
	static constexpr char* BenchModelPath = "Models/bench/bench_v01.obj";

	static constexpr char* CubeTexturePath = "Textures/metal.jpg";
	static constexpr char* BaseGrassTexture = "Textures/grass2.jpg";
	static constexpr char* ExtraGrassTexture = "Textures/grass1.jpg";
	static constexpr char* LeavesTexture = "Textures/leaves.png";

	static constexpr char* SkyboxBasePath = "Textures/grimmnight";
	static constexpr char* SkyboxExtension = ".bmp";

	static constexpr char* ShaderProjectionMatrixLocationName = "projection_matrix";
	static constexpr char* ShaderViewMatrixLocationName = "view_matrix";
	static constexpr char* ShaderModelMatrixLocationName = "model_matrix";
	static constexpr char* ShaderNormalMatrixLocationName = "normal_matrix";

	static constexpr char* ShaderModelColorLocationName = "model_color";
	static constexpr char* ShaderAmbientColorLocationName = "ambient_color";
	static constexpr char* ShaderCameraPosLocationName = "camera_position";

	static constexpr char* ShaderAmbientStrengthLocationName = "ambient_strength";
	static constexpr char* ShaderSpecularStrengthLocationName = "specular_strength";
	static constexpr char* ShaderSpecularShininessLocationName = "specular_shininess";
	
	static constexpr char* ShaderLightPositionLocationName = "position";
	static constexpr char* ShaderLightColorLocationName = "color";
	static constexpr char* ShaderLightConstLocationName = "constant_factor";
	static constexpr char* ShaderLightLinearLocationName = "linear_factor";
	static constexpr char* ShaderLightQuadraticLocationName = "quadratic_factor";

	static constexpr char* ShaderSpotLightsArrayLocationName = "spot_lights";
	static constexpr char* ShaderSpotLightDirectionLocationName = "direction";
	static constexpr char* ShaderSpotLightInnerAngleLocationName = "inner_cut_angle";
	static constexpr char* ShaderSpotLightOuterAngleLocationName = "outer_cut_angle";

	static constexpr char* ShaderPointLightsArrayLocationName = "point_lights";

	static constexpr char* ShaderFogOnLocationName = "fog_on";
	static constexpr char* ShaderFogIntensityLocationName = "fog_intensity";
	static constexpr char* ShaderFogBrightnessLocationName = "fog_brightness";

	static constexpr char* ShaderTexturesLocationName = "textures";
	static constexpr char* ShaderNumTexturesLocationName = "num_textures";

	static constexpr char* Shader2DScreenTextureLocationName = "screen_texture";
	static constexpr char* Shader2DClipDirectionLocationName = "clip_direction";
	static constexpr char* Shader2DClipOffsetLocationName = "clip_offset";

	static constexpr char* ShaderSkyboxLocationName = "skybox";
	static constexpr char* Shader2DBlurLimitLocationName = "blur_brightness_limit";

	static constexpr char* ShaderSSAOPositionDepthBufferName = "position_depth_buffer";
	static constexpr char* ShaderSSAONormalBufferName = "normal_buffer";
	static constexpr char* ShaderSSAOColorBufferName = "color_buffer";
	static constexpr char* ShaderSSAONoiseBufferName = "noise_buffer";

private:
	Settings() = delete;
	Settings(const Settings &) = delete;
	const Settings & operator=(const Settings &) = delete;
};

