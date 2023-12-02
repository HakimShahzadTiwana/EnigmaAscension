// Fill out your copyright notice in the Description page of Project Settings.


#include "EnigmaAscension/Public/Player/EACharacter.h"

#include <Player/EAPlayerState.h>

#include "AbilitySystemComponent.h"
#include "EnigmaAscension/Public/GameplayAbilitySystem/EAAttributeSet.h"
#include "EnigmaAscension/Public/GameplayAbilitySystem/EAGameplayAbility.h"
#include "GameplayAbilitySystem/EAAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Core/EAGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EAPlayerController.h"

// Sets default values
AEACharacter::AEACharacter()
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::AEACharacter()"));
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	AbilitySystemComponent = CreateDefaultSubobject<UEAAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEAAttributeSet>("AttributeSet");

	PrimaryAttackTag = UGameplayTagsManager::Get().RequestGameplayTag("Player.Attack.Primary");
	SecondaryAttackTag = UGameplayTagsManager::Get().RequestGameplayTag("Player.Attack.Secondary");
	SimpleAbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Player.Ability.Simple");
	UltimateAbilityTag = UGameplayTagsManager::Get().RequestGameplayTag("Player.Ability.Ultimate");
}

// Called when the game starts or when spawned
void AEACharacter::BeginPlay()
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::BeginPlay()"));
	Super::BeginPlay();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEACharacter::OnHealthChanged);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue,FString::Printf(TEXT("PrimaryAttackTag is : %s"),*PrimaryAttackTag.ToString()));
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AEACharacter::OnStaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AEACharacter::OnManaChanged);

}

// Called every frame
void AEACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// UE_LOG(LogGAS,Log,TEXT("AEACharacter::SetupPlayerInputComponent()"));
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEACharacter::PossessedBy(AController* NewController)
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::PossessedBy()"));
	Super::PossessedBy(NewController);
	//Server GAS Init
	if(IsValid(AbilitySystemComponent) && IsValid(AttributeSet))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitializeAttributes();
		//Only server should give the abilities
		GiveDefaultAbilities();
	}
	else
	{
		UE_LOG(LogGAS,Warning,TEXT("AEACharacter::PossessedBy - AbilitySystemComponent or AttributeSet are null"));
	}
}

void AEACharacter::OnRep_PlayerState()
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnRep_PlayerState()"));
	Super::OnRep_PlayerState();
	if(IsValid(AbilitySystemComponent) && IsValid(AttributeSet))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitializeAttributes();

		// //Client needs to bind the inputs
		// if(AbilitySystemComponent && InputComponent)
		// {
		// 	const FGameplayAbilityInputBinds Binds("Confirm","Cancel",FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("EEAAbilityInput"))),static_cast<int32>(EEAAbilityInput::Confirm),static_cast<int32>(EEAAbilityInput::Cancel));
		// 	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,Binds);
		// 	UE_LOG(LogGAS,Log,TEXT("AEACharacter::SetupPlayerInputComponent Inputs have been binded"));
		// }
		// else
		// {
		// 	UE_LOG(LogGAS,Warning,TEXT("AEACharacter::OnRep_PlayerState - AbilitySystemComponent or InputComponent are null"));
		// }
	}
	else
	{
		UE_LOG(LogGAS,Warning,TEXT("AEACharacter::OnRep_PlayerState - AbilitySystemComponent or AttributeSet are null"));
	}
}

UAbilitySystemComponent* AEACharacter::GetAbilitySystemComponent() const 
{
	//UE_LOG(LogGAS,Log,TEXT("AEACharacter::GetAbilitySystemComponent()"));
	return AbilitySystemComponent;
}

void AEACharacter::GiveAbilityToSelf(TSubclassOf<UEAGameplayAbility> Ability)
{
	//Spec allows to define level for ability. Have to send the input id for the ability
	FGameplayAbilitySpec AbilitySpec(Ability,1,static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID),this);
	AbilitySystemComponent->GiveAbility(AbilitySpec);
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::GiveAbilities - Ability Given is : %s "),*AbilitySpec.GetDebugString());
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Ability Given is : %s "),*AbilitySpec.GetDebugString()));
}

void AEACharacter::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnHealthChanged"));
	if(OnAttributeChangeData.NewValue<=0)
	{
		PlayCharacterDeathMontage();
	}
	if(AEAPlayerController* EA_Controller = Cast<AEAPlayerController>(GetController()); IsValid(Controller)){
		EA_Controller->Client_UpdateHealthUI(OnAttributeChangeData.NewValue,AttributeSet->GetMaxHealth());
	}
}

void AEACharacter::OnStaminaChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnStaminaChanged"));
	if(AEAPlayerController* EA_Controller = Cast<AEAPlayerController>(GetController()); IsValid(Controller)){
		EA_Controller->Client_UpdateStaminaUI(OnAttributeChangeData.NewValue,AttributeSet->GetMaxStamina());
	}
}

void AEACharacter::OnManaChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnManaChanged"));
	if(AEAPlayerController* EA_Controller = Cast<AEAPlayerController>(GetController()); IsValid(Controller)){
		EA_Controller->Client_UpdateManaUI(OnAttributeChangeData.NewValue,AttributeSet->GetMaxMana());
	}
}

void AEACharacter::HideCharacterOnDeath()
{
	this->SetActorHiddenInGame(true);
	this->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
	//Destroy();
	FTimerHandle RespawnTime;
	GetWorldTimerManager().SetTimer(RespawnTime,this,&AEACharacter::SpawnCharacter,5,false);
}

