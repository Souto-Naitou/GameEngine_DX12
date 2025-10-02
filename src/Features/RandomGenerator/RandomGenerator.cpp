#include "RandomGenerator.h"

void RandomGenerator::Initialize()
{
    engine_.seed(seed_());
}

void RandomGenerator::Finalize()
{

}
