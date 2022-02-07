#include "window.hpp"

Window::Window(int width, int height) : width_{width}, height_{height} {
  data_.resize(height);
  for (int y = 0; y < height; y++) {
    data_.resize(width);
  }
}

void Window::DrawTo(PixelWriter& writer, Vector2D<int> position) {
  if (!transparent_color_) {
    for (int y = 0; y < Height(); y++) {
      for (int x = 0; x < Width(); x++) {
        writer.Write(position.x + x, position.y + y, At(x, y));
      }
    }
    return;
  }
}
