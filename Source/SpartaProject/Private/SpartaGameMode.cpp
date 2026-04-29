#include "SpartaGameMode.h"
#include "PawnCharactor.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"

ASpartaGameMode::ASpartaGameMode()
{
    PlayerControllerClass = ASpartaPlayerController::StaticClass();
    DefaultPawnClass = ASpartaCharacter::StaticClass();
    GameStateClass = ASpartaGameState::StaticClass();
}
