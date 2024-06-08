#ifndef OBSTACLE_H
#define OBSTACLE_H

struct Obstacle {
    float x;
    float y;
    float size;

    Obstacle(float x, float y, float sz);

    float ping() const;  // Method to handle a ping and return amplitude
};

#endif // OBSTACLE_H
