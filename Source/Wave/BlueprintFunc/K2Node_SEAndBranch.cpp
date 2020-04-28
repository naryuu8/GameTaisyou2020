// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SEAndBranch.h"
#include "EdGraphSchema_K2.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "GraphEditorSettings.h"
#include "EditorCategoryUtils.h"
#include "KismetCompiler.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_Knot.h"

#define LOCTEXT_NAMESPACE "K2Node_SEAndBranch"

// ノードのデフォルトのピン定義
void UK2Node_SEAndBranch::AllocateDefaultPins()
{
	// 入力実行ピン
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	// 出力Then実行ピン
	UEdGraphPin* TruePin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	TruePin->PinFriendlyName = LOCTEXT("AllocateDefaultPins_True", "true");

	// 出力Else実行ピン
	UEdGraphPin* FalsePin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
	FalsePin->PinFriendlyName = LOCTEXT("AllocateDefaultPins_False", "false");

	// 入力Conditionピン
	AddUniqueConditionPin();

	Super::AllocateDefaultPins();
}

// ノードタイトルの設定
FText UK2Node_SEAndBranch::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("GetNodeTitleDefault", "SE And Branch");
}

// ノード説明の設定
FText UK2Node_SEAndBranch::GetTooltipText() const
{
	return LOCTEXT("GetTooltipText", "Evaluate from the smaller pin number.");
}

// ノードタイトル色の設定
FLinearColor UK2Node_SEAndBranch::GetNodeTitleColor() const
{
	return GetDefault<UGraphEditorSettings>()->ExecBranchNodeTitleColor;
}

// ノードのメニュー上におけるカテゴリの設定
FText UK2Node_SEAndBranch::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::FlowControl);
}

// ノードアイコンの設定
FSlateIcon UK2Node_SEAndBranch::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon("EditorStyle", "GraphEditor.Branch_16x");
	return Icon;
}

// 番号つきConditionピンの名前を取得
FName UK2Node_SEAndBranch::GetPinNameGivenIndex(int32 Index) const
{
	return *FString::Printf(TEXT("%s_%d"), *UEdGraphSchema_K2::PN_Condition.ToString(), Index);
}

// 指定したピンがConditionピンか取得
bool UK2Node_SEAndBranch::IsConditionPin(UEdGraphPin* TargetPin) const
{
	return TargetPin && TargetPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Boolean;
}

// Thenピンの取得（主にExpandNode内や、SpawnIntermediateNodeでUK2Node_SEAndBranchが使われるときに使用する）
UEdGraphPin * UK2Node_SEAndBranch::GetThenPin() const
{
	UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
	check(Pin);
	return Pin;
}

// Elseピンの取得（主にExpandNode内や、SpawnIntermediateNodeでUK2Node_SEAndBranchが使われるときに使用する）
UEdGraphPin * UK2Node_SEAndBranch::GetElsePin() const
{
	UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Else);
	check(Pin);
	return Pin;
}

// Conditionピンの取得（主にExpandNode内や、SpawnIntermediateNodeでUK2Node_SEAndBranchが使われるときに使用する）
UEdGraphPin * UK2Node_SEAndBranch::GetConditionPinGivenIndex(const int32 Index)
{
	return FindPin(GetPinNameGivenIndex(Index));
}

// ワイヤーの繋がっていないConditionピンを含むか
bool UK2Node_SEAndBranch::IsHaveUnlinkConditionPin() const
{
	for (const auto& Pin : Pins)
	{
		if (IsConditionPin(Pin) && Pin->LinkedTo.Num() == 0)
		{
			return true;
		}
	}
	return false;
}

// 未使用番号のConditionピンの名前を取得
FName UK2Node_SEAndBranch::GetUniquePinName() const
{
	FName NewPinName;
	for (int32 i = 0; true; i++)
	{
		NewPinName = GetPinNameGivenIndex(i);
		if (!FindPin(NewPinName))
		{
			break;
		}
	}
	return NewPinName;
}

// 未使用番号のConditionピンを作成する
UEdGraphPin * UK2Node_SEAndBranch::AddUniqueConditionPin()
{
	UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetUniquePinName());
	Pin->DefaultValue = TEXT("true");
	return Pin;
}

