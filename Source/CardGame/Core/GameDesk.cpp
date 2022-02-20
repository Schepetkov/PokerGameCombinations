#include "CardGame/Core/GameDesk.h"
#include "PokerPlayer.h"

bool FCard::operator == (const FCard& Other) const
{
	return Other.CardSuit == CardSuit && Other.Value == Value;
}

TArray<FCard> AGameDesk::GetCards()
{
	TArray<FCard> PlayersCards;

	for (int32 i = 0; i < 5; i++)
	{
		int32 RandomCardIndex = FMath::RandRange(0, CardDeck.Num() - 1);

		const FCard Card = CardDeck[RandomCardIndex];

		PlayersCards.Add(Card);
		CardDeck.RemoveSingle(Card);
	}

	return PlayersCards;
}

void AGameDesk::RefreshGame()
{
	CardDeck.Empty();
	AllPokerPlayers.Empty();
	
	CardDeck = GetGameData().CardsPack;

	//Spawn Player
	for (int32 i = 0; i < 10; i++)
	{
		AllPokerPlayers.Add(SpawnPokerPlayer());
	}

	//For debug
	//DebugCombinations();

	//Sort by strongest combinations
	APokerPlayer* TempPokerPlayer = nullptr;
	int32 j {0};
	for (int32 i = 0; i < 10 - 1; i++)
	{
		j = i + 1;
		TempPokerPlayer = AllPokerPlayers[j];

		while (j > 0 && TempPokerPlayer->PlayerCombination < AllPokerPlayers[j - 1]->PlayerCombination)
		{
			AllPokerPlayers[j] = AllPokerPlayers[j - 1];
			j--;
		}

		AllPokerPlayers[j] = TempPokerPlayer;
	}

	//Sort by strongest card value
	j = 0;
	for (int32 i = 0; i < 10 - 1; i++)
	{
		for (j = 0; j < 10 - i - 1; j++)
		{
			if (AllPokerPlayers[j]->PlayerCombination == AllPokerPlayers[j + 1]->PlayerCombination)
			{
				if (AllPokerPlayers[j]->PlayerCombination == EPokerCombinations::TwoPair)
				{
					if (IsPlayerATwoPairGreat(AllPokerPlayers[j + 1], AllPokerPlayers[j]))
					{
						AllPokerPlayers.Swap(j, j + 1);
					}
				}
				else if (AllPokerPlayers[j]->CombinationRang < AllPokerPlayers[j + 1]->CombinationRang)
				{
					AllPokerPlayers.Swap(j, j + 1);
				}
			}
		}
	}

	//For debug
	//ShowDebugLog();
}

void AGameDesk::BeginPlay()
{
	Super::BeginPlay();

	RefreshGame();
}

const FDeckCards& AGameDesk::GetGameData() const
{
	FDeckCards* Settings = CardsData.GetRow<FDeckCards>("AGameDesk::GetDeckCardsData");
	check(Settings);
	return *Settings;
}

APokerPlayer* AGameDesk::SpawnPokerPlayer()
{
	APokerPlayer* NewPokerPlayer = nullptr;

	if (GetWorld())
	{
		NewPokerPlayer = GetWorld()->SpawnActor<APokerPlayer>(GetActorLocation(), GetActorRotation());
		if (NewPokerPlayer)
		{
			NewPokerPlayer->GivingCards(GetCards());
		}
	}

	return NewPokerPlayer;
}

FString AGameDesk::GetCombinationName(EPokerCombinations InPokerCombinations)
{
	FString PokerCombinationName = "None";
	
	switch (InPokerCombinations)
	{
	case EPokerCombinations::Default:
		PokerCombinationName = "Default";
		break;
	case EPokerCombinations::RoyalFlush:
		PokerCombinationName = "RoyalFlush";
		break;
	case EPokerCombinations::StraightFlush:
		PokerCombinationName = "StraightFlush";
		break;
	case EPokerCombinations::FourOfKind:
		PokerCombinationName = "FourOfKind";
		break;
	case EPokerCombinations::FullHouse:
		PokerCombinationName = "FullHouse";
		break;
	case EPokerCombinations::Flush:
		PokerCombinationName = "Flush";
		break;
	case EPokerCombinations::Straight:
		PokerCombinationName = "Straight";
		break;
	case EPokerCombinations::ThreeOfKind:
		PokerCombinationName = "ThreeOfKind";
		break;
	case EPokerCombinations::TwoPair:
		PokerCombinationName = "TwoPair";
		break;
	case EPokerCombinations::OnePair:
		PokerCombinationName = "OnePair";
		break;
	case EPokerCombinations::HighCard:
		PokerCombinationName = "HighCard";
		break;
	default:
		break;
	}

	return PokerCombinationName;
}

