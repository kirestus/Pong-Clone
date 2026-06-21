#include <headers/bat.h>
#include <headers/dataStruct.h>
#include <memory>

class BatSystem
{
public:
    BatSystem();
    std::shared_ptr<Bat> WhichBatShouldBallAttatchTo(const DataStruct& rTuple);
    void HandleBallAttachements(const DataStruct& rTuple);
    void AttachBallToBat(const DataStruct& rTuple, std::shared_ptr<Bat> pBat);
    eBatMoveDirection CaclulateCurrentMoveDirection(const DataStruct& rTuple, std::shared_ptr<Bat> pBat);
    void CalculateBatSpeed(const DataStruct& rTuple, std::shared_ptr<Bat> pBat, const float fDeltaT);
    void NudgeBat(const DataStruct& rTuple, std::shared_ptr<Bat> pBat);
};