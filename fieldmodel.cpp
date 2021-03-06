#include "fieldmodel.h"

FieldModel::FieldModel()
{
    initField();

    mField[0][0] = Ship;
    mField[0][1] = Ship;
    mField[0][2] = Ship;
}

FieldModel::CellState FieldModel::getCellValue(int i, int j)
{
    return mField[i][j];
}

void FieldModel::initField()
{
    for (int i = 0; i < FIELD_SIZE; ++i)
    {
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            mField[i][j] = Empty;
        }
    }

    mField[0][0] = Hurted;
}
