
#include "Core/MomentumGameModeBase.h"

#include "Character/MomentumCharacter.h"
#include "Core/MomentumPlayerController.h"
#include "Core/MomentumGameStateBase.h"
#include "Core/MomentumPlayerState.h"
#include "Core/MomentumHUD.h"

AMomentumGameModeBase::AMomentumGameModeBase()
{
	DefaultPawnClass = AMomentumCharacter::StaticClass();
	PlayerControllerClass = AMomentumPlayerController::StaticClass();
	GameStateClass = AMomentumGameStateBase::StaticClass();
	PlayerStateClass = AMomentumPlayerState::StaticClass();
	HUDClass = AMomentumHUD::StaticClass();
}
