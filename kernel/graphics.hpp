#pragma once

#include "frame_buffer_config.hpp"

struct PixelColor {
  uint8_t r, g, b;
};

class PixelWriter {
  public:
    // コンストラクタ: 初期化子リストで config_ を初期化
    PixelWriter(const FrameBufferConfig& config) : config_{config} {
    }
    // デストラクタ: 明示的なデフォルト定義
    virtual ~PixelWriter() = default;
    // 純粋仮想関数
    virtual void Write(int x, int y, const PixelColor& c) = 0;

  // protected はサブクラスからの参照をできるようにする
  protected:
    uint8_t* PixelAt(int x, int y) {
      return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
    }

  private:
    const FrameBufferConfig& config_;
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter {
  public:
    // 継承コンストラクタ: 親クラスのコンストラクタを使えるようにする
    using PixelWriter::PixelWriter;

    virtual void Write(int x, int y, const PixelColor& c) override;
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
  public:
    // 親クラスのコンストラクタを使えるようにする
    using PixelWriter::PixelWriter;

    virtual void Write(int x, int y, const PixelColor& c) override;
};

template <typename T>
struct Vector2D {
  T x, y;

  template<typename U>
  Vector2D<T>& operator +=(const Vector2D<U>& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
};

void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                    const Vector2D<int>& size, const PixelColor& c);
void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                    const Vector2D<int>& size, const PixelColor& c);