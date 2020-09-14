#ifndef FIELDMODEL_H
#define FIELDMODEL_H


#define FIELD_SIZE 10

class FieldModel
{
public:
    enum CellState
    {
        Empty = 0,
        Ship,
        Hurted,
        Killed
    };

    FieldModel();
    CellState getCellValue(int i, int j);

private:
    void initField();

private:
    CellState mField[FIELD_SIZE][FIELD_SIZE];
};

#endif // FIELDMODEL_H
