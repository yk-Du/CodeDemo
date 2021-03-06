#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "ex19.h"

int Monster_attack(void *self, int damage){
    Monster *monster = self;
    assert(monster);
    assert(monster->_(description));
    
    printf("You attck %s!\n", monster->_(description));
    monster->hit_points -= damage;
    if(monster->hit_points > 0){
        printf("It is still alive.\n"); 
        return 0;
    } else {
        printf("It is dead!\n"); 
        return 1;
    }
}

int Monster_init(void *self){
    assert(self);
    Monster *monster = self;
    monster->hit_points = 10;
    return 1;
}

Object MonsterProto = {
    .init = Monster_init,
    .attack = Monster_attack,
    .destroy = Object_destroy,
};

void *Room_move(void *self, Direction direction){
    assert(self);
    Room *room = self;
    Room *next = NULL;
    
    if(direction == NORTH && room->north){
        printf("You go north, into:\n"); 
        next = room->north;
    } else if(direction == SOUTH  && room->south){
        printf("You go south, into:\n"); 
        next = room->south;
    } else if(direction == EAST  && room->east){
        printf("You go east, into:\n"); 
        next = room->east;
    } else if(direction == WEST  && room->west){
        printf("You go west, into:\n"); 
        next = room->west;
    }

    if(next){
        next->_(describe(next)); 
    }
    
    return next;
}

int Room_attack(void *self, int damage){
    assert(self);
    Room *room = self;
    Monster *monster = room->bad_guy;
    if(monster){
        monster->_(attack)(monster, damage); 
        return 1;
    } else {
        printf("you flail in the air at nothing, Idiot.\n");
        return 0; 
    }
}

void Room_destroy(void *self){
    Room *room = self;
    if(room->bad_guy){
        if(room->bad_guy->_(destroy)){
            room->bad_guy->_(destroy)(room->bad_guy);
        }
    }
    Object_destroy(room);
}

Object RoomProto = {
    .attack = Room_attack,
    .move = Room_move,
    .destroy = Room_destroy,
};

void *Map_move(void *self, Direction direction){
    assert(self);
    Map *map = self;

    Room *location = map->location;
    assert(location);

    Room *next = NULL;
    next = location->_(move)(location, direction);
    if(next){
        map->location = next; 
    } else {
        printf("There is no room to go!\n");
    }
    return next;
}

int Map_attack(void *self, int damage){
    assert(self);
    Map *map = self;
    Room *location = map->location;
    return location->_(attack)(location, damage);
}

int Map_init(void *self){
    assert(self);
    Map *map = self;

    map->room_num = 5;
    map->room_list = calloc(map->room_num, sizeof(Room*));

    Room *hall = map->room_list[0] = NEW(Room, "The great Hall");
    Room *throne = map->room_list[1] = NEW(Room, "The throne room");
    Room *arena = map->room_list[2] = NEW(Room, "The arena, with the minotaur");
    Room *kitchen = map->room_list[3] = NEW(Room, "Kitchen, you have the knife now");
    Room *toilet = map->room_list[4] = NEW(Room, "Toilet, you have to reset");

    arena->bad_guy = NEW(Monster, "The evil minotaur");
    toilet->bad_guy = NEW(Monster, "The evil DDD");

    hall->north = throne;

    throne->west = arena;
    throne->east = kitchen;
    throne->south = hall;

    arena->east = throne;
    kitchen->west = throne;

    toilet->south = kitchen;
    toilet->west = hall;
    kitchen->north = toilet;
    hall->east = toilet;
    
    map->start = hall;
    map->location = hall;

    return 1;
}

void Map_destroy(void *self){
    Map *map = self;
    assert(map);
    int i;
    for(i = 0; i < map->room_num; i++){
        Room* room = map->room_list[i];
        if(room) {
            if(room->_(destroy)) room->_(destroy)(room);
        }
    }
    memset(map->room_list, 0, map->room_num * sizeof(Room*));
    free(map->room_list);
    Object_destroy(map);
}

Object MapProto = {
    .init = Map_init,
    .attack = Map_attack,
    .move = Map_move,
    .destroy = Map_destroy,
};

int process_input(Map *game){
    assert(game);
    printf("\n>");
    
    char ch = getchar();
    while(getchar() != '\n');
    int damage = rand() % 4;
    /*printf("Input %d\n", ch);*/
    switch(ch) {
        case -1:
            printf("Giving up? You suck.\n");
            return 0;
            break;
        case 'n':
            game->_(move)(game, NORTH);
            break;
        case 's':
            game->_(move)(game, SOUTH);
            break;
        case 'e':
            game->_(move)(game, EAST);
            break;
        case 'w':
            game->_(move)(game, WEST);
            break;
        case 'a':
            game->_(attack)(game, damage);
            break;
        case '1':
            printf("You can go:\n");
            if(game->location->north) printf("NORTH\n");
            if(game->location->south) printf("SOUTH\n");
            if(game->location->east) printf("EAST\n");
            if(game->location->west) printf("WEST\n");
            break;
        default:
            printf("What?: %d\n", ch);
    }
    return 1;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));

    Map *game = NEW(Map, "The Hall of the Minotaur.");
    assert(game);
    assert(game->location);
    printf("You enter the ");
    game->location->_(describe)(game->location);
    
    while(process_input(game)){
    }
    game->_(destroy)(game);
    return 0;
}
