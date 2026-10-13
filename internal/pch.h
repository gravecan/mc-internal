// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <includes.h>
#include <logger/logger.hpp>
#include <minhook/MinHook.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_opengl3.h>
#include <sdk/jvm/jvm.hpp>
#include <managers/classloader/classloader.hpp>
#include <game/classes/classes.hpp>
#include <game/classes/minecraft/minecraft.hpp>
#include <game/classes/log4j/log4j.hpp>
#include <game/classes/entity/entity.hpp>
#include <game/classes/world/world.hpp>
#include <game/hooks/detours/wglswapbuffers.hpp>
#include <cheat/modules/modules.hpp>
#include <core.hpp>


#endif //PCH_H
