// 2020 Purple Pill Productions.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class ACTORTESTPROJ_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category="Damage")
	int32 TotalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageTimeInSeconds;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Damage")
	float DamagePerSecond;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitProperties();

	virtual void CalculateValues();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
};
