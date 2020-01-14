#include "SessionStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "WADOnlineSession.h"
#include "../WADGameInstance.h"

UWADGameInstance* USessionStatics::GetWADGameInstance(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return CastChecked<UWADGameInstance>(World->GetGameInstance(), ECastCheckedType::NullChecked);
	}
	
	ensureMsgf(false, TEXT("Unable to locate an instance of FPSGameInstance."));

	return nullptr;
}

UWADOnlineSession* USessionStatics::GetWADOnlineSession(const UObject* WorldContextObject)
{
	if (UWADGameInstance* GameInstance = USessionStatics::GetWADGameInstance(WorldContextObject))
	{
		return GameInstance->GetWADOnlineSession();
	}

	return nullptr;
}

bool USessionStatics::CreateSession(const UObject* WorldContextObject, FName SessionName, bool bIsLAN, int32 NumberOfPlayers)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		return OnlineSession->CreateSession(GetUniqueNetId(WorldContextObject), SessionName, bIsLAN, NumberOfPlayers);
	}

	return false;
}

void USessionStatics::FindSessions(const UObject* WorldContextObject, bool bIsLan)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		OnlineSession->FindSessions(GetUniqueNetId(WorldContextObject), bIsLan);
	}
}

void USessionStatics::ShowSessionUI(const UObject* WorldContextObject)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		OnlineSession->ShowSessionUI();
	}
}

void USessionStatics::HideSessionUI(const UObject* WorldContextObject)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		OnlineSession->HideSessionUI();
	}
}

void USessionStatics::JoinSession(const UObject* WorldContextObject, const FOnlineSessionSearchResult& SearchResult, FName SessionName)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		OnlineSession->JoinSession(GetUniqueNetId(WorldContextObject), SearchResult, SessionName);
	}
}

void USessionStatics::DestroySession(const UObject* WorldContextObject, FName SessionName)
{
	if (UWADOnlineSession* OnlineSession = USessionStatics::GetWADOnlineSession(WorldContextObject))
	{
		OnlineSession->DestroySession(SessionName);
	}
}

const TSharedPtr<const FUniqueNetId> USessionStatics::GetUniqueNetId(const UObject* WorldContextObject)
{
	UWADGameInstance* GameInstance = USessionStatics::GetWADGameInstance(WorldContextObject);
	ULocalPlayer* const Player = GameInstance->GetFirstGamePlayer();
	FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(Player->GetPreferredUniqueNetId());
	return UniqueNetIdWrapper.GetUniqueNetId();
}
