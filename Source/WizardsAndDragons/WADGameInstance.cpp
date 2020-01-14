#include "WADGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Engine/LocalPlayer.h"
#include "OnlineSession/WADOnlineSession.h"

UWADGameInstance::UWADGameInstance()
{

}

TSubclassOf<UOnlineSession> UWADGameInstance::GetOnlineSessionClass()
{
	return OnlineSessionClass;
}

UWADOnlineSession* UWADGameInstance::GetWADOnlineSession() const
{
	return CastChecked<UWADOnlineSession>(OnlineSession);
}

bool UWADGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	return false;
}

const TSharedPtr<const FUniqueNetId> UWADGameInstance::GetUniqueNetId(const ULocalPlayer* InLocalPlayer) const
{
	InLocalPlayer = InLocalPlayer ? InLocalPlayer : GetFirstGamePlayer();
	FUniqueNetIdWrapper UniqueNetIdWrapper = FUniqueNetIdWrapper(InLocalPlayer->GetPreferredUniqueNetId());
	return UniqueNetIdWrapper.GetUniqueNetId();
}

