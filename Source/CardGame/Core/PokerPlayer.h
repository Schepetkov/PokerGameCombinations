#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameDesk.h"
#include "PokerPlayer.generated.h"

UCLASS()
class CARDGAME_API APokerPlayer : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FCard> PlayerCards;
	
	EPokerCombinations PlayerCombination = EPokerCombinations::Default;
	int32 CombinationRang {0};

public:
	void GivingCards(const TArray<FCard>& InPlayerCards);
	void DeterminePokerCombination();

protected:
	void SortPlayerCards();

};
