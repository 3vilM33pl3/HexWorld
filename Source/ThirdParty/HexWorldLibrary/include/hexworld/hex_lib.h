#pragma once

enum class Direction {
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

constexpr const char* DirectionEnumToString(Direction d) {
    switch (d) {
        case Direction::N: return "N";
        case Direction::NE: return "NE";
        case Direction::E: return "E";
        case Direction::SE: return "SE";
        case Direction::S: return "S";
        case Direction::SW: return "SW";
        case Direction::W: return "W";
        case Direction::NW: return "NW";
    }
}

struct Hexagon {
    int64_t X;
    int64_t Y;
    int64_t Z;
    std::string Type;
    Direction Direction;

    Hexagon(int x_, int y_, int z_, std::string type_, enum Direction direction_)
            :X(x_), Y(y_), Z(z_), Type(type_), Direction(direction_) {};
};

struct AxialCoordinates {
    int32_t Q;
    int32_t R;

    AxialCoordinates(int32_t Q_, int32_t R_) {
        Q = Q_;
        R = R_;
    }
};