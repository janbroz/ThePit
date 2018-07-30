// Fill out your copyright notice in the Description page of Project Settings.

#include "PitGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPitGameInstance::UPitGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPitGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UPitGameInstance::OnStartOnlineGameComplete);

	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPitGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPitGameInstance::OnJoinSessionComplete);

	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPitGameInstance::OnDestroySessionComplete);

	CurrentLocation = EPlayerLocation::MainMenu;

	static ConstructorHelpers::FObjectFinder<UDataTable> HeroStatsTable_BP(TEXT("DataTable'/Game/GameplayData/Heroes/Heroes-stats.Heroes-stats'"));
	if (HeroStatsTable_BP.Object)
	{
		HeroStatsTable = HeroStatsTable_BP.Object;
	}
}

bool UPitGameInstance::HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No online subsystem"));
	}
	return false;
}

void UPitGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Sessions->StartSession(SessionName);

				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("The session was good")));
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Holi"));
}

void UPitGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
		if (bWasSuccessful)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
		}
	}
}

void UPitGameInstance::FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}
			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void UPitGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			BlueprintSessionResults.Empty();
			BSearchResult.Empty();
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("There are exactly %d sessions")), SessionSearch->SearchResults.Num());
			}
			
			if (SessionSearch->SearchResults.Num() > 0)
			{
				for (int32 SearchIndex = 0; SearchIndex < SessionSearch->SearchResults.Num(); SearchIndex++)
				{
					UE_LOG(LogTemp, Warning, TEXT("A session"));
					FBlueprintSessionResult Result;
					Result.OnlineResult = SessionSearch->SearchResults[SearchIndex];
					BlueprintSessionResults.Add(Result);
					//BSearchResult.Add(FOnlineSearchResultWrapper(SessionSearch->SearchResults[SearchIndex]));
				}
			}
		}
	}
	if (Pit_OnUpdateSessionUI.IsBound())
	{
		Pit_OnUpdateSessionUI.Broadcast();
	}

}

bool UPitGameInstance::PJoinSession(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	CurrentLocation = EPlayerLocation::Lobby;
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
			if (!bSuccessful)
			{
				CurrentLocation = EPlayerLocation::MainMenu;
			}
		}
	}

	return bSuccessful;
}

void UPitGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
		
	}
}

void UPitGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true);
			}
		}
	}
}

void UPitGameInstance::StartOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSession(Player->GetPreferredUniqueNetId(), GameSessionName, true, true, 4);
	
}

void UPitGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId(), true, true);
}

void UPitGameInstance::JoinOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FOnlineSessionSearchResult SearchResult;

	UE_LOG(LogTemp, Warning, TEXT("Trying to join a game"));

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			if (SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
			{
				SearchResult = SessionSearch->SearchResults[i];
				PJoinSession(Player->GetPreferredUniqueNetId(), GameSessionName, SearchResult);
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("there are no game sessions to join"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("There are no sessions")));
		}
	}
}

void UPitGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(GameSessionName);
		}
	}
}