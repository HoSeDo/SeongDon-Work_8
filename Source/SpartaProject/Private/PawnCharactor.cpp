#include "PawnCharactor.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APawnCharactor::APawnCharactor()
{
	PrimaryActorTick.bCanEverTick = true;

    // 1. 충돌체 생성 및 루트 설정
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    SetRootComponent(CapsuleComp);
    CapsuleComp->SetCapsuleHalfHeight(88.0f);
    CapsuleComp->SetCapsuleRadius(34.0f);

    // [Physics 설정] 물리 시뮬레이션 비활성화
    CapsuleComp->SetSimulatePhysics(false);

    // 2. 메시 생성 및 부착
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(CapsuleComp);
    MeshComp->SetSimulatePhysics(false); // 메시도 물리 끔

    // 3. 스프링 암 생성 및 부착
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(CapsuleComp);
    SpringArmComp->TargetArmLength = 400.0f; // 카메라 거리
    SpringArmComp->bUsePawnControlRotation = true; // 컨트롤러 회전에 따라 카메라 회전

    // 4. 카메라 생성 및 스프링 암 끝에 부착
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
    DirectionArrow->SetupAttachment(CapsuleComp); // 캡슐에 부착
}

void APawnCharactor::BeginPlay()
{
    Super::BeginPlay();

    // 1. Enhanced Input Subsystem에 IMC 등록
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultContext, 0);
        }
    }
}

void APawnCharactor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawnCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

    APawn::SetupPlayerInputComponent(PlayerInputComponent);

    // 2. 입력 액션과 함수 바인딩
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // MoveAction과 LookAction이 헤더에 선언되어 있어야 합니다.
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawnCharactor::Move);
        }

        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APawnCharactor::Look);
        }
    }
}

// 3. 이동 로직 구현
void APawnCharactor::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    if (Controller != nullptr)
    {
        // 1. 현재 내가 바라보는 방향(월드 기준)을 가져옵니다.
        const FVector Forward = GetActorForwardVector();
        const FVector Right = GetActorRightVector();

        // 2. 입력값과 방향을 곱해 '월드 기준 이동 벡터'를 만듭니다.
        FVector Direction = (Forward * MovementVector.X) + (Right * MovementVector.Y);

        // 이미 Direction에 내 회전 정보가 들어있기 때문에 World로 밀어야 정확합니다.
        AddActorWorldOffset(Direction * MoveSpeed * DeltaTime, true);
    }
}

// 4. 회전 로직 구현
void APawnCharactor::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    if (Controller != nullptr)
    {
        // 1. 좌우 회전 (Yaw): Pawn 전체를 회전시킵니다.
        float NewYaw = LookAxisVector.X * RotationSpeed * DeltaTime;
        AddActorLocalRotation(FRotator(0.0f, NewYaw, 0.0f));

        // 2. 상하 회전 (Pitch): 카메라 지지대(SpringArm)만 회전시킵니다.
        float NewPitch = -LookAxisVector.Y * RotationSpeed * DeltaTime;

        // 현재 SpringArm의 회전을 가져와서 Pitch만 더해줍니다.
        FRotator CurrentArmRotation = SpringArmComp->GetRelativeRotation();
        CurrentArmRotation.Pitch = FMath::Clamp(CurrentArmRotation.Pitch + NewPitch, -60.0f, 60.0f); 

        SpringArmComp->SetRelativeRotation(FRotator(CurrentArmRotation.Pitch, 0.0f, 0.0f));
    }
}

