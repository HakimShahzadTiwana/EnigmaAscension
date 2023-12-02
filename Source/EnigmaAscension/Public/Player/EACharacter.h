// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "EAPlayerController.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "UI/PlayerHUD.h"
#include "EACharacter.generated.h"



class UEAAbilitySystemComponent;
class UEAAttributeSet;
class UEAGameplayAbility;
class UGameplayEffect;
UCLASS()
class ENIGMAASCENSION_API AEACharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEACharacter();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|Tags")
	FGameplayTag PrimaryAttackTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|Tags")
	FGameplayTag SecondaryAttackTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|Tags")
	FGameplayTag SimpleAbilityTag;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|Tags")
	FGameplayTag UltimateAbilityTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GAS|Abilities|AttackRadius")
	float PrimaryAttackRadius=75;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GAS|Abilities|AttackRadius")
	float SecondaryAttackRadius=75;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GAS|Abilities|AttackRadius")
	float SimpleAbilityRadius=75;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GAS|Abilities|AttackRadius")
	float UltimateAbilityRadius=300;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|AttackMontages")
	UAnimMontage* PrimaryAttackMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|AttackMontages")
	UAnimMontage* SecondaryAttackMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|AttackMontages")
	UAnimMontage* SimpleAbilityMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Abilities|AttackMontages")
	UAnimMontage* UltimateAbilityMontage;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|DeathMontage")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="GAS|Abilities|AttackSocket")
	FName AttackSocketName = "Sword_Top";

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS|Components")
	UEAAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS|Components")
	UEAAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="GAS|Defaults")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffects;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="GAS|Defaults")
	TArray<TSubclassOf<class UEAGameplayAbility>> DefaultAbilities;

protected:
	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void HideCharacterOnDeath();
	virtual void GiveDefaultAbilities();

	
	virtual void InitializeAttributes();

	UFUNCTION(Exec)
	virtual void InitAttr();
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	virtual void HandlePrimaryAttack();
	UFUNCTION(BlueprintCallable,Server,Reliable)
	virtual void HandleSecondaryAttack();
	UFUNCTION(BlueprintCallable,Server,Reliable)
	virtual void HandleSimpleAbility();
	UFUNCTION(BlueprintCallable,Server,Reliable)
	virtual void HandleUltimateAbility();
	
	UFUNCTION(BlueprintCallable)
	virtual void SendGameplayEventFromHit(FGameplayTag EventTag,float AttackRadius);
	


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GiveAbilityToSelf(TSubclassOf<UEAGameplayAbility> Ability);
	virtual void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> Effect);
	UFUNCTION(BlueprintCallable)
	virtual void PlayCharacterDeathMontage();
	void SpawnCharacter();
	UPROPERTY(BlueprintReadWrite)
	FInputID CurrentInputID;
	
};


