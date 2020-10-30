// 2020 Purple Pill Productions.

#include "MyActor.h"
//#include "StaticData.h"

// Sets default values
AMyActor::AMyActor():
	TotalDamage(200),
	DamageTimeInSeconds(1.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaybeLoadStaticData();

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Actor play begins"));
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::PostInitProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Post init properties"));

	Super::PostInitProperties();
	
	CalculateValues();
}

void AMyActor::CalculateValues()
{
	DamagePerSecond = TotalDamage / DamageTimeInSeconds;
}

void AMyActor::PostLoad()
{
	UE_LOG(LogTemp, Warning, TEXT("[MYLOG] Post actor load"));

	MaybeLoadStaticData();

	Super::PostLoad();
}

void AMyActor::MaybeLoadStaticData()
{
	/*if (FSDManager::staticDataLoaded()) {
		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] --------> Setting static data for actor."));
		FTypeData<ESDTypes::type1> instanceData = FSDManager::GetTypeInstanceData<ESDTypes::type1>(1);
		DamageTimeInSeconds = instanceData.prop1;
		TotalDamage = instanceData.prop2;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("[MYLOG] --------> !! Static data was not loaded."));
	}*/
}

#if WITH_EDITOR
void AMyActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateValues();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
