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

        //double k1 = 10.f / p1.z;
        //double k2 = 10.f / p2.z;

        //p1.x *= k1;
        //p1.y *= k1;
        //p2.x *= k2;
        //p2.y *= k2;
        
        p1 = camera_get_project_point(camera, &p1);
        p2 = camera_get_project_point(camera, &p2);

        printf("(%f, %f, %f) -> (%f, %f, %f)\r", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
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
        
        //matrix_t *point_mat = matrix_from_column_vec4(&point4);
        matrix_set_column_vec4(tmp_point_mat, &point);
        //str_t ps = vec4_str(&point);
        //printf("Vector: %s\n", ps.str);
        //str_free(&ps);
        //ps = matrix_str(tmp_point_mat);
        //printf("Vector mat:\n%s\n", ps.str);
        //str_free(&ps);
        
        matrix_mul(tmp_new_point_mat, mat, tmp_point_mat);
        
        //str_t s = matrix_str(tmp_new_point_mat);
        //printf("%s\n", s.str);
        //str_free(&s);
        
        vec3_t *draw_point = &dst_model->verticies[i];
        printf("%s\n", matrix_str(tmp_new_point_mat).str);
        draw_point->x = matrix_get(tmp_new_point_mat, 0, 0);
        draw_point->y = matrix_get(tmp_new_point_mat, 1, 0);
        draw_point->z = matrix_get(tmp_new_point_mat, 2, 0);
        
        //printf("%s\n", vec3_str(&src_model->verticies[0]).str);
    }
}

void transform_model(model_t *dst_model, model_t *src_model, const camera_t* camera, ) {
    for (size_t i = 0; i < src_model->verticies_count; ++i) {
        
    }
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (win == NULL) {
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
    //matrix_init();

    camera_t camera = {
        .pos = {0, 0, -3},
        .rot = {0, 0, 0},
        .display_pos = {0, 0, -2}
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
    //model_t *draw_cube = model_from(cube);
    model_t *draw_cube = model_from_raw(verticies, sizeof(verticies) / sizeof(vec3_t), edges, sizeof(edges) / sizeof(size_t) / 2);

    matrix_t *transform_mat = matrix_new(4, 4);

    double fov = 1.f / tan(70 / 2.f);
    double aspect_ratio = 800.f / 600.f;
    double near = 10.f;
    double far = 100.f;

    double near_speed = 10;
    double far_speed = 10;
    
    printf("%s\n", vec3_str(&cube->verticies[0]).str);
    printf("%s\n", vec3_str(&draw_cube->verticies[0]).str);
    printf("%p and %p\n", cube, draw_cube);    

    Uint64 dt_now = SDL_GetPerformanceCounter();
    double dt;
    vec3_t angle_speed = {500, 500, 500};
    vec3_t angle = {0, 10, 0};
    vec3_t move_vec = {300, 200, 10};
    double move_speed = 10001;
    vec3_t scale_vec = {100, 100, 100};

    printf("dist = %f\n", vec3_dist(&angle_speed, &angle));
    
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
                            angle.z += angle_speed.y * dt;
                        } break;
                        case SDLK_DOWN: {
                            angle.z -= angle_speed.y * dt;
                        } break;
                        case SDLK_RIGHT: {
                            angle.y += angle_speed.x * dt;
                        } break;
                        case SDLK_LEFT: {
                            angle.y -= angle_speed.x * dt;
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
                            move_vec.x -= move_speed * dt;
                        } break;
                        case SDLK_2: {
                            move_vec.x += move_speed * dt;
                        } break;
                        case SDLK_3: {
                            move_vec.y += move_speed * dt;
                        } break;
                        case SDLK_4: {
                            move_vec.y -= move_speed * dt;
                        } break;
                    }
                } break;
            }
        }

        dt = (SDL_GetPerformanceCounter() - dt_now) / (double)SDL_GetPerformanceFrequency();
        //printf("dt = %f\n", dt);
        dt_now = SDL_GetPerformanceCounter();
    
        //matrix_mul(transform_mat, get_move_matrix(&move_vec), get_scale_matrix(&scale_vec));
        //matrix_mul(transform_mat, transform_mat, get_xrotate_matrix(angle.x));
        //matrix_mul(transform_mat, transform_mat, get_yrotate_matrix(angle.y));
        //matrix_mul(transform_mat, transform_mat, get_zrotate_matrix(angle.z));
        ////matrix_mul(transform_mat, transform_mat, get_clip_matrix(fov, aspect_ratio, far, near));

        //angle.x += speed.x * dt;
        //angle.y += speed.y * dt;
        //angle.z += speed.z * dt;
       
        //transform_model_mat(draw_cube, cube, transform_mat);

        for (size_t i = 0; i < cube->verticies_count; ++i) {
            draw_cube->verticies[i] = cube->verticies[i];
            vec3_t *vec = &draw_cube->verticies[i];
            
            vec3_rotx(vec, angle.x);
            vec3_roty(vec, angle.y);
            vec3_rotz(vec, angle.z);
            vec3_scale(vec, &scale_vec);
            vec3_add(vec, &move_vec);
        }
        
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
    ////matrix_free(tmp_point_mat);
    ////matrix_free(tmp_new_point_mat);
    //matrix_free(transform_mat);
}
