#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "GameDesk.generated.h"

class APokerPlayer;

UENUM(Blueprintable)
enum class EPokerCombinations : uint8
{
	Default	      UMETA(DisplayName = "Default"),
	RoyalFlush    UMETA(DisplayName = "Royal Flush"),
	StraightFlush UMETA(DisplayName = "Straight Flush"),
	FourOfKind    UMETA(DisplayName = "Four of a Kind"),
	FullHouse     UMETA(DisplayName = "Full House"),
	Flush         UMETA(DisplayName = "Flush"),
	Straight      UMETA(DisplayName = "Straight"),
	ThreeOfKind   UMETA(DisplayName = "Three of a Kind"),
	TwoPair       UMETA(DisplayName = "Two Pair"),
	OnePair       UMETA(DisplayName = "One Pair"),
	HighCard      UMETA(DisplayName = "High Card"),
};

UENUM(Blueprintable)
enum class ECardSuit : uint8
{
	Default	UMETA(DisplayName = "Default"),
	Hearts  UMETA(DisplayName = "Hearts"),
	Tiles   UMETA(DisplayName = "Tiles"),
	Clovers UMETA(DisplayName = "Clovers"),
	Pikes   UMETA(DisplayName = "Pikes")
};

USTRUCT(BlueprintType)
struct FCard
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Value {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECardSuit CardSuit = ECardSuit::Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* CardSuitImage = nullptr;

	bool operator == (const FCard& Other) const;
};

USTRUCT(BlueprintType)
struct FDeckCards : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCard> CardsPack;

};

UCLASS()
class CARDGAME_API AGameDesk : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<APokerPlayer*> AllPokerPlayers;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshGame();

protected:
	virtual void BeginPlay() override;

private:
	TArray<FCard> CardDeck;

	UPROPERTY(EditDefaultsOnly, Category = FirstSetup)
	FDataTableRowHandle CardsData;
	
private:
	TArray<FCard> GetCards();
	APokerPlayer* SpawnPokerPlayer();
	
	UFUNCTION()
	bool IsPlayerATwoPairGreat(const APokerPlayer* PlayerA, const APokerPlayer* PlayerB);
	
	UFUNCTION(BlueprintCallable)
	const FDeckCards& GetGameData() const;

	//Debug
	FString GetCombinationName(EPokerCombinations InPokerCombinations);
	void DebugCombinations();
	void ShowDebugLog();

};
