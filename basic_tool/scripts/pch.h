/**
 * @file pch.h
 * @brief プリコンパイル済みヘッダー
 * @author Matsumoto0628
 * @date 2026-05-03
 */
#pragma once

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// ComPtr
#include <wrl/client.h>

// math
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4x4.h"
#include "rotation_converter.h"
