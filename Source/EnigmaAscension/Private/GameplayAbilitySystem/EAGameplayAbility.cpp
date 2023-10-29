// Fill out your copyright notice in the Description page of Project Settings.


#include "EnigmaAscension/Public/GameplayAbilitySystem/EAGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayAbilitySystem/EAAbilitySystemComponent.h"
#include "Player/EACharacter.h"


UEAGameplayAbility::UEAGameplayAbility()
{
	AbilityInputID = EEAAbilityInput::None;
	AbilityID=EEAAbilityInput::None;
}


void UEAGameplayAbility::ActivateAttackGameplayEvent(UAnimMontage* MontageToPlay, FGameplayTag EventTag)
{
	//TODO (Performance) : Follow BP flow here as well for exec pins and async task.
	UE_LOG(LogGameplayTasks, Log, TEXT("Running Montage"));
	UAbilityTask_PlayMontageAndWait* MontageEvent =UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,"None",MontageToPlay,1,"None",true,1,0);
	MontageEvent->Activate();
	UAbilityTask_WaitGameplayEvent* GameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,EventTag,nullptr,true,true);

	//TODO : EventReceived Delegate Not being called
	GameplayEvent->EventReceived.AddDynamic(this,&UEAGameplayAbility::OnAttackEventReceived);
	GameplayEvent->Activate();
	
}
void UEAGameplayAbility::OnAttackEventReceived(FGameplayEventData Payload)
{
	UE_LOG(LogGAS, Log, TEXT("AttackEventRecieved"));
	if(AEACharacter* TargetCharacter = Cast<AEACharacter>(Payload.Target))
	{
		UE_LOG(LogGAS, Log, TEXT("Applying Gamplay Effect To Self"));
		FGameplayEffectContextHandle EffectContextHandle = TargetCharacter->GetAbilitySystemComponent()->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		//SpecHandle used to define the level of ability to provide
		const FGameplayEffectSpecHandle SpecHandle = TargetCharacter->GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffect,1,EffectContextHandle);

		if(SpecHandle.IsValid())
		{
			//Apply the values(default in this case) using the effect
			FActiveGameplayEffectHandle GEHandle = TargetCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			UE_LOG(LogGAS,Log,TEXT("AEACharacter::InitializeAttributes - GEHandle Applied Effect Data : %s"),*SpecHandle.Data.Get()->ToSimpleString());
		}
	}
	
}

