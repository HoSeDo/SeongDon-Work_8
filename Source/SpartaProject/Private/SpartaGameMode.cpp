#include "SpartaGameMode.h"
#include "PawnCharactor.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
    DefaultPawnClass = APawnCharactor::StaticClass();
    PlayerControllerClass = ASpartaPlayerController::StaticClass();
}

