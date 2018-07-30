// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Online.h"
#include "FindSessionsCallbackProxy.h"
#include "Data/DataStructures.h"
#include "Engine/DataTable.h"
#include "PitGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FOnlineSearchResultWrapper
{
	GENERATED_BODY()
public:
	FOnlineSessionSearchResult PitSearchResult;

public:
	FOnlineSearchResultWrapper() {}
	FOnlineSearchResultWrapper(FOnlineSessionSearchResult ResultMe) {
		PitSearchResult = ResultMe;
	}
};

// For testing only
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUISessions);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);

UCLASS()
class THEPIT_API UPitGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPitGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	// Delegates for session creation/starting
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	// Handles to register the delegates of session creation/starting
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	// Session settings
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	// Delegate for searching sessions
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	// Handle for the searching delegate
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	// Join a session
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	// Handle for the join session delegate
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	// Delegate for session destroy
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	// Handle for the delegate of session destroy
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FOnlineSearchResultWrapper> BSearchResult;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FBlueprintSessionResult> BlueprintSessionResults;
	// testing delegate
	FUpdateUISessions Pit_OnUpdateSessionUI;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		EPlayerLocation CurrentLocation;

public:
	// Used when the session create is completed
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// Used when the session start is completed
	virtual void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	
	// Host a game
	bool HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	// Find a session
	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence);

	// We found the session
	void OnFindSessionsComplete(bool bWasSuccessful);

	// Join a session
	bool PJoinSession(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Destroy a session
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// Blueprint callable functions
	UFUNCTION(BlueprintCallable, Category = "Network with blueprints")
		void StartOnlineGame();
	UFUNCTION(BlueprintCallable, Category = "Network with blueprints")
		void FindOnlineGames();
	UFUNCTION(BlueprintCallable, Category = "Network with blueprints")
		void JoinOnlineGame();
	UFUNCTION(BlueprintCallable, Category = "Network with blueprints")
		void DestroySessionAndLeaveGame();

public:
	// PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero information")
		UDataTable* HeroStatsTable;

};
