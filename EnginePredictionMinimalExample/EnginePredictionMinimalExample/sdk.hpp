#pragma once

#include <windows.h>
#include <Psapi.h>
#include <memory>

#include "Vector.hpp"
#include "MoveData.hpp"
#include "BaseEntity.hpp"
#include "UserCmd.hpp"
#include "GlobalVarBase.hpp"
#include "GameMovement.hpp"
#include "MoveHelper.hpp"
#include "Prediction.hpp"
#include "ClientEntityList.hpp"
#include "EngineClient.hpp"

#include "util.hpp"
#include "Hooks.hpp"
#include "PredictionSystem.hpp"

#define IN_ATTACK (1<<0)