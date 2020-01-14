#pragma once

#include "OnlineSessionSettings.h"
#include "SessionInfo.generated.h"

USTRUCT(BlueprintType)
struct FWADBlueprintSessionInfo
{
	GENERATED_BODY()
public:

	FOnlineSessionSearchResult SessionSearchResult;
};