void AGameDesk::DebugCombinations()
{
	AllPokerPlayers[0]->PlayerCards[0].Value = 1;
	AllPokerPlayers[0]->PlayerCards[1].Value = 2;
	AllPokerPlayers[0]->PlayerCards[2].Value = 3;
	AllPokerPlayers[0]->PlayerCards[3].Value = 4;
	AllPokerPlayers[0]->PlayerCards[4].Value = 5;
	AllPokerPlayers[0]->PlayerCards[0].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[0]->PlayerCards[1].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[0]->PlayerCards[2].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[0]->PlayerCards[3].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[0]->PlayerCards[4].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[0]->DeterminePokerCombination();

	AllPokerPlayers[1]->PlayerCards[0].Value = 10;
	AllPokerPlayers[1]->PlayerCards[1].Value = 10;
	AllPokerPlayers[1]->PlayerCards[2].Value = 11;
	AllPokerPlayers[1]->PlayerCards[3].Value = 11;
	AllPokerPlayers[1]->PlayerCards[4].Value = 11;
	AllPokerPlayers[1]->DeterminePokerCombination();

	AllPokerPlayers[2]->PlayerCards[0].Value = 4;
	AllPokerPlayers[2]->PlayerCards[1].Value = 4;
	AllPokerPlayers[2]->PlayerCards[2].Value = 4;
	AllPokerPlayers[2]->PlayerCards[3].Value = 4;
	AllPokerPlayers[2]->PlayerCards[4].Value = 1;
	AllPokerPlayers[2]->DeterminePokerCombination();

	AllPokerPlayers[3]->PlayerCards[0].Value = 10;
	AllPokerPlayers[3]->PlayerCards[1].Value = 10;
	AllPokerPlayers[3]->PlayerCards[2].Value = 11;
	AllPokerPlayers[3]->PlayerCards[3].Value = 11;
	AllPokerPlayers[3]->PlayerCards[4].Value = 8;
	AllPokerPlayers[3]->DeterminePokerCombination();

	AllPokerPlayers[4]->PlayerCards[0].Value = 1;
	AllPokerPlayers[4]->PlayerCards[1].Value = 2;
	AllPokerPlayers[4]->PlayerCards[2].Value = 3;
	AllPokerPlayers[4]->PlayerCards[3].Value = 4;
	AllPokerPlayers[4]->PlayerCards[4].Value = 5;
	AllPokerPlayers[4]->DeterminePokerCombination();

	AllPokerPlayers[7]->PlayerCards[0].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[7]->PlayerCards[1].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[7]->PlayerCards[2].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[7]->PlayerCards[3].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[7]->PlayerCards[4].CardSuit = ECardSuit::Clovers;
	AllPokerPlayers[7]->DeterminePokerCombination();

	AllPokerPlayers[6]->PlayerCards[0].Value = 9;
	AllPokerPlayers[6]->PlayerCards[1].Value = 10;
	AllPokerPlayers[6]->PlayerCards[2].Value = 11;
	AllPokerPlayers[6]->PlayerCards[3].Value = 12;
	AllPokerPlayers[6]->PlayerCards[4].Value = 13;
	AllPokerPlayers[6]->PlayerCards[0].CardSuit = ECardSuit::Hearts;
	AllPokerPlayers[6]->PlayerCards[1].CardSuit = ECardSuit::Hearts;
	AllPokerPlayers[6]->PlayerCards[2].CardSuit = ECardSuit::Hearts;
	AllPokerPlayers[6]->PlayerCards[3].CardSuit = ECardSuit::Hearts;
	AllPokerPlayers[6]->PlayerCards[4].CardSuit = ECardSuit::Hearts;
	AllPokerPlayers[6]->DeterminePokerCombination();

	AllPokerPlayers[9]->PlayerCards[0].Value = 3;
	AllPokerPlayers[9]->PlayerCards[1].Value = 3;
	AllPokerPlayers[9]->PlayerCards[2].Value = 3;
	AllPokerPlayers[9]->PlayerCards[3].Value = 5;
	AllPokerPlayers[9]->PlayerCards[4].Value = 7;
	AllPokerPlayers[9]->DeterminePokerCombination();
}

void AGameDesk::ShowDebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG] After Sort!"));

	for (APokerPlayer* Player : AllPokerPlayers)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG] Player = %s"), *Player->GetName());
		UE_LOG(LogTemp, Error, TEXT("[DEBUG] Combination = %s"), *GetCombinationName(Player->PlayerCombination));
		for (const FCard& Card : Player->PlayerCards)
		{
			UE_LOG(LogTemp, Error, TEXT("[DEBUG] Card = %d"), Card.Value);
		}
	}
}

bool AGameDesk::IsPlayerATwoPairGreat(const APokerPlayer* PlayerA, const APokerPlayer* PlayerB)
{
	TArray<int32> FirstContainer;
	TArray<int32> SecondContainer;

	for (int32 i = 0; i < 5; i++)
	{
		for (int32 j = i + 1; j < 5; j++)
		{
			if (PlayerA->PlayerCards[i].Value == PlayerA->PlayerCards[j].Value)
			{
				FirstContainer.Add(PlayerA->PlayerCards[i].Value);
			}
			
			if (PlayerB->PlayerCards[i].Value == PlayerB->PlayerCards[j].Value)
			{
				SecondContainer.Add(PlayerB->PlayerCards[i].Value);
			}
		}
	}

	if (FirstContainer.Num() == 2 && SecondContainer.Num() == 2)
	{
		if (FirstContainer[0] == SecondContainer[0] && FirstContainer[1] == SecondContainer[1])
		{
			if (PlayerA->CombinationRang > PlayerB->CombinationRang)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		if (FirstContainer.Last() > SecondContainer.Last())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return false;
}