void AEACharacter::GiveDefaultAbilities()
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::GiveAbilities()"));
	//The server should always give abilities
	if(HasAuthority())
	{
		if(AbilitySystemComponent && !DefaultAbilities.IsEmpty())
		{
			for(auto& StartupAbility : DefaultAbilities)
			{
				
				GiveAbilityToSelf(StartupAbility);
			}
		}
		else
		{
			UE_LOG(LogGAS,Warning,TEXT("AEACharacter::GiveAbilities - AbilitySystemComponent or DefaultAbilities are null"));
		}
	
	}
	
}

void AEACharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> Effect)
{
	//The context in which we're applying the effect, i.e. the instigator, the target etc.
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	//SpecHandle used to define the level of ability to provide
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect,1,EffectContextHandle);

	if(SpecHandle.IsValid())
	{
		//Apply the values(default in this case) using the effect
		FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		UE_LOG(LogGAS,Log,TEXT("AEACharacter::InitializeAttributes - GEHandle Applied Effect Data : %s"),*SpecHandle.Data.Get()->ToSimpleString());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("GEHandle Applied Effect Data : %s"),*SpecHandle.Data.Get()->ToSimpleString()));
	}
	else
	{
		UE_LOG(LogGAS,Warning,TEXT("AEACharacter::InitializeAttributes - Effect Spec Handle is not valid"));
	}
}

void AEACharacter::InitializeAttributes()
{
	UE_LOG(LogGAS,Log,TEXT("AEACharacter::InitializeAttributes()"));
	if(AbilitySystemComponent && !DefaultAttributeEffects.IsEmpty())
	{
		for(auto DefaultEffect : DefaultAttributeEffects )
		{
			ApplyEffectToSelf(DefaultEffect);
		}
	}
	UE_LOG(LogGAS,Warning,TEXT("AEACharacter::InitializeAttributes - AbilitySystemComponent or DefaultAttributeEffects are null"));
	
}

void AEACharacter::InitAttr()
{
	InitializeAttributes();
	FGameplayTagContainer CoolDownTagContainer;
	FGameplayEffectQuery Query;
	Query.MakeQuery_MatchAllEffectTags(CoolDownTagContainer);
	AbilitySystemComponent->RemoveActiveEffects(Query);
}

void AEACharacter::HandlePrimaryAttack_Implementation()
{
	SendGameplayEventFromHit(PrimaryAttackTag,PrimaryAttackRadius);
}

void AEACharacter::HandleSecondaryAttack_Implementation()
{
	SendGameplayEventFromHit(SecondaryAttackTag,SecondaryAttackRadius);
}

void AEACharacter::HandleSimpleAbility_Implementation()
{
	SendGameplayEventFromHit(SimpleAbilityTag,SimpleAbilityRadius);
}

void AEACharacter::HandleUltimateAbility_Implementation()
{
	SendGameplayEventFromHit(UltimateAbilityTag,UltimateAbilityRadius);
}

void AEACharacter::SendGameplayEventFromHit(FGameplayTag EventTag, float AttackRadius)
{
	FVector SocketLocation=GetMesh()->GetSocketLocation(AttackSocketName);
	DrawDebugSphere(GetWorld(),SocketLocation,AttackRadius,12,FColor::Blue,false,5,0,2);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),SocketLocation,AttackRadius,ObjectTypes,AEACharacter::StaticClass(),ActorsToIgnore,OverlappedActors);
	if(!OverlappedActors.IsEmpty())
	{
		FGameplayAbilityTargetDataHandle TargetHandle= UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OverlappedActors[0]);
		FGameplayEventData EventData;
		EventData.Instigator = GetInstigator();
		EventData.Target=OverlappedActors[0];
		EventData.TargetData = TargetHandle;
		// AEAGameMode* GameMode = Cast<AEAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		// int* PlayerStartFrame = GameMode->ClientStartFrame.Find((Cast<AEAPlayerState>(GetPlayerState())->GetPlayerIndex()));
		// TArray<FPlayerInputData>* FrameInputs = GameMode->InputBuffer.Find(*PlayerStartFrame + CurrentInputID.Frame);
		// if(FrameInputs)
		// {
		// 	for( auto Inputs : *FrameInputs)
		// 	{
		// 		UE_LOG(LogCore, Log, TEXT("Searching Array of frame %d "),*PlayerStartFrame + CurrentInputID.Frame);
		// 		Inputs.TargetControllerID = Cast<AEAPlayerState>(Cast<AEACharacter>(OverlappedActors[0])->GetPlayerState())->GetPlayerIndex();
		// 	}
		// 	
		// 	
		// }
		// else
		// {
		// 	UE_LOG(LogCore, Warning, TEXT("Frame not found in buffer for frame %d, Server Frame is currently %d"),*PlayerStartFrame + CurrentInputID.Frame, GameMode->Server_FrameCount);
		// 	
		// }
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(),EventTag,EventData);
	}
	else
	{
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Blue,TEXT("No Hits Found"));	
	}
	
}

void AEACharacter::PlayCharacterDeathMontage()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("Playing Death Montage")));
	float DeathTime = PlayAnimMontage(DeathMontage);
	FTimerHandle Death_TimerHandle;
	GetWorldTimerManager().SetTimer(Death_TimerHandle,this,&AEACharacter::HideCharacterOnDeath,DeathTime,false);
}

void AEACharacter::SpawnCharacter()
{
	InitializeAttributes();
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
	SetActorLocation(FVector(-12090.000000,-16200.000000,70.000000));
	this->SetActorHiddenInGame(false);
}

