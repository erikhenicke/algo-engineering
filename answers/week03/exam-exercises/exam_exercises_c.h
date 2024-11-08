//
// Created by erik on 03.11.24.
//

#pragma once

#include <stdbool.h>

void hello_world();
void fibs_from(int n);
void game_of_life_sp(int size, int gens, int num_threads, bool outputImages);
void game_of_life_dp(int size, int gens, int num_threads);