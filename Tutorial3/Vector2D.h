class Vector2D{
public:
    Vector2D();
    Vector2D(int x_, int y_);
    ~Vector2D();

    void addX(int x_);
    void addY(int y_);
    void multX(int x_);

    int getX();
    int getY();
    //int** getXY();
    int* getXY();

    void setX(int x_);
    void setY(int y_);
    void setXY(int x_, int y_);


private:
    int x, y;
    //int **coord = 0; // NULL
    int* coord = 0;
};