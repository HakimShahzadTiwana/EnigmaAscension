// Fill out your copyright notice in the Description page of Project Settings.


#include "EnigmaAscension/Public/GameplayAbilitySystem/EAAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Player/EACharacter.h"

class AEACharacter;

UEAAttributeSet::UEAAttributeSet()
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::UEAAttributeSet()"));
}

void UEAAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	//UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::PostGameplayEffectExecute"));
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.0f,GetMaxHealth()));
		// AActor* DamagedActor = nullptr;
		// AController* DamagedController = nullptr;
		// if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		// {
		// 	DamagedActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		// 	DamagedController = Data.Target.AbilityActorInfo->PlayerController.Get();
			// if (GetHealth() <= 0)
			// {
			// 	// Handle death with GASCharacter. Note this is just one example of how this could be done.
			// 	if (AEACharacter* GASChar = Cast<AEACharacter>(Data.))
			// 	{
			// 		//TODO: Construct a gameplay cue event for this death
			// 		//For now call function that will play death montage
			// 		GASChar->PlayCharacterDeathMontage();
			// 	}
			// }
		//}
		
	}
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(),0.0f,GetMaxStamina()));
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.0f,GetMaxMana()));
	}
	
   
	    // Get the Target actor
        
    	// // Get the Source actor
    	// AActor* AttackingActor = nullptr;
    	// AController* AttackingController = nullptr;
    	// AController* AttackingPlayerController = nullptr;
    	// if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
    	// {
    	//     AttackingActor = Source->AbilityActorInfo->AvatarActor.Get();
    	//     AttackingController = Source->AbilityActorInfo->PlayerController.Get();
    	//     AttackingPlayerController = Source->AbilityActorInfo->PlayerController.Get();
    	//     if (AttackingController == nullptr && AttackingActor != nullptr)
    	//     {
    	//         if (APawn* Pawn = Cast<APawn>(AttackingActor))
    	//         {
    	//             AttackingController = Pawn->GetController();
    	//         }
    	//     }
    	// }
    
	
}

void UEAAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::GetLifetimeReplicatedProps"));
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,MaxStamina,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,Stamina,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEAAttributeSet,AttackDamage,COND_None,REPNOTIFY_Always);
}


void UEAAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxStamina"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxStamina -  New Max Stamina is %f, Old Max Stamina was : %f"),MaxStamina.GetCurrentValue(),OldMaxStamina.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,MaxStamina,OldMaxStamina);
}

void UEAAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Stamina"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Stamina -  New Stamina is %f, Old Stamina was : %f"),Stamina.GetCurrentValue(),OldStamina.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,Stamina,OldStamina);
}

void UEAAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxHealth"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxHealth -  New Max Health is %f, Old Max Health was : %f"),MaxHealth.GetCurrentValue(),OldMaxHealth.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,MaxHealth,OldMaxHealth);
}

void UEAAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Health"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Health -  New Health is %f, Old Max Stamina was : %f"),Health.GetCurrentValue(),OldHealth.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,Health,OldHealth); 
}

void UEAAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxMana"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_MaxMana -  New Max Stamina is %f, Old Max Stamina was : %f"),MaxMana.GetCurrentValue(),OldMaxMana.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,MaxMana,OldMaxMana);
}

void UEAAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Mana"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_Mana -  New Max Stamina is %f, Old Health was : %f"),Mana.GetCurrentValue(),OldMana.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,Mana,OldMana);
}

void UEAAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_AttackDamage"));
	UE_LOG(LogGAS,Log,TEXT("UEAAttributeSet::OnRep_AttackDamage -  New AttackDamage is %f, Old AttackDamage was : %f"),AttackDamage.GetCurrentValue(),OldAttackDamage.GetCurrentValue());
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEAAttributeSet,AttackDamage,OldAttackDamage);
}

