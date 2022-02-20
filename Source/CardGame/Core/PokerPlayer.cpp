#include "CardGame/Core/PokerPlayer.h"
#include "GameDesk.h"

void APokerPlayer::GivingCards(const TArray<FCard>& InPlayerCards)
{
	PlayerCards = InPlayerCards;
	
	SortPlayerCards();
	DeterminePokerCombination();
}

void APokerPlayer::SortPlayerCards()
{
	int32 j{0};

	for (int32 i = 0; i < 5 - 1; i++)
	{
		for (j = 0; j < 5 - i - 1; j++)
		{
			if (PlayerCards[j].Value > PlayerCards[j + 1].Value)
			{
				PlayerCards.Swap(j, j + 1);
			}
		}
	}
}

void APokerPlayer::DeterminePokerCombination()
{
	bool bHasFlush = false;
	bool bStraight = false;

	//check Flush
	if (PlayerCards[0].CardSuit == PlayerCards[1].CardSuit && PlayerCards[0].CardSuit == PlayerCards[2].CardSuit
	 && PlayerCards[0].CardSuit == PlayerCards[3].CardSuit && PlayerCards[0].CardSuit == PlayerCards[4].CardSuit)
	{
		bHasFlush = true;
	}

	//check Straight
	int32 j {0};
	for (int32 i = 0; i < 5; i++)
	{
		if (i > 0 && PlayerCards[i].Value == PlayerCards[i - 1].Value + 1)
		{
			j++;
		}

		if (j == 4)
		{
			bStraight = true;

			//check RoyalFlush && StraightFlush
			if (bHasFlush)
			{
				if (PlayerCards[0].Value == 9 && PlayerCards[4].Value == 13)
				{
					PlayerCombination = EPokerCombinations::RoyalFlush;
					return;
				}

				PlayerCombination = EPokerCombinations::StraightFlush;
				return;
			}
		}
	}

	//check four of a kind
	if (PlayerCards[0].Value == PlayerCards[1].Value && PlayerCards[1].Value == PlayerCards[2].Value && PlayerCards[2].Value == PlayerCards[3].Value
	|| (PlayerCards[1].Value == PlayerCards[2].Value && PlayerCards[2].Value == PlayerCards[3].Value && PlayerCards[3].Value == PlayerCards[4].Value))
	{
		if (PlayerCards[0].Value == PlayerCards[1].Value)
		{
			CombinationRang = PlayerCards[0].Value;
		}
		else
		{
			CombinationRang = PlayerCards[4].Value;
		}

		PlayerCombination = EPokerCombinations::FourOfKind;
		return;
	}

	//check Full House
	if (PlayerCards[0].Value == PlayerCards[1].Value && PlayerCards[3].Value == PlayerCards[4].Value 
	&& (PlayerCards[2].Value == PlayerCards[1].Value || PlayerCards[2].Value == PlayerCards[3].Value))
	{
		if (PlayerCards[0].Value == PlayerCards[2].Value)
		{
			CombinationRang = PlayerCards[0].Value;
		}
		else
		{
			CombinationRang = PlayerCards[2].Value;
		}
		
		PlayerCombination = EPokerCombinations::FullHouse;
		return;
	}

	if (bHasFlush)
	{
		CombinationRang = PlayerCards.Last().Value;
		PlayerCombination = EPokerCombinations::Flush;
		return;
	}
	
	if (bStraight)
	{
		CombinationRang = PlayerCards.Last().Value;
		PlayerCombination = EPokerCombinations::Straight;
		return;
	}

	//check three of kind
	if ((PlayerCards[0].Value == PlayerCards[1].Value && PlayerCards[1].Value == PlayerCards[2].Value) 
	|| (PlayerCards[1].Value == PlayerCards[2].Value && PlayerCards[2].Value == PlayerCards[3].Value)
	|| (PlayerCards[2].Value == PlayerCards[3].Value && PlayerCards[3].Value == PlayerCards[4].Value))
	{
		if (PlayerCards[0].Value == PlayerCards[2].Value)
		{
			CombinationRang = PlayerCards[0].Value;
		}
		else if (PlayerCards[1].Value == PlayerCards[3].Value)
		{
			CombinationRang = PlayerCards[1].Value;
		}
		else if (PlayerCards[3].Value == PlayerCards[4].Value)
		{
			CombinationRang = PlayerCards[3].Value;
		}
		
		PlayerCombination = EPokerCombinations::ThreeOfKind;
		return;
	}
	
	//check two pair
	if ((PlayerCards[0].Value == PlayerCards[1].Value && PlayerCards[2].Value == PlayerCards[3].Value)
	|| (PlayerCards[1].Value == PlayerCards[2].Value && PlayerCards[3].Value == PlayerCards[4].Value)
	|| (PlayerCards[0].Value == PlayerCards[1].Value && PlayerCards[3].Value == PlayerCards[4].Value))
	{
		PlayerCombination = EPokerCombinations::TwoPair;
		return;
	}

	//check one pair
	TArray<FCard> Cards = PlayerCards;
	while (Cards.Num() != 0)
	{
		int32 CardValue = Cards[0].Value;
		CombinationRang = Cards[0].Value;
		
		Cards.RemoveSingle(Cards[0]);

		auto OnePairCard = Cards.FindByPredicate([CardValue](const FCard PlayerCard)
		{
			return PlayerCard.Value == CardValue;
		});

		if (OnePairCard)
		{
			PlayerCombination = EPokerCombinations::OnePair;
			return;
		}
	}

	PlayerCombination = EPokerCombinations::HighCard;
	CombinationRang = PlayerCards.Last().Value;
}
