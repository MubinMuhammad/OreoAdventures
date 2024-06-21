#ifndef GAME_ENGINE_TIME_H_
#define GAME_ENGINE_TIME_H_

namespace gameEngine {
  struct Time {
    float m_crnt;
    float m_last;
    float m_delta;
    void update();
  };
}

#endif
