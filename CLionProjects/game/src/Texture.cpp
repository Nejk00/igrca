#include"Texture.hpp"

SDL_Texture* Texture::LoadTexture(const char* texture) {
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game :: renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void Texture::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game :: renderer, tex, &src, &dest, NULL, NULL, flip);

}
void Texture::RenderNumber(int number, int x, int y, float scale) {
    if (!Game::numbersTexture) return;

    float renderScale = scale * (static_cast<float>(Game::SCREEN_WIDTH) / 1080);
    const int DIGIT_WIDTH = 16 * renderScale;
    const int SPACING = 2 * renderScale;

    // Convert coordinates
    int renderX = Game::scaleX * x;
    int renderY = Game::scaleY * y;

    std::string numStr = std::to_string(number);

    // Center calculation removed for positional accuracy
    for (size_t i = 0; i < numStr.size(); ++i) {
        char c = numStr[i];
        if (c < '0' || c > '9') continue;

        int digit = c - '0';
        SDL_Rect srcRect = { digit * 16, 0, 16, 16 };
        SDL_Rect destRect = {
            renderX + static_cast<int>(i * (DIGIT_WIDTH + SPACING)),
            renderY,
            static_cast<int>(DIGIT_WIDTH),
            static_cast<int>(16 * renderScale)
        };
        SDL_RenderCopy(Game::renderer, Game::numbersTexture, &srcRect, &destRect);
    }
}

void Texture::RenderText(const std::string& text, int x, int y, float scale) {
    if (!Game::lettersTexture) return;

    int renderX = Game::scaleX * x;
    int renderY = Game::scaleY * y;
    float renderScale = scale * (static_cast<float>(Game::SCREEN_WIDTH) / 1080);

    const int destWidth = Game::LETTER_WIDTH * renderScale;
    const int destHeight = Game::LETTER_HEIGHT * renderScale;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        int index = toupper(c) - 'A'; // Assuming texture starts with 'A' at index 0
        if (c == ' ') continue;
        if (index < 0 || index >= 26) index = 26; // Assume '?' for invalid

        SDL_Rect src = { index * Game::LETTER_WIDTH, 0, Game::LETTER_WIDTH, Game::LETTER_HEIGHT };
        SDL_Rect dest = {
            renderX + static_cast<int>(i * destWidth * 1.2f),
            renderY,
            destWidth,
            destHeight
        };
        SDL_RenderCopy(Game::renderer, Game::lettersTexture, &src, &dest);
    }
}