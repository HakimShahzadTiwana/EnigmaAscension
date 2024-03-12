// Fill out your copyright notice in the Description page of Project Settings.


#include "EnigmaAscension/Public/Player/EACharacter.h"

#include <Player/EAPlayerState.h>

#include "AbilitySystemComponent.h"
#include "EnigmaAscension/Public/GameplayAbilitySystem/EAAttributeSet.h"
#include "EnigmaAscension/Public/GameplayAbilitySystem/EAGameplayAbility.h"
#include "GameplayAbilitySystem/EAAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagsManager.h"
#include "Core/EAGameMode.h"
#include "Core/EAGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EAPlayerController.h"
#include "SupportItems/EASupportItemBase.h"

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
	// TODO : Moved to player controller so remove this 
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
	// if(IsValid(AbilitySystemComponent) && IsValid(AttributeSet))
	// {
	// 	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	// 	InitializeAttributes();

		/*
		 * //Client needs to bind the inputs
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
		*/
	// }
	// else
	// {
	// 	UE_LOG(LogGAS,Warning,TEXT("AEACharacter::OnRep_PlayerState - AbilitySystemComponent or AttributeSet are null"));
	// }
}

void AEACharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if(IsValid(AbilitySystemComponent) && IsValid(GetController()))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitializeAttributes();

		if(AEAPlayerController* EAController = Cast<AEAPlayerController>(GetController()))
		{
			EAController->PlayerPawn = this;
			EAController->SetupInputComponent();
			EAController->BindGasInputs();
		}
	
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
	if(OnAttributeChangeData.NewValue<=0 && !bIsDead)
	{
		bIsDead=true;
		if(!GetWorld()->IsNetMode(NM_Client))
		{
			if(OnAttributeChangeData.GEModData)
			{
				AEACharacter* TargetChar = Cast<AEACharacter>(OnAttributeChangeData.GEModData->Target.GetOwnerActor());
				AEACharacter* InstigatorChar = Cast<AEACharacter>(OnAttributeChangeData.GEModData->EffectSpec.GetContext().GetInstigator());
				if(IsValid(TargetChar))
				{
					bool MyTeam = Cast<AEAPlayerState>(TargetChar->GetPlayerState())->bIsTeamA;
					AEAGameState* GameState = Cast<AEAGameState>(UGameplayStatics::GetGameState(GetWorld()));
					UE_LOG(LogGameMode, Log, TEXT("%hs - Incrementing team score"), __FUNCTION__);
					InstigatorChar->GetPlayerState<AEAPlayerState>()->Stats.MatchScore++;
					UE_LOG(LogGAS, Log, TEXT("%hs - Updated Player score to %d"), __FUNCTION__,InstigatorChar->GetPlayerState<AEAPlayerState>()->Stats.MatchScore);
					GameState->Server_IncrementTeamScore(MyTeam);
				}
				else
				{
					UE_LOG(LogGAS, Warning, TEXT("Cast To targetChar failed"));
				}
			}
		}
		PlayCharacterDeathMontage();
	}
	if(AEAPlayerController* EA_Controller = Cast<AEAPlayerController>(GetController()); IsValid(Controller)){
		EA_Controller->Client_UpdateHealthUI(OnAttributeChangeData.NewValue,AttributeSet->GetMaxHealth());
	}
}

void AEACharacter::OnStaminaChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	// UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnStaminaChanged"));
	if(AEAPlayerController* EA_Controller = Cast<AEAPlayerController>(GetController()); IsValid(Controller)){
		EA_Controller->Client_UpdateStaminaUI(OnAttributeChangeData.NewValue,AttributeSet->GetMaxStamina());
	}
}

void AEACharacter::OnManaChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	// UE_LOG(LogGAS,Log,TEXT("AEACharacter::OnManaChanged"));
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

void AEACharacter::ApplyEffectToSelf_Implementation(TSubclassOf<UGameplayEffect> Effect)
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
	//DrawDebugSphere(GetWorld(),SocketLocation,AttackRadius,12,FColor::Blue,false,5,0,2);
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
		
		
		AEAGameMode* GameMode = Cast<AEAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		int PlayerInd = Cast<AEAPlayerState>(GetPlayerState())->GetPlayerIndex();
		UE_LOG(LogGAS, Log, TEXT("%hc PlayerIndex is : %d "),*__FUNCTION__,PlayerInd);
		for (int i=0 ; i<GameMode->InputBuffer.Num();i++)
		{
			if (GameMode->InputBuffer[i].PlayerInputID.InstigatorControllerID == PlayerInd )
			{
				int TargetInd = Cast<AEAPlayerState>(Cast<AEACharacter>(OverlappedActors[0])->GetPlayerState())->GetPlayerIndex();
				GameMode->InputBuffer[i].TargetControllerID = TargetInd;
				GameMode->Rollback(PlayerInd,TargetInd);
				// GameMode->PrintBufferSnapShot();
			}
		}
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
	//TODO : Fix collision hits when dead
	//SetActorEnableCollision(false);
	float DeathTime = PlayAnimMontage(DeathMontage);
	FTimerHandle Death_TimerHandle;
	GetWorldTimerManager().SetTimer(Death_TimerHandle,this,&AEACharacter::HideCharacterOnDeath,DeathTime,false);
}

void AEACharacter::SpawnCharacter()
{
	InitializeAttributes();
	bIsDead=false;
	//SetActorEnableCollision(true);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
	SetActorLocation(FVector(-12090.000000,-16200.000000,70.000000));
	this->SetActorHiddenInGame(false);
}

void AEACharacter::SkipAttackAnimation(UAnimMontage* AnimMontage,float ping)
{
	if(isAnimPlaying)
	{
		AnimMontage->BlendOut = ping;
	}
	
	
}

void AEACharacter::RollbackAnimation(EEAAbilityInput InputType,float ping)
{
	switch(InputType)
	{
		
	case EEAAbilityInput::PrimaryAttack:

		 SkipAttackAnimation(PrimaryAttackMontage,ping);
		 break;

	case EEAAbilityInput::SecondaryAttack:

		 SkipAttackAnimation(SecondaryAttackMontage,ping);
		 break;

	case EEAAbilityInput::UltimateAbility:
		
		SkipAttackAnimation(UltimateAbilityMontage,ping);
		break;
		
	case EEAAbilityInput::SimpleAbility:

		SkipAttackAnimation(SimpleAbilityMontage,ping);
		break;

	default:
		break;
	}
}

void AEACharacter::Server_PickupSupportItem_Implementation(AEASupportItemBase* Item)
{
	Item->HideAndCooldownSupportItem();
}

