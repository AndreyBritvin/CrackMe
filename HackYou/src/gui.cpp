#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "patcher.h"

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

// Функция для рендеринга текста на экране
err_code_t render_text(SDL_Renderer* renderer, TTF_Font* font, const char* message, int x, int y)
{
    assert(renderer);
    assert(font);
    assert(message);

    SDL_Color textColor = {161, 30, 227}; // Розовый цвет текста

    // Создание текстовой поверхности из строки message
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, textColor);
    if (textSurface == NULL)
    {
        printf("Не удалось создать поверхность текста: %s\n", TTF_GetError());
        return 1;
    }

    // Создание текстуры из поверхности
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Не удалось создать текстуру из поверхности: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h}; // Позиция и размер текста
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);     // Рендерим текст
    SDL_DestroyTexture(textTexture);                            // Освобождаем текстуру
    SDL_FreeSurface(textSurface);                               // Освобождаем память поверхности
                                                                //        после создания текстуры
    return OK;
}

// TODO: make enum for all errors
// TODO: make all this initialisations to functions?

int init_sdl(patcher_t patcher)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Ошибка инициализации TTF: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Lykatcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        printf("Ошибка создания окна: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Ошибка создания рендерера: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *background = NULL;

    SDL_Surface *bg_surface = IMG_Load("img/image.png");
    if (!bg_surface)
    {
        printf("Ошибка загрузки изображения: %s\n", IMG_GetError());
    }
    else
    {
        background = SDL_CreateTextureFromSurface(renderer, bg_surface);
        SDL_FreeSurface(bg_surface);
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music *music = Mix_LoadMUS("music.mp3");
    if (!music)
    {
        printf("Ошибка загрузки музыки: %s\n", Mix_GetError());
    }
    else
    {
        Mix_PlayMusic(music, -1);
    }

    TTF_Font* font = TTF_OpenFont("fonts/font.ttf", 40);
    if (!font)
    {
        printf("Не удалось загрузить шрифт: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Кнопки "Патч" и "Выход"
    SDL_Rect patchButton = { 100, 500, 200, 50 };
    SDL_Rect exitButton  = { 500, 500, 200, 50 };
    char message[32] = " ";                        // Инициализация строки для сообщений

    SDL_bool running = SDL_TRUE;
    while (running)
    {
        SDL_Event event = {};
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = SDL_FALSE; // Закрыть программу
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.x >= exitButton.x && event.button.x <= exitButton.x + exitButton.w &&
                    event.button.y >= exitButton.y && event.button.y <= exitButton.y + exitButton.h)
                {
                    running = SDL_FALSE; // Закрыть программу
                }

                if (event.button.x >= patchButton.x && event.button.x <= patchButton.x + patchButton.w &&
                    event.button.y >= patchButton.y && event.button.y <= patchButton.y + patchButton.h)
                {
                //     printf("Введите путь к файлу: ");
                //     char filename[1024] = {};
                //     scanf("%1023s", filename); // Ввод имени файла
                    err_code_t result = patcher("PASSWO_1.com");

                    if (result == PATCHED)
                    {
                        snprintf(message, sizeof(message), "Success");
                    }
                    else if (result == ALREADY_PATCHED)
                    {
                        snprintf(message, sizeof(message), "Already patched");
                    }
                    else
                    {
                        snprintf(message, sizeof(message), "Wrong file");
                    }
                }
            }
        }

        SDL_RenderClear(renderer);

        if (background)
        {
            SDL_RenderCopy(renderer, background, NULL, NULL);
        }

        // Рисуем кнопки
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        // Рисуем кнопку "Патч"
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);   // Синий цвет для кнопки
        SDL_RenderFillRect(renderer, &patchButton);             // Рисуем кнопку
        render_text(renderer, font, "PaTch", patchButton.x + 50, patchButton.y + 10);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &exitButton);
        render_text(renderer, font, "eXiT", exitButton.x + 50, exitButton.y + 10);

        // Отображаем текст в зависимости от состояния
        if (message[0] != ' ')
        {
            render_text(renderer, font, message, 250, 125);
        }

        SDL_RenderPresent(renderer); // Отображаем изменения на экране
    }

    // Освобождаем ресурсы
    TTF_CloseFont(font);
    if (background) SDL_DestroyTexture(background);
    if (music)      Mix_FreeMusic(music);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
