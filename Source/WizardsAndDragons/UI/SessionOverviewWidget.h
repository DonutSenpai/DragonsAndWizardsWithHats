#pragma once

//#include "UserWidget.h"
#include "../OnlineSession/SessionInfo.h"
#include "Blueprint/UserWidget.h"
#include "SessionOverviewWidget.generated.h"

class FOnlineSessionSearch;

UCLASS()
class USessionOverviewWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void HandleSessionsFound(TSharedPtr<FOnlineSessionSearch> SessionSearch);

	UFUNCTION(BlueprintImplementableEvent, Category = "Session|UI", meta = (DisplayName = "On Session Found"))
	void BP_OnSessionFound(const FWADBlueprintSessionInfo& SessionInfo);
};
