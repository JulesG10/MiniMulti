#pragma once
#include<stdlib.h>

typedef enum NetObjectTypes {
    NONE = 0,
    PLAYER = 1,

}NetObjectTypes;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#define NET_MESSAGE_SIZE 2048
#define NET_NAME_SIZE 24

typedef struct NetCharacterObject {
    int state;
    int direction;

    float frame;
    float speed;
    float x, y;
}NetObject;

typedef struct NetMessage {
    char content[NET_MESSAGE_SIZE];
    size_t id;
    size_t reply_id;
}NetMessage;


typedef struct NetData {
    NetCharacterObject object;
    //NetMessage message;
    long long time;
    time_t id;
}NetData;