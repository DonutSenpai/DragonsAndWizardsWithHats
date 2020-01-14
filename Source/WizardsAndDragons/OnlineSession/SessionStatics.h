#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionInfo.h"
#include "SessionStatics.generated.h"

class UWADGameInstance;
class UWADOnlineSession;

UCLASS()
class  USessionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static UWADGameInstance* GetWADGameInstance(const UObject* WorldContextObject);
	static UWADOnlineSession* GetWADOnlineSession(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "WAD|Session", meta = (WorldContext = "WorldContextObject", DisplayName = "WAD Create Session"))
	static bool CreateSession(const UObject* WorldContextObject, FName SessionName, bool bIsLAN = true, int32 NumberOfPlayers = 4);

	UFUNCTION(BlueprintCallable, Category = Session, meta = (WorldContext = "WorldContextObject", DisplayName = "WAD Find Sessions"))
	static void FindSessions(const UObject* WorldContextObject, bool bIsLan);

	UFUNCTION(BlueprintCallable, Category = Session, meta = (WorldContext = "WorldContextObject"))
	static void ShowSessionUI(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = Session, meta = (WorldContext = "WorldContextObject"))
	static void HideSessionUI(const UObject* WorldContextObject);

	static void JoinSession(const UObject* WorldContextObject, const FOnlineSessionSearchResult& SearchResult, FName SessionName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = Session, meta = (WorldContext = "WorldContextObject", DisplayName = "WAD Destroy Session"))
	static void DestroySession(const UObject* WorldContextObject, FName SessionName);

	static const TSharedPtr<const FUniqueNetId> GetUniqueNetId(const UObject* WorldContextObject);
};