// ピンの削除
void UK2Node_SEAndBranch::RemovePin(UEdGraphPin * TargetPin)
{
	// 指定したピンを削除する
	DestroyPin(TargetPin);
	Pins.Remove(TargetPin);

	// Conditionピンの番号を詰める
	int32 ThenIndex = 0;
	for (auto Pin : Pins)
	{
		if (IsConditionPin(Pin))
		{
			Pin->PinName = GetPinNameGivenIndex(ThenIndex++);
		}
	}
}

// ピンの追加と削除（ピンとワイヤーが接続されたり切断されたときに呼ばれる）
void UK2Node_SEAndBranch::PinConnectionListChanged(UEdGraphPin * Pin)
{
	if (IsConditionPin(Pin))
	{
		// Conditionピンからワイヤーが切断されたとき、切断されたワイヤーを削除
		if (Pin->LinkedTo.Num() == 0)
		{
			RemovePin(Pin);
		}

		// ワイヤーで繋がったピンしかないとき、ピンを追加
		if (!IsHaveUnlinkConditionPin())
		{
			AddUniqueConditionPin();
		}

		// ノード外観を更新する
		GetGraph()->NotifyGraphChanged();
	}

	Super::PinConnectionListChanged(Pin);
}

// ピンの再構築（主にこのノードがあるBlueprintGraphがロードされたときに呼ばれる関数）
void UK2Node_SEAndBranch::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins); // 内部でAllocateDefaultPins()が呼ばれる

	const int32 AlreadyPinNum = Pins.Num();
	for (int i = 0; i < OldPins.Num() - AlreadyPinNum; i++)
	{
		AddUniqueConditionPin();
	}
}

// メニューアクションの登録（コンテキストメニューからノードをスポーンさせるようにする）
void UK2Node_SEAndBranch::GetMenuActions(FBlueprintActionDatabaseRegistrar & ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_SEAndBranch::ExpandNode(FKismetCompilerContext & CompilerContext, UEdGraph * SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	// Conditionピンを取得
	TArray<UEdGraphPin*> ConditionPins;
	for (int32 i = 0; true; i++)
	{
		// TODO
		// 必要であればデフォルトピンの値による自明なフロー制御を行う
		// ・Trueデフォルトピンは、ConditionPinsに含めない
		// ・Falseデフォルトピンは、結果が自明（必ずElseに実行が流れる）
		UEdGraphPin* ConditionPin = FindPin(GetPinNameGivenIndex(i));
		if (!ConditionPin)
		{
			break;
		}
		ConditionPins.Add(ConditionPin);
	}

	// Rerouteノードをスポーンする
	UK2Node_Knot* RerouteNode = CompilerContext.SpawnIntermediateNode<UK2Node_Knot>(this, SourceGraph);
	RerouteNode->AllocateDefaultPins();
	UEdGraphPin* RerouteExecPin = RerouteNode->GetInputPin();

	// Branchノードをスポーンする
	TArray<UK2Node_IfThenElse*> BranchNodes;
	for (auto ConditionPin : ConditionPins)
	{
		UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
		BranchNode->AllocateDefaultPins();
		BranchNode->GetElsePin()->MakeLinkTo(RerouteExecPin);
		CompilerContext.MovePinLinksToIntermediate(*ConditionPin, *BranchNode->GetConditionPin());
		BranchNodes.Add(BranchNode);
	}

	// Branchノード同士を接続する
	for (int32 i = 0; i < BranchNodes.Num() - 1; i++)
	{
		UEdGraphPin* PrevBranchThenPin = BranchNodes[i]->GetThenPin();
		UEdGraphPin* PostBranchExecPin = BranchNodes[i + 1]->GetExecPin();
		PrevBranchThenPin->MakeLinkTo(PostBranchExecPin);
	}

	// ピンリンクの移動
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *BranchNodes[0]->GetExecPin());
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *BranchNodes.Last()->GetThenPin());
	CompilerContext.MovePinLinksToIntermediate(*GetElsePin(), *RerouteNode->GetOutputPin());

	BreakAllNodeLinks();
}

#undef LOCTEXT_NAMESPACE