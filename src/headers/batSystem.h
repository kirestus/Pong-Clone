#include <headers/bat.h>
#include <headers/dataStruct.h>
#include <memory>

class BatSystem
{
public:
    BatSystem();
    eBatMoveDirection DetermCurrentMoveDirection(const DataStruct& rTuple, std::shared_ptr<Bat> pBat);
    void CalculateBatSpeed(const DataStruct& rTuple, std::shared_ptr<Bat> pBat, const float fDeltaT);
    void NudgeBat(const DataStruct& rTuple, std::shared_ptr<Bat> pBat);
};