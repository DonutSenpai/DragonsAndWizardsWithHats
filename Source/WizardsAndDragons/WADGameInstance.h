#pragma once

#include "Engine/GameInstance.h"
#include "WADGameInstance.generated.h"

class UWADOnlineSession;

UCLASS()
class UWADGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UWADGameInstance();

	/** @return OnlineSession class to use for this game instance  */
	virtual TSubclassOf<UOnlineSession> GetOnlineSessionClass() override;

	UWADOnlineSession* GetWADOnlineSession() const;
protected:
	UPROPERTY(EditDefaultsOnly, NoClear, Category = OnlineSession)
	TSubclassOf<UWADOnlineSession> OnlineSessionClass;

	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	const TSharedPtr<const FUniqueNetId> GetUniqueNetId(const ULocalPlayer* LocalPlayer = nullptr) const;
};
