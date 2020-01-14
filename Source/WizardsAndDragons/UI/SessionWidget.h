#pragma once

#include "Blueprint/UserWidget.h"
#include "../OnlineSession/SessionInfo.h"
#include "SessionWidget.generated.h"

class FOnlineSessionSearch;

UCLASS()
class USessionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Session|UI")
	void JoinSession();

	UFUNCTION(BlueprintCallable, Category = "Session|UI")
	void InitSessionWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = "Session|UI", meta = (DisplayName = "On Init Session Widget"))
	void BP_InitSessionWidget(const FString& ServerName, int32 CurrentPlayers, int32 MaxPlayers, int32 Ping);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Session, meta = (ExposeOnSpawn))
	FWADBlueprintSessionInfo SessionInfo;
};
