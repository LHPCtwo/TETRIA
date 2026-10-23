#pragma once
#include "../core/vec3.hpp"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

namespace render {

// Input de teclado REAL via terminal (POSIX termios, modo raw + não
// bloqueante) — não é GUI/mouse (sem display neste ambiente, ver
// README v8), mas é leitura real de bytes vindos de stdin, não um valor
// simulado incrementado por fórmula fixa. Funciona com um humano
// digitando num terminal interativo de verdade; testável também
// enviando bytes reais por um pipe (ver tests/test_keyboard_input.cpp),
// o que prova o mecanismo de leitura em si, não substitui o teclado.
//
// Teclas: w/s = frente/trás (z), a/d = esquerda/direita (x), p = tirar
// foto (modo panorâmico, README v22/v23), q = sair.
class KeyboardInput {
public:
    KeyboardInput() {
        is_tty_ = isatty(STDIN_FILENO);
        if (is_tty_) {
            tcgetattr(STDIN_FILENO, &original_termios_);
            struct termios raw = original_termios_;
            raw.c_lflag &= ~(ICANON | ECHO); // modo raw: sem buffer de linha, sem eco
            raw.c_cc[VMIN] = 0;
            raw.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            raw_mode_set_ = true;
        }
        // Também funciona sem TTY (ex.: entrada vindo de um pipe/arquivo,
        // como em teste automatizado) — nesse caso não mexe em termios,
        // só lê o que chegar em stdin, não bloqueante via fcntl.
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    ~KeyboardInput() {
        if (raw_mode_set_) {
            tcsetattr(STDIN_FILENO, TCSANOW, &original_termios_);
        }
    }

    // Lê todas as teclas disponíveis em stdin agora (não bloqueia) e
    // atualiza a posição acumulada. Retorna false se 'q' foi pressionado
    // (sinal real de saída, vindo de input, não de contador de frames).
    bool PollAndApply(Vec3& pos, float step = 1.0f) {
        char buf[64];
        ssize_t n;
        bool got_input = false;
        bool quit = false;
        while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
            got_input = true;
            for (ssize_t i = 0; i < n; ++i) {
                switch (buf[i]) {
                    case 'w': pos.z += step; break;
                    case 's': pos.z -= step; break;
                    case 'd': pos.x += step; break;
                    case 'a': pos.x -= step; break;
                    case 'p': photo_requested_ = true; break; // não sai do loop, só marca
                    case 'q': quit = true; break; // marca, não sai ainda (bug corrigido: precisa registrar got_input)
                    default: break;
                }
            }
        }
        last_had_input_ = got_input;
        return !quit;
    }

    // Consome o pedido de foto (tecla 'p'): retorna se havia um pedido
    // pendente e reseta a flag — chamado 1x por frame por quem decide a
    // ramificação Draw() vs Compile() (mutuamente exclusivas).
    bool ConsumePhotoRequest() {
        bool r = photo_requested_;
        photo_requested_ = false;
        return r;
    }

    bool IsInteractiveTTY() const { return is_tty_; }
    bool LastPollHadInput() const { return last_had_input_; }

private:
    bool is_tty_ = false;
    bool raw_mode_set_ = false;
    bool last_had_input_ = false;
    bool photo_requested_ = false;
    struct termios original_termios_{};
};

} // namespace render
