class Vector2D{
public:
    typedef struct {
        float x;
        float y;
    } Coord;

    Vector2D();
    Vector2D(float x_, float y_);
    ~Vector2D();

    void add_vector(Vector2D vector_);
    void mult_vector(Vector2D vector_);

    float get_x() const;
    float get_y() const;
    Coord get_coord();

    void set_x(float x_);
    void set_y(float y_);
    void set_coord(Coord coord_);


private:
    Coord coord; ///< 2D (x, y)
};