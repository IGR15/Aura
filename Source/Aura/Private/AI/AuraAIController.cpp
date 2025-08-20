// IGR1S


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	BlackboardComp=CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(BlackboardComp);
	BehaviorTreeComp=CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComp);
}
