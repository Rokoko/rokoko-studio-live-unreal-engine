// 2023 Copyright Sony Corporation, Inc. All Rights Reserved


#include "MocopiBPLibrary.h"
#include "MocopiLiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "LiveLinkClientReference.h"


void UMocopiBPLibrary::CreateMocopiSource(FLiveLinkSourceHandle& handle, const int inputPort, const FString& subjectName)
{
  TSharedPtr<ILiveLinkSource> mocopiSource = MakeShared<FMocopiLiveLinkSource>(inputPort, FName(subjectName));

  FLiveLinkClientReference client;
  client.GetClient()->AddSource(mocopiSource);

  handle.SetSourcePointer(mocopiSource);
}
