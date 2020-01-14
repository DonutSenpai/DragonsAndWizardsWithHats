#include "SessionWidget.h"
#include "../OnlineSession/SessionStatics.h"

void USessionWidget::JoinSession()
{
	USessionStatics::JoinSession(GetWorld(), SessionInfo.SessionSearchResult);
}

void USessionWidget::InitSessionWidget()
{
	int32 MaxPlayers = SessionInfo.SessionSearchResult.Session.SessionSettings.NumPublicConnections;
	int32 CurrentPlayers = MaxPlayers - SessionInfo.SessionSearchResult.Session.NumOpenPublicConnections;

	BP_InitSessionWidget(SessionInfo.SessionSearchResult.Session.OwningUserName, CurrentPlayers, MaxPlayers, SessionInfo.SessionSearchResult.PingInMs);
}
