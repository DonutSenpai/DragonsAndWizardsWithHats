#include "SessionOverviewWidget.h"
#include "OnlineSessionSettings.h"

void USessionOverviewWidget::HandleSessionsFound(TSharedPtr<FOnlineSessionSearch> SessionSearch)
{
	if (!SessionSearch.IsValid())
	{
		return;
	}

	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		FWADBlueprintSessionInfo SessionInfo;
		SessionInfo.SessionSearchResult = SearchResult;

		BP_OnSessionFound(SessionInfo);
	}
}
