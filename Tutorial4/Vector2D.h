class Vector2D{
public:
    typedef struct {
        int x;
        int y;
    } Coord;

    Vector2D();
    Vector2D(int x_, int y_);
    ~Vector2D();

    void add_vector(Vector2D vector_);
    void mult_vector(Vector2D vector_);

    int get_x();
    int get_y();
    Coord get_coord();

    void set_x(int x_);
    void set_y(int y_);
    void set_coord(Coord coord_);


private:
    Coord coord; ///< 2D (x, y)
};