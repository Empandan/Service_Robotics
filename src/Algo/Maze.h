class Maze {
public:
static constexpr int SIZE = 7;

    enum Direction {
        North,
        East,
        South,
        West
    };

    Maze();

    void addWall(int row, int col, Direction dir);
    bool hasWall(int row, int col, Direction dir) const;
    bool canMove(int row, int col, Direction dir) const;

    private:
    bool verticalWalls[SIZE][SIZE + 1] = {};
    bool horizontal[SIZE + 1][SIZE] ={};

    


};