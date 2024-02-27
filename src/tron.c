#include "include/camera.h"
#include "include/math/matrix.h"
#include "include/math/vec.h"
#include "include/model.h"
#include "include/utils/str.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


void tron_draw_model(SDL_Renderer *ren, const camera_t *camera, const model_t *model) {
    for (size_t i = 0; i < model->edges_count; ++i) {
        vec3_t p1 = model->verticies[model->edges[i * 2]];
        vec3_t p2 = model->verticies[model->edges[i * 2 + 1]];

        p1 = camera_get_project_point(camera, &p1);
        p2 = camera_get_project_point(camera, &p2);

        SDL_RenderDrawLineF(ren, p1.x, p1.y, p2.x, p2.y);
    }
}

void transform_model_mat(model_t *dst_model, model_t *src_model, const matrix_t *mat) {
    static matrix_t *tmp_point_mat = NULL; 
    static matrix_t *tmp_new_point_mat = NULL;

    if (tmp_point_mat == NULL) {
        printf("creating temporary matrixes");
        tmp_point_mat = matrix_new(4, 1);
        tmp_new_point_mat = matrix_new(4, 1);
    }
    
    for (size_t i = 0; i < src_model->verticies_count; ++i) {
        vec4_t point = vec4_from_vec3(&src_model->verticies[i]);
        point.w = 1;
        
        matrix_set_column_vec4(tmp_point_mat, &point);
        matrix_mul(tmp_new_point_mat, mat, tmp_point_mat);
        
        vec3_t *draw_point = &dst_model->verticies[i];
        draw_point->x = matrix_get(tmp_new_point_mat, 0, 0);
        draw_point->y = matrix_get(tmp_new_point_mat, 1, 0);
        draw_point->z = matrix_get(tmp_new_point_mat, 2, 0);
    }
}

void transform_model(model_t *dst_model, model_t *src_model) {
    for (size_t i = 0; i < src_model->verticies_count; ++i) {
        vec3_t *vec = &dst_model->verticies[i];
        *vec = src_model->verticies[i];
        
        vec3_rotx(vec, src_model->rot.x);
        vec3_roty(vec, src_model->rot.y);
        vec3_rotz(vec, src_model->rot.z);
        vec3_scale(vec, &src_model->scale);
        vec3_add(vec, &src_model->pos);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI); if (win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return EXIT_FAILURE;
	}

    camera_t camera = {
        .pos = {400, 290, -1000},
        .rot = {0, 0, 0},
        .display_pos = {400, 290, -1100}
    };

    vec3_t cube_verticies[] = {
        {0, 0, -1},//{-1, 1, -1},
        {1, 1, -1},
        {1, -1, -1},
        {-1, -1, -1},

        {-1, 1, 1},
        {1, 1, 1},
        {1, -1, 1},
        {-1, -1, 1},
    };
    size_t cube_edges[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},

        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    vec3_t verticies[] = {
        {0, 1, -0.5},

        {-1, -1, -1},
        {1, -1, -1},
        {1, -1, 1},
        {-1, -1, 1}
    };
    size_t edges[][2] = {
        {0, 1},
        {0, 2},
        {0, 3},
        {0, 4},

        {1, 2},
        {2, 3},
        {3, 4},
        {4, 1}
    };
    
    model_t *cube = model_from_raw(verticies, sizeof(verticies) / sizeof(vec3_t), edges, sizeof(edges) / sizeof(size_t) / 2);
    model_t *draw_cube = model_from_raw(verticies, sizeof(verticies) / sizeof(vec3_t), edges, sizeof(edges) / sizeof(size_t) / 2);

    cube->scale = (vec3_t){100, 100, 100};
    cube->pos = (vec3_t){300, 300, 2020};
    cube->scale = (vec3_t){300, 300, 300};
    printf("%s\n", vec3_str(&cube->verticies[0]).str);
    printf("%s\n", vec3_str(&draw_cube->verticies[0]).str);
    printf("%p and %p\n", cube, draw_cube);    

    Uint64 dt_now = SDL_GetPerformanceCounter();
    double dt;
    vec3_t angle_speed = {500, 500, 500};
    double move_speed = 10001;
    
    bool run = true;
    while (run) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    run = false;
                } break;
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP: {
                            cube->rot.z += angle_speed.y * dt;
                        } break;
                        case SDLK_DOWN: {
                            cube->rot.z -= angle_speed.y * dt;
                        } break;
                        case SDLK_RIGHT: {
                            cube->rot.y += angle_speed.x * dt;
                        } break;
                        case SDLK_LEFT: {
                            cube->rot.y -= angle_speed.x * dt;
                        } break;

                        case SDLK_d: {
                            camera.pos.x += move_speed * dt;
                            camera.display_pos.x += move_speed * dt;
                        } break;
                        case SDLK_a: {
                            camera.pos.x -= move_speed * dt;
                            camera.display_pos.x -= move_speed * dt;
                        } break;
                        case SDLK_w: {
                            camera.pos.y -= move_speed * dt;
                            camera.display_pos.y -= move_speed * dt;
                        } break;
                        case SDLK_s: {
                            camera.pos.y += move_speed * dt;
                            camera.display_pos.y += move_speed * dt;
                        } break;
                        case SDLK_q: {
                            camera.pos.z += move_speed * dt;
                            camera.display_pos.z += move_speed * dt;
                        } break;
                        case SDLK_e: {
                            camera.pos.z -= move_speed * dt;
                            camera.display_pos.z -= move_speed * dt;
                        } break;

                        case SDLK_t: {
                            camera.rot.x += angle_speed.x * dt;
                        } break;
                        case SDLK_r: {
                            camera.rot.x -= angle_speed.x * dt;
                        } break;
                        case SDLK_g: {
                            camera.rot.y += angle_speed.y * dt;
                        } break;
                        case SDLK_f: {
                            camera.rot.y -= angle_speed.y * dt;
                        } break;

                        case SDLK_1: {
                            cube->pos.x -= move_speed * dt;
                        } break;
                        case SDLK_2: {
                            cube->pos.x += move_speed * dt;
                        } break;
                        case SDLK_3: {
                            cube->pos.y += move_speed * dt;
                        } break;
                        case SDLK_4: {
                            cube->pos.y -= move_speed * dt;
                        } break;
                        case SDLK_5: {
                            cube->pos.z += move_speed * dt;
                        } break;
                        case SDLK_6: {
                            cube->pos.z -= move_speed * dt;
                        } break;
                    }
                } break;
            }
        }

        dt = (SDL_GetPerformanceCounter() - dt_now) / (double)SDL_GetPerformanceFrequency();
        dt_now = SDL_GetPerformanceCounter();

        transform_model(draw_cube, cube);
        
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255,255,255,0);
        tron_draw_model(ren, &camera, draw_cube);

        SDL_RenderPresent(ren);
    }
    

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

    model_free(cube);
    model_free(draw_cube);
}
