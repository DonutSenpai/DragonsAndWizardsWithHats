#pragma once

#include "GameFramework/OnlineSession.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WADOnlineSession.generated.h"

class FOnlineSessionSettings;
class FOnlineSessionSearch;
class USessionOverviewWidget;

UCLASS(BlueprintType, Blueprintable)
class UWADOnlineSession : public UOnlineSession
{
	GENERATED_BODY()
public:
	UWADOnlineSession();
	
	// Create Sessions
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	bool CreateSession(TSharedPtr<const FUniqueNetId> NetId, FName SessionName, bool bIsLAN, int32 MaxPlayers);
	void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// Start Sessions
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	void HandleStartSessionComplete(FName SessionName, bool bWasSuccessful);

	// Find Sessions
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	void FindSessions(TSharedPtr<const FUniqueNetId> NetId, bool bIsLAN);
	void HandleFindSessionsComplete(bool bWasSuccessful);

	// Join Sessions
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	bool JoinSession(TSharedPtr<const FUniqueNetId> NetId, const FOnlineSessionSearchResult& SearchResult, FName SessionName);
	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Destroy Sessions
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	void DestroySession(FName SessionName);
	void HandleSessionDestroyed(FName SessionName, bool bWasSuccessful);


	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void RegisterOnlineDelegates() override;
	virtual void ClearOnlineDelegates() override;


	// UI

	UPROPERTY(EditAnywhere, NoClear, Category = UI)
	TSubclassOf<USessionOverviewWidget> SessionWidgetClass;
	UPROPERTY(Transient)
	USessionOverviewWidget* SessionWidgetInstance = nullptr;

	void ShowSessionUI();
	void HideSessionUI();

	// Game Instance
	class UWADGameInstance* GetGameInstance() const;

	// UObject
	virtual UWorld* GetWorld() const override;

	void DebugPrint(const FString& DebugString);
};