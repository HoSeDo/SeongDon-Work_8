#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "InputActionValue.h"
#include "PawnCharactor.generated.h"

UCLASS()
class SPARTAPROJECT_API APawnCharactor : public APawn
{
	GENERATED_BODY()

public:
	APawnCharactor();

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CapsuleComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere)
    UArrowComponent* DirectionArrow;

    // 에디터에서 할당할 입력 에셋들
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* DefaultContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotationSpeed = 90.0f; // 초당 회전 각도

    // 입력 처리 함수
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

};
