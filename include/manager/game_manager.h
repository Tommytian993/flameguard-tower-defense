#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H
#define SDL_MAIN_HANDLED

#include "manager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

// class manages the whole game
class GameManager : public Manager<GameManager>
{
    friend class Manager<GameManager>; // allow Manager to create GameManagers

public:
    // main running function start game loop
    int run(int argc, char **argv)
    {
        // 记录初始性能计数器和频率，用于计算每一帧经过的时间
        Uint64 last_counter = SDL_GetPerformanceCounter();
        const Uint64 counter_freq = SDL_GetPerformanceFrequency();

        // 游戏主循环
        while (!is_quit)
        {
            // 事件处理：比如键盘、鼠标事件等
            while (SDL_PollEvent(&event))
                on_input();

            // 当前时间点
            Uint64 current_counter = SDL_GetPerformanceCounter();
            // 计算两帧之间的时间差（以秒为单位）
            double delta = (double)(current_counter - last_counter) / counter_freq;
            last_counter = current_counter;

            // 控制帧率为60 FPS
            if (delta * 1000.0 > (1000.0 / 60))
                SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

            // 更新逻辑，比如物体移动、碰撞检测等
            on_update(delta);

            // 渲染设置：清除画面并设置背景色为黑色
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // 渲染游戏内容（绘制精灵等）
            on_render();

            // 显示渲染结果
            SDL_RenderPresent(renderer);
        }

        return 0;
    }

protected:
    GameManager() {
        // 初始化SDL核心、图像、音频、字体等组件
        init_assert(SDL_Init(SDL_INIT_EVERYTHING), "SDL2 初始化失败!");
        init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_image 初始化失败!");
        init_assert(Mix_Init(MIX_INIT_MP3), "SDL_mixer 初始化失败!");
        init_assert(TTF_Init(), "SDL_ttf 初始化失败!");

        // 初始化音频播放
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        // 启用中文输入法支持
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

        // 创建窗口
        window = SDL_CreateWindow("村庄保卫战", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
        init_assert(window, "创建游戏窗口失败!");

        // 创建渲染器
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
        init_assert(renderer, "创建渲染器失败!");
    }

    // destructor free all resources
    ~GameManager()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }

private:
    SDL_Event event;

    bool is_quit = false;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    // assert function, if fail to initalize pop message box and exit
    void init_assert(bool flag, const char *err_msg)
    {
        if (flag)
            return;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "游戏启动失败", err_msg, window);
        exit(-1);
    }

    // 处理输入事件（待实现）
    void on_input()
    {
        // 例子：按下ESC退出游戏
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        {
            is_quit = true;
        }
    }

    void on_update(double delta)
    {
        // delta: time between 2 frames in unit of second
    }

    void on_render()
    {
    }
};
#endif // !_GAME_MANAGER_H
