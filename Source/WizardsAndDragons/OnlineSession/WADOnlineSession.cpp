#include "WADOnlineSession.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../WADGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/SessionOverviewWidget.h"

UWADOnlineSession::UWADOnlineSession()
{
	SessionWidgetClass = USessionOverviewWidget::StaticClass();
}

bool UWADOnlineSession::CreateSession(TSharedPtr<const FUniqueNetId> NetId, FName SessionName, bool bIsLAN, int32 MaxPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && NetId.IsValid())
		{
			TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = true;
			SessionSettings->NumPublicConnections = MaxPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*NetId, SessionName, *SessionSettings);
		}
	}

	return false;
}

void UWADOnlineSession::HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful)
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
			}
		}
	}
}

void UWADOnlineSession::HandleStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("POC")), true, "listen");
	}
}

void UWADOnlineSession::FindSessions(TSharedPtr<const FUniqueNetId> NetId, bool bIsLAN)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && NetId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*NetId, SearchSettingsRef);
		}
	}
}

void UWADOnlineSession::HandleFindSessionsComplete(bool bWasSuccessful)
{
	DebugPrint(TEXT("HandleFindSessionsComplete ") + bWasSuccessful ? TEXT("True") : TEXT("False"));
	
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			if (SessionWidgetInstance)
			{
				SessionWidgetInstance->HandleSessionsFound(SessionSearch);
			}

			DebugPrint(TEXT("Sessions found: ") + FString::FromInt(SessionSearch->SearchResults.Num()));
		}
	}
}

bool UWADOnlineSession::JoinSession(TSharedPtr<const FUniqueNetId> NetId, const FOnlineSessionSearchResult& SearchResult, FName SessionName)
{
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && NetId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = Sessions->JoinSession(*NetId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UWADOnlineSession::HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const FString StringResult = LexToString(Result);
	DebugPrint(TEXT("HandleJoinSessionComplete ") + StringResult);

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				DebugPrint(TEXT("TravelURL ") + TravelURL);
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UWADOnlineSession::DestroySession(FName SessionName)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(SessionName);
		}
	}
}

void UWADOnlineSession::HandleSessionDestroyed(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MAP_MainMenu", true);
			}
		}
	}
}

void UWADOnlineSession::RegisterOnlineDelegates()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UWADOnlineSession::HandleCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UWADOnlineSession::HandleStartSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UWADOnlineSession::HandleFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UWADOnlineSession::HandleJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UWADOnlineSession::HandleSessionDestroyed);
}

void UWADOnlineSession::ClearOnlineDelegates()
{

}

void UWADOnlineSession::ShowSessionUI()
{
	if (!SessionWidgetInstance)
	{
		SessionWidgetInstance = CreateWidget<USessionOverviewWidget>(GetWorld(), SessionWidgetClass);
		SessionWidgetInstance->AddToViewport();
	}
}

void UWADOnlineSession::HideSessionUI()
{
	if (SessionWidgetInstance)
	{
		SessionWidgetInstance->RemoveFromParent();
		SessionWidgetInstance = nullptr;
	}
}

UWADGameInstance* UWADOnlineSession::GetGameInstance() const
{
	return CastChecked<UWADGameInstance>(GetOuter());
}

UWorld* UWADOnlineSession::GetWorld() const
{
	return GetGameInstance()->GetWorld();
}

void UWADOnlineSession::DebugPrint(const FString& DebugString)
{
	UKismetSystemLibrary::PrintString(GetWorld(), DebugString, true, true, FLinearColor::Yellow, 5.0f);
}
