#include "pch.h"
#include "Renderer/Renderer.h"
#include "Math/Transform.h"
#include "Renderer/Model.h"
#include "Math/MathUtil.h"
#include "Renderer/Texture.h"
#include "Math/Rect.h"

namespace nu
{
    void Renderer::Quit()
    {
        TTF_Quit();
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool Renderer::Initialize(const char* name, int width, int height)
    {
        m_screen_size.x = (float)width;
        m_screen_size.y = (float)height;

        SDL_Init(SDL_INIT_VIDEO);

        if (!TTF_Init()) {
            std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_window = SDL_CreateWindow(name, width, height, 0);
        if (m_window == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        m_renderer = SDL_CreateRenderer(m_window, NULL);
        if (m_renderer == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        SDL_SetDefaultTextureScaleMode(m_renderer, SDL_SCALEMODE_PIXELART);
        SDL_SetRenderVSync(m_renderer, 1);

        return true;
    }

    void Renderer::SetColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) const
    {
        SDL_SetRenderDrawColor(m_renderer, red, green, blue, alpha);
    }

    void Renderer::SetColorFloat(float red, float green, float blue, float alpha) const
    {
        SDL_SetRenderDrawColorFloat(m_renderer, red, green, blue, alpha);
    }

    void Renderer::Clear() {
        SDL_RenderClear(m_renderer);
    }

    void Renderer::RenderPresent() const
    { 
        SDL_RenderPresent(m_renderer);
    }

    void Renderer::RenderPoint(float x, float y) const
    {
        SDL_RenderPoint(m_renderer, x, y);
    }

    void Renderer::RenderLine(float x1, float y1, float x2, float y2) const
    {
        SDL_RenderLine(m_renderer, x1, y1, x2, y2);
    }

    void Renderer::RenderRect(float x, float y, float width, float height, bool fill = true) const
    {
        SDL_FRect rect{ x, y, width, height };
        (fill) ? SDL_RenderFillRect(m_renderer, &rect) : SDL_RenderRect(m_renderer, &rect);
    }

    void Renderer::DrawModel(const Model& model, const Transform& transform) const
    {
        for (auto& mesh : model.GetMeshes()) {
            SetColorFloat(mesh.GetColor().r, mesh.GetColor().g, mesh.GetColor().b, 1.0f);
            auto& points = mesh.GetPoints();
            for (int i = 0; i < points.size(); ++i)
            {
                if (i == 0) { continue; }
                Vector2 v1 = points.at(i - 1);
                Vector2 v2 = points.at(i);

                v1 *= transform.scale;
                v2 *= transform.scale;

                v1 = v1.Rotate(transform.rotation * DegToRad);
                v2 = v2.Rotate(transform.rotation * DegToRad);

                v1 += transform.position;
                v2 += transform.position;

                RenderLine(v1.x, v1.y, v2.x, v2.y);
            }
        }
        
    }


    void Renderer::DrawTexture(const Texture& texture, float x, float y, float angle, float scale, bool flipH, bool flipV) const
    {
        Vector2 size = texture.GetSize();

        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect destRect;
        destRect.w = size.x * scale;
        destRect.h = size.y * scale;
        destRect.x = (x - cameraX) - (destRect.w * 0.5f);
        destRect.y = (y - cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, NULL, &destRect, angle, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Transform& transform, bool flipH, bool flipV) const
    {
        Vector2 size = texture.GetSize();
        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect destRect;
        destRect.w = size.x * transform.scale;
        destRect.h = size.y * transform.scale;


        destRect.x = (transform.position.x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (transform.position.y + cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, NULL, &destRect, transform.rotation, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Rect& source, float x, float y, float angle, float scale, bool flipH, bool flipV) const
    {
        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect sourceRect;
        sourceRect.x = source.x;
        sourceRect.y = source.y;
        sourceRect.w = source.w;
        sourceRect.h = source.h;

        SDL_FRect destRect;
        destRect.w = source.w * scale;
        destRect.h = source.h * scale;
        destRect.x = (x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (y + cameraY) - (destRect.h * 0.5f);

        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, &sourceRect, &destRect, angle, NULL, flip_type);
    }

    void Renderer::DrawTexture(const Texture& texture, const Rect& source, const Transform& transform, bool flipH, bool flipV) const
    {

        float cameraX = (m_camera_enabled) ? m_camera.x - GetWindowWidth() * 0.5f : 0.0f;
        float cameraY = (m_camera_enabled) ? m_camera.y - GetWindowHeight() * 0.5f : 0.0f;

        SDL_FRect sourceRect;
        sourceRect.x = source.x;
        sourceRect.y = source.y;
        sourceRect.w = source.w;
        sourceRect.h = source.h;

        SDL_FRect destRect;
        destRect.w = source.w * transform.scale;
        destRect.h = source.h * transform.scale;

        destRect.x = (transform.position.x + cameraX) - (destRect.w * 0.5f);
        destRect.y = (transform.position.y + cameraY) - (destRect.h * 0.5f);


        // https://wiki.libsdl.org/SDL3/SDL_RenderTexture
        SDL_FlipMode flip_type = SDL_FLIP_NONE;
        if (flipH && flipV)
        {
            flip_type = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
        }
        else if (flipH)
        {
            flip_type = SDL_FLIP_HORIZONTAL;
        }
        else if (flipV)
        {
            flip_type = SDL_FLIP_VERTICAL;
        }
        SDL_RenderTextureRotated(m_renderer, texture.m_texture, &sourceRect, &destRect, transform.rotation, NULL, flip_type);
    }

    Renderer::~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
};